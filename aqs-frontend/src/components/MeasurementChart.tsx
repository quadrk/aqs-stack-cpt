import { useEffect, useState, useMemo } from "react";
import { useNavigate } from "react-router-dom";
import { getMeasurements, getDevices, Measurement, Device } from "../services/api";
import { exportAsCSV, exportAsXLSX, exportAsPDF } from "../utils/export";
import { sensorColors, sensorNames, sensorUnits } from "../styles/theme";
import { MainLayout } from "../components/layout";
import {
  Line,
  XAxis,
  YAxis,
  CartesianGrid,
  Tooltip,
  ResponsiveContainer,
  Legend,
  ComposedChart,
} from "recharts";

const SensorCheckbox = ({
  sensorId,
  isSelected,
  onChange,
}: {
  sensorId: string;
  isSelected: boolean;
  onChange: (sensorId: string) => void;
}) => {
  const color = sensorColors[sensorId as keyof typeof sensorColors] || '#757575';
  const name = sensorNames[sensorId as keyof typeof sensorNames] || sensorId;
  const unit = sensorUnits[sensorId as keyof typeof sensorUnits] || '';
  return (
    <div
      className={`sensor-chip ${isSelected ? 'selected' : ''}`}
      onClick={() => onChange(sensorId)}
      style={{
        borderColor: isSelected ? color : 'var(--gray-300)',
        backgroundColor: isSelected ? `${color}10` : 'var(--background-paper)',
      }}
    >
      <div
        className="sensor-chip-icon"
        style={{
          backgroundColor: color,
          opacity: isSelected ? 1 : 0.3,
        }}
      ></div>
      <span className="sensor-chip-text">{name} {unit && `(${unit})`}</span>
    </div>
  );
};

const StatCard = ({
  title,
  value,
  description,
}: {
  title: string;
  value: string | number;
  description?: string;
}) => (
  <div className="stat-card">
    <div className="stat-card-title">{title}</div>
    <div className="stat-card-value">{value}</div>
    {description && <div className="stat-card-description">{description}</div>}
  </div>
);

const CustomTooltip = ({ active, payload, label }: any) => {
  if (active && payload && payload.length) {
    const date = new Date(label);
    return (
      <div style={{
        backgroundColor: 'var(--background-paper)',
        border: '1px solid var(--gray-200)',
        borderRadius: 'var(--radius-md)',
        padding: 'var(--spacing-sm)',
        boxShadow: 'var(--shadow-sm)'
      }}>
        <p style={{
          margin: '0 0 var(--spacing-xs) 0',
          fontWeight: 600,
          borderBottom: '1px solid var(--gray-200)',
          paddingBottom: 'var(--spacing-xxs)',
          fontSize: '0.875rem'
        }}>
          {date.toLocaleString()}
        </p>
        <div style={{ display: 'flex', flexDirection: 'column', gap: '4px' }}>
          {payload.map((entry: any, index: number) => {
            const sensorId = entry.dataKey;
            const name = sensorNames[sensorId as keyof typeof sensorNames] || sensorId;
            const unit = sensorUnits[sensorId as keyof typeof sensorUnits] || '';
            return (
              <div key={`item-${index}`} style={{
                display: 'flex',
                alignItems: 'center',
                fontSize: '0.8125rem'
              }}>
                <div style={{
                  width: '8px',
                  height: '8px',
                  backgroundColor: entry.color,
                  marginRight: '8px',
                  borderRadius: '50%'
                }} />
                <span>
                  {name}: <span style={{ fontWeight: 600 }}>{entry.value.toFixed(2)}</span> {unit}
                </span>
              </div>
            );
          })}
        </div>
      </div>
    );
  }
  return null;
};

const Dropdown = ({
  label,
  value,
  options,
  onChange,
  disabled = false
}: {
  label: string;
  value: string;
  options: { value: string; label: string }[];
  onChange: (value: string) => void;
  disabled?: boolean;
}) => (
  <div className="input-container" style={{ marginBottom: 0 }}>
    <label className="input-label" htmlFor={`dropdown-${label}`}>{label}</label>
    <select
      id={`dropdown-${label}`}
      className="input"
      value={value}
      onChange={(e) => onChange(e.target.value)}
      disabled={disabled}
      style={{
        backgroundColor: 'var(--background-paper)',
        color: 'var(--text-primary)',
        border: '1px solid var(--gray-600)',
        borderRadius: 'var(--radius-md)',
        padding: '10px 12px',
        width: '100%',
        cursor: disabled ? 'not-allowed' : 'pointer',
        opacity: disabled ? 0.7 : 1
      }}
    >
      {options.map((option) => (
        <option key={option.value} value={option.value}>
          {option.label}
        </option>
      ))}
    </select>
  </div>
);

const EmptyChart = ({ deviceId, availableIds }: { deviceId?: string, availableIds?: string[] }) => (
  <div className="chart-empty">
    <div className="text-center mb-lg">
      <h1 className="mb-xs">Nie sú k dispozícii žiadne údaje</h1>
      <p className="text-secondary">Vyberte senzory na zobrazenie alebo počkajte na príjem údajov</p>
      {deviceId && (
        <div style={{
          marginTop: '20px', padding: '15px',
          background: '#ff930010', border: '1px solid #ff930050',
          borderRadius: '8px', fontSize: '14px', textAlign: 'left'
        }}>
          <h3 style={{ fontSize: '16px', marginBottom: '10px', color: '#ff9300' }}>Debug info</h3>
          <p><strong>Aktuálne zariadenie ID:</strong> {deviceId}</p>
          {availableIds && availableIds.length > 0 && (
            <>
              <p><strong>Dostupné zariadenia ID:</strong></p>
              <ul style={{ paddingLeft: '20px' }}>
                {availableIds.map(id => (<li key={id}>{id}</li>))}
              </ul>
            </>
          )}
        </div>
      )}
    </div>
  </div>
);

const formatXAxis = (tickItem: string) => {
  const date = new Date(tickItem);
  return `${date.getHours().toString().padStart(2, '0')}:${date.getMinutes().toString().padStart(2, '0')}`;
};

export default function Dashboard() {
  const [data, setData] = useState<Measurement[]>([]);
  const [devices, setDevices] = useState<Device[]>([]);
  const [selectedSensors, setSelectedSensors] = useState<string[]>([]);
  const [selectedLocation, setSelectedLocation] = useState<string>("");
  const [selectedDeviceId, setSelectedDeviceId] = useState<string>("");
  const [isLoading, setIsLoading] = useState(true);
  const [isLoadingDevices, setIsLoadingDevices] = useState(true);

  const navigate = useNavigate();

  // Уникальные локации
  const locations = useMemo(() => (
    [...new Set(devices.map(d => d.location || "Neznáma lokalita"))].map(loc => ({ value: loc, label: loc }))
  ), [devices]);

  // Устройства в выбранной локации (value — device_id!)
  const deviceOptions = useMemo(() => (
    devices
      .filter(d => (d.location || "Neznáma lokalita") === selectedLocation)
      .map(device => ({ value: device.device_id, label: device.name }))
  ), [devices, selectedLocation]);

  // Загрузка устройств
  useEffect(() => {
    setIsLoadingDevices(true);
    getDevices()
      .then((devices) => {
        setDevices(devices);
        if (devices.length > 0) {
          const firstLocation = devices[0].location || "Neznáma lokalita";
          setSelectedLocation(firstLocation);
          const firstDevice = devices.find(
            d => (d.location || "Neznáma lokalita") === firstLocation
          );
          if (firstDevice) setSelectedDeviceId(firstDevice.device_id);
        }
        setIsLoadingDevices(false);
      })
      .catch(() => setIsLoadingDevices(false));
  }, []);

  // Смена локации
  const handleLocationChange = (location: string) => {
    setSelectedLocation(location);
    const firstDevice = devices.find(
      d => (d.location || "Neznáma lokalita") === location
    );
    if (firstDevice) setSelectedDeviceId(firstDevice.device_id);
    else setSelectedDeviceId("");
    setSelectedSensors([]);
    setIsLoading(true);
  };

  // Смена устройства (UUID)
  const handleDeviceChange = (deviceId: string) => {
    setSelectedDeviceId(deviceId); // deviceId — UUID!
    setSelectedSensors([]);
    setIsLoading(true);
  };

  // Получение измерений для выбранного устройства + ОТЛАДКА!
  useEffect(() => {
    if (!selectedDeviceId) {
      setData([]);
      setIsLoading(false);
      return;
    }
    setIsLoading(true);
    getMeasurements()
      .then(received => {
        // ==== ОТЛАДОЧНЫЕ ЛОГИ! ====
        console.log("selectedDeviceId:", selectedDeviceId);
        console.log("device_id в полученных данных:", [...new Set(received.map(m => m.device_id))]);
        if (received.length > 0) {
          console.log("sample measurement:", received[0]);
        }
        // ===========================
        const filtered = received.filter(m => m.device_id === selectedDeviceId);
        setData(filtered);
        if (filtered.length > 0) {
          const sensors = [...new Set(filtered.map(m => m.sensor_id))];
          setSelectedSensors(sensors.slice(0, Math.min(2, sensors.length)));
        } else {
          setSelectedSensors([]);
        }
        setIsLoading(false);
      })
      .catch(() => {
        setData([]);
        setSelectedSensors([]);
        setIsLoading(false);
      });
  }, [selectedDeviceId]);

  const handleLogout = async () => {
    localStorage.removeItem("access_token");
    localStorage.removeItem("refresh_token");
    navigate("/login");
  };

  const toggleSensor = (sensorId: string) => {
    setSelectedSensors((prev) =>
      prev.includes(sensorId)
        ? prev.filter((item) => item !== sensorId)
        : [...prev, sensorId]
    );
  };

  const sensorList = [...new Set(data.map(m => m.sensor_id))];
  const filteredData = data.filter(m => selectedSensors.includes(m.sensor_id));
  const hasData = filteredData.length > 0;

  const grouped = new Map<string, Record<string, any>>();
  filteredData.forEach((m) => {
    const time = new Date(m.ts).toISOString();
    if (!grouped.has(time)) {
      grouped.set(time, {
        ts: time,
        time: new Date(m.ts).getTime(),
        formattedTime: formatXAxis(time),
      });
    }
    grouped.get(time)![m.sensor_id] = m.value;
  });
  const chartData = Array.from(grouped.values()).sort((a, b) => a.time - b.time);

  const statistics = selectedSensors.reduce((acc, sensorId) => {
    const sensorData = filteredData.filter(m => m.sensor_id === sensorId);
    const values = sensorData.map(m => m.value);
    if (values.length === 0) return acc;
    acc[sensorId] = {
      min: Math.min(...values),
      max: Math.max(...values),
      avg: values.reduce((sum, val) => sum + val, 0) / values.length,
      count: values.length,
      unit: sensorUnits[sensorId as keyof typeof sensorUnits] || '',
      name: sensorNames[sensorId as keyof typeof sensorNames] || sensorId,
    };
    return acc;
  }, {} as Record<string, { min: number; max: number; avg: number; count: number; unit: string; name: string; }>);

  const handleExportCSV = () => {
    if (!hasData || !filteredData.length) return;
    exportAsCSV(filteredData);
  };
  const handleExportXLSX = () => {
    if (!hasData || !filteredData.length) return;
    exportAsXLSX(filteredData);
  };
  const handleExportPDF = () => {
    if (!hasData || !filteredData.length) return;
    exportAsPDF(filteredData);
  };

  return (
    <MainLayout>
      <nav className="navbar">
        <div className="navbar-brand">Systém monitorovania kvality ovzdušia</div>
        <div className="navbar-nav">
          <button className="btn btn-outline btn-sm" onClick={() => navigate("/devices")}>
            Zariadenia
          </button>
          <button className="btn btn-error btn-sm" onClick={handleLogout}>
            Odhlásiť sa
          </button>
        </div>
      </nav>

      <div style={{ padding: 'var(--spacing-lg)' }}>
        <div className="card mb-lg">
          <div className="card-header">
            <h2 className="card-header-title">Parametre monitorovania</h2>
          </div>
          <div className="card-body">
            <div className="mb-lg">
              <h3 className="mb-sm">Filter údajov</h3>
              <div className="grid" style={{ gridTemplateColumns: "1fr 1fr", gap: "var(--spacing-lg)" }}>
                <Dropdown
                  label="Lokalita"
                  value={selectedLocation}
                  options={locations.length > 0 ? locations : [{ value: "", label: "Načítavanie..." }]}
                  onChange={handleLocationChange}
                  disabled={isLoadingDevices || locations.length === 0}
                />
                <Dropdown
                  label="Zariadenie"
                  value={selectedDeviceId}
                  options={deviceOptions.length > 0 ? deviceOptions : [{ value: "", label: "Načítavanie..." }]}
                  onChange={handleDeviceChange}
                  disabled={isLoadingDevices || deviceOptions.length === 0}
                />
              </div>
            </div>
            <div className="mb-md">
              <h3 className="mb-sm">Vyberte senzory na zobrazenie</h3>
              <div className="sensor-selector">
                {sensorList.map((sensorId) => (
                  <SensorCheckbox
                    key={sensorId}
                    sensorId={sensorId}
                    isSelected={selectedSensors.includes(sensorId)}
                    onChange={toggleSensor}
                  />
                ))}
              </div>
            </div>
          </div>
        </div>
        <div className="card mb-lg">
          <div className="card-header">
            <h2 className="card-header-title">Graf</h2>
            {hasData && (
              <div className="export-tools">
                <button className="export-btn" onClick={handleExportCSV}>
                  Export CSV
                </button>
                <button className="export-btn" onClick={handleExportXLSX}>
                  Export Excel
                </button>
                <button className="export-btn" onClick={handleExportPDF}>
                  Export PDF
                </button>
              </div>
            )}
          </div>
          <div className="card-body">
            {isLoading ? (
              <div className="loader-container">
                <div className="loader"></div>
              </div>
            ) : hasData ? (
              <div className="chart-container">
                <ResponsiveContainer width="100%" height="100%">
                  <ComposedChart
                    data={chartData}
                    margin={{ top: 10, right: 30, left: 0, bottom: 0 }}
                  >
                    <CartesianGrid strokeDasharray="3 3" vertical={false} stroke="var(--gray-200)" />
                    <XAxis
                      dataKey="ts"
                      tickFormatter={formatXAxis}
                      tick={{ fill: 'var(--text-secondary)', fontSize: 12 }}
                      axisLine={{ stroke: 'var(--gray-300)' }}
                      tickLine={{ stroke: 'var(--gray-300)' }}
                    />
                    <YAxis
                      tick={{ fill: 'var(--text-secondary)', fontSize: 12 }}
                      axisLine={{ stroke: 'var(--gray-300)' }}
                      tickLine={{ stroke: 'var(--gray-300)' }}
                    />
                    <Tooltip content={<CustomTooltip />} />
                    <Legend
                      formatter={(value) => sensorNames[value as keyof typeof sensorNames] || value}
                      wrapperStyle={{
                        paddingTop: 20,
                        fontSize: '0.875rem',
                        color: 'var(--text-secondary)'
                      }}
                    />
                    {selectedSensors.map((sensorId) => (
                      <Line
                        key={sensorId}
                        type="monotone"
                        dataKey={sensorId}
                        stroke={sensorColors[sensorId as keyof typeof sensorColors] || '#757575'}
                        strokeWidth={2}
                        dot={{ r: 0 }}
                        activeDot={{ r: 6, strokeWidth: 0 }}
                        connectNulls
                      />
                    ))}
                  </ComposedChart>
                </ResponsiveContainer>
              </div>
            ) : (
              <EmptyChart
                deviceId={selectedDeviceId}
                availableIds={[...new Set(data.map(m => m.device_id))]}
              />
            )}
          </div>
        </div>
        {hasData && (
          <div className="card">
            <div className="card-header">
              <h2 className="card-header-title">Statistika podľa senzorov</h2>
            </div>
            <div className="card-body">
              <div className="grid" style={{
                gridTemplateColumns: "repeat(auto-fill, minmax(220px, 1fr))"
              }}>
                {Object.entries(statistics).map(([sensorId, stats]) => (
                  <StatCard
                    key={sensorId}
                    title={stats.name}
                    value={`${stats.avg.toFixed(2)} ${stats.unit}`}
                    description={`Min: ${stats.min.toFixed(2)} ${stats.unit} · Max: ${stats.max.toFixed(2)} ${stats.unit}`}
                  />
                ))}
              </div>
            </div>
          </div>
        )}
      </div>
    </MainLayout>
  );
}
