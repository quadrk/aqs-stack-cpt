/**
 * Система цветов для приложения
 */

// Карта сенсоров и их названий для лучшего отображения
export const sensorNames: Record<string, string> = {
  T: "Температура",
  H: "Влажность",
  PM25: "Частицы PM2.5", 
  NO2: "Диоксид азота",
  CO2: "Углекислый газ",
  CO: "Угарный газ",
  default: "Неизвестный сенсор"
};

// Единицы измерения для различных типов сенсоров
export const sensorUnits: Record<string, string> = {
  T: "°C",
  H: "%",
  PM25: "μg/m³",
  NO2: "ppb",
  CO2: "ppm",
  CO: "ppm",
  default: ""
};

// Цвета для различных типов сенсоров
export const sensorColors: Record<string, string> = {
  T: "#4a7aaf",       // Более мягкий синий
  H: "#7e6c9c",       // Более мягкий фиолетовый
  PM25: "#5a9396",    // Более мягкий бирюзовый
  NO2: "#5d8a60",     // Более мягкий зеленый 
  CO2: "#c78d46",     // Более мягкий оранжевый
  CO: "#b87575",      // Более мягкий красный
  default: "#777777", // Нейтральный серый
};

// Основные цвета приложения
export const colors = {
  primary: {
    light: "#64b5f6",
    main: "#2196f3",
    dark: "#1976d2",
  },
  secondary: {
    light: "#81c784",
    main: "#4caf50",
    dark: "#388e3c",
  },
  error: {
    light: "#e57373",
    main: "#f44336",
    dark: "#d32f2f",
  },
  warning: {
    light: "#ffb74d",
    main: "#ff9800",
    dark: "#f57c00",
  },
  info: {
    light: "#4fc3f7",
    main: "#03a9f4",
    dark: "#0288d1",
  },
  success: {
    light: "#81c784",
    main: "#4caf50",
    dark: "#388e3c",
  },
  grey: {
    50: "#fafafa",
    100: "#f5f5f5",
    200: "#eeeeee",
    300: "#e0e0e0",
    400: "#bdbdbd",
    500: "#9e9e9e",
    600: "#757575",
    700: "#616161",
    800: "#424242",
    900: "#212121",
  },
  text: {
    primary: "#212121",
    secondary: "#757575",
    disabled: "#9e9e9e",
  },
  background: {
    default: "#f8f9fa",
    paper: "#ffffff",
  },
};