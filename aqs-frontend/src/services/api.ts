import axios from "axios";

export const api = axios.create({
  baseURL: import.meta.env.VITE_API_URL,
  headers: {
    "Content-Type": "application/json",
    // "X-API-KEY": ""
  },
});

// Автоматически добавляем токен, если есть
api.interceptors.request.use((config) => {
    const token = localStorage.getItem("access_token");
    if (token) {
      config.headers.Authorization = `Bearer ${token}`;
    }
    return config;
  });

export interface Measurement {
    device_id: string;
    location?: String;
    sensor_id: string;
    ts: string;
    value: number;
  }

  export interface Device {
    id: number;
    device_id: string;
    name: string;
    location?: string;
    description?: string;
    api_key: string;
    is_active: boolean;
    user_email: string;
    created_at: string;
  }  

  export interface DeviceCreate {
    name: string;
    location?: string;
    description?: string;
  }

  export async function getMeasurements(): Promise<Measurement[]> {
    const response = await api.get<Measurement[]>("/admin/measurements/");
    return response.data;
  }

  export async function getDevices(): Promise<Device[]> {
    const response = await api.get<Device[]>("/admin/devices/");
    return response.data;
  }
  
  export async function createDevice(data: DeviceCreate): Promise<Device> {
    const response = await api.post<Device>("/admin/devices/", data);
    return response.data;
  }
  
  