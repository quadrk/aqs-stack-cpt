import { useEffect, useState } from "react";
import { useNavigate } from "react-router-dom";
import { getDevices, Device, createDevice, DeviceCreate } from "../services/api";
import { MainLayout } from "../components/layout";


export default function UserDevicesPage() {
  const [devices, setDevices] = useState<Device[]>([]);
  const [isLoading, setIsLoading] = useState(true);
  const [form, setForm] = useState<DeviceCreate>({
    name: "",
    location: "",
    description: "",
  });
  const [isSubmitting, setIsSubmitting] = useState(false);
  const [copySuccess, setCopySuccess] = useState<string | null>(null);
  
  const navigate = useNavigate();

  // Загружаем список устройств
  useEffect(() => {
    setIsLoading(true);
    getDevices()
      .then((response) => {
        setDevices(response);
        setIsLoading(false);
      })
      .catch((err) => {
        console.error("Chyba pri načítaní zariadení:", err);
        setIsLoading(false);
      });
  }, []);

  // Обработчик создания нового устройства
  const handleCreate = async (e: React.FormEvent) => {
    e.preventDefault();
    
    if (!form.name) return;
    
    setIsSubmitting(true);
    
    try {
      const newDevice = await createDevice(form);
      setDevices((prev) => [...prev, newDevice]);
      // Сбрасываем форму
      setForm({ name: "", location: "", description: "" });
    } catch (err) {
      console.error("Chyba pri vytváraní zariadenia:", err);
    } finally {
      setIsSubmitting(false);
    }
  };

  // Обработчик копирования API ключа
  const copyToClipboard = async (text: string, deviceId: number) => {
    try {
      await navigator.clipboard.writeText(text);
      setCopySuccess(`device-${deviceId}`);
      setTimeout(() => setCopySuccess(null), 2000);
    } catch (err) {
      console.error("Chyba pri kopírovaní:", err);
    }
  };

  // Форматирование даты
  const formatDate = (dateString: string) => {
    const date = new Date(dateString);
    return date.toLocaleDateString() + ' ' + date.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
  };

  return (
    <MainLayout>
      {/* Верхняя навигация */}
      <nav className="navbar">
        <div className="navbar-brand">Správa zariadení</div>
        <div className="navbar-nav">
          <button className="btn btn-outline btn-sm" onClick={() => navigate("/")}>
            Späť na grafy
          </button>
        </div>
      </nav>

      <div style={{ padding: 'var(--spacing-lg)' }}>
        {/* Список устройств */}
        <div className="card mb-lg">
          <div className="card-header">
            <h2 className="card-header-title">Moje zariadenia</h2>
          </div>
          <div className="card-body">
            {isLoading ? (
              <div className="loader-container">
                <div className="loader"></div>
              </div>
            ) : devices.length === 0 ? (
              <div className="text-center py-xl">
                <h3 className="mb-xs">Zatiaľ nemáte žiadne zariadenia</h3>
                <p className="text-secondary mb-md">
                  Použite formulár nižšie na pridanie vášho prvého zariadenia na monitorovanie
                </p>
              </div>
            ) : (
              <div className="table-container">
                <table className="table">
                  <thead>
                    <tr>
                      <th>Názov</th>
                      <th>Lokalita</th>
                      <th>Popis</th>
                      <th>API kľúč</th>
                      <th>Vytvorené</th>
                    </tr>
                  </thead>
                  <tbody>
                    {devices.map((device) => (
                      <tr key={device.id}>
                        <td>
                          <div style={{ fontWeight: 500 }}>{device.name}</div>
                        </td>
                        <td>{device.location || "—"}</td>
                        <td>{device.description || "—"}</td>
                        <td>
                          <div className="flex items-center gap-xs">
                            <code
                              style={{
                                fontSize: "0.8125rem",
                                backgroundColor: "rgba(255, 255, 255, 0.05)",
                                padding: "0.25rem 0.5rem",
                                borderRadius: "4px",
                                fontFamily: "monospace",
                                maxWidth: "180px",
                                overflow: "hidden",
                                textOverflow: "ellipsis",
                                whiteSpace: "nowrap",
                                border: "1px solid rgba(255, 255, 255, 0.1)",
                                color: "var(--text-primary)"
                              }}
                            >
                              {device.api_key}
                            </code>
                            <button
                              className="btn btn-ghost btn-icon btn-sm"
                              onClick={() => copyToClipboard(device.api_key, device.id)}
                              title="Kopírovať API kľúč"
                              style={{
                                minHeight: "auto",
                                width: "auto",
                                height: "auto",
                                color: copySuccess === `device-${device.id}` ? 'var(--success-500)' : 'var(--text-secondary)',
                                fontWeight: 'bold'
                              }}
                            >
                              Kopírovať
                            </button>
                          </div>
                        </td>
                        <td>{formatDate(device.created_at)}</td>
                      </tr>
                    ))}
                  </tbody>
                </table>
              </div>
            )}
          </div>
        </div>

        {/* Форма добавления устройства */}
        <div className="card">
          <div className="card-header">
            <h2 className="card-header-title">Pridať nové zariadenie</h2>
          </div>
          <div className="card-body">
            <form onSubmit={handleCreate}>
              <div className="grid" style={{ gridTemplateColumns: "1fr 1fr", gap: "var(--spacing-lg)" }}>
                <div className="input-container">
                  <label className="input-label" htmlFor="device-name">Názov zariadenia</label>
                  <input
                    id="device-name"
                    className="input"
                    type="text"
                    value={form.name}
                    onChange={(e) => setForm({ ...form, name: e.target.value })}
                    required
                    placeholder="Zadajte názov zariadenia"
                  />
                </div>
                
                <div className="input-container">
                  <label className="input-label" htmlFor="device-location">Lokalita</label>
                  <input
                    id="device-location"
                    className="input"
                    type="text"
                    value={form.location || ""}
                    onChange={(e) => setForm({ ...form, location: e.target.value })}
                    placeholder="Napríklad: Tajovskeho 40"
                  />
                  <div className="input-hint">Uveďte, kde sa zariadenie nachádza</div>
                </div>
              </div>
              
              <div className="input-container">
                <label className="input-label" htmlFor="device-description">Popis</label>
                <textarea
                  id="device-description"
                  className="input"
                  value={form.description || ""}
                  onChange={(e) => setForm({ ...form, description: e.target.value })}
                  placeholder="Dodatočné informácie o zariadení"
                  rows={3}
                ></textarea>
              </div>
              
              <div className="flex justify-between items-center mt-lg">
                <div className="text-sm text-secondary">
                  Po vytvorení zariadenia dostanete jedinečný API kľúč
                </div>
                <button
                  type="submit"
                  className="btn btn-secondary"
                  disabled={isSubmitting || !form.name}
                >
                  {isSubmitting ? (
                    <>
                      <div className="loader" style={{ borderTop: "2px solid white", width: "16px", height: "16px" }}></div>
                      <span className="ml-xs">Vytváranie...</span>
                    </>
                  ) : (
                    "Vytvoriť zariadenie"
                  )}
                </button>
              </div>
            </form>
          </div>
        </div>
      </div>
    </MainLayout>
  );
}