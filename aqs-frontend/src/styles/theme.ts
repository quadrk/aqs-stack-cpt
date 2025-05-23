/**
 * Современная система дизайна для приложения
 */

// Типизация системы цветов
export interface ColorPalette {
  50: string;
  100: string;
  200: string;
  300: string;
  400: string;
  500: string;
  600: string;
  700: string;
  800: string;
  900: string;
}

// Основная палитра (темнее)
export const colors = {
  primary: {
    50: '#ccddf0',
    100: '#a3c2e3',
    200: '#7aa7d6',
    300: '#518cc9',
    400: '#3971bd',
    500: '#2156b0',
    600: '#1b499d',
    700: '#143c8a',
    800: '#0e2f77',
    900: '#082364',
  },
  secondary: {
    50: '#cfe7d1',
    100: '#a0d0a3',
    200: '#71b976',
    300: '#52a259',
    400: '#438b4a',
    500: '#34743b',
    600: '#265d2c',
    700: '#17461d',
    800: '#0f2f13',
    900: '#061809',
  },
  gray: {
    50: '#e2e2e2',
    100: '#d2d2d2',
    200: '#c2c2c2',
    300: '#b2b2b2',
    400: '#a2a2a2',
    500: '#929292',
    600: '#6b6b6b',
    700: '#5b5b5b',
    800: '#323232',
    900: '#121212',
  },
  error: {
    50: '#f9d7d7',
    100: '#f3b0b0',
    200: '#ed8a8a',
    300: '#e66363',
    400: '#e03d3d',
    500: '#d91616',
    600: '#b31212',
    700: '#8c0e0e',
    800: '#660a0a',
    900: '#3f0606',
  },
  warning: {
    50: '#ffefc8',
    100: '#ffe091',
    200: '#ffd05a',
    300: '#ffc123',
    400: '#ffb200',
    500: '#d99900',
    600: '#b38000',
    700: '#8c6600',
    800: '#664c00',
    900: '#3f3200',
  },
  info: {
    50: '#c8e9fd',
    100: '#91d3fc',
    200: '#5abefb',
    300: '#23a8fa',
    400: '#0792e4',
    500: '#0078cc',
    600: '#005ea3',
    700: '#00447a',
    800: '#002b52',
    900: '#001229',
  },
  success: {
    50: '#d4ebdb',
    100: '#a9d7b7',
    200: '#7ec493',
    300: '#53b06f',
    400: '#389c5a',
    500: '#1d8845',
    600: '#146c37',
    700: '#0c5129',
    800: '#06351b',
    900: '#021a0d',
  },
  background: {
    default: '#1e1e1e',
    paper: '#2d2d2d',
    dark: '#252525',
  },
  text: {
    primary: '#e0e0e0',
    secondary: '#b0b0b0',
    disabled: '#707070',
    hint: '#909090',
  },
  divider: 'rgba(255, 255, 255, 0.12)',
};

// Карта сенсоров и их названий
export const sensorNames: Record<string, string> = {
  T: 'Teplota',
  H: 'Vlhkosť',
  PM25: 'Častice PM2.5',
  NO2: 'Oxid dusičitý',
  CO2: 'Oxid uhličitý',
  CO: 'Oxid uhoľnatý',
  LUX: 'Osvetlenie',
  NOISE: 'Hluk',
  default: 'Neznámy senzor',
};

// Единицы измерения для сенсоров
export const sensorUnits: Record<string, string> = {
  T: '°C',
  H: '%',
  PM25: 'µg/m³',
  NO2: 'ppb',
  CO2: 'ppm',
  CO: 'ppm',
  LUX: 'lx',
  NOISE: 'dB',
  default: '',
};

// Цвета для сенсоров (более современные)
export const sensorColors: Record<string, string> = {
  T: '#4285f4',      // Google Blue
  H: '#673ab7',      // Deep Purple
  PM25: '#00acc1',   // Cyan
  NO2: '#43a047',    // Green
  CO2: '#fb8c00',    // Orange
  CO: '#e53935',     // Red
  LUX: '#ffb300',    // Amber
  NOISE: '#546e7a',  // Blue Grey
  default: '#757575', // Grey
};

// Типографика
export const typography = {
  fontFamily: `-apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, 'Helvetica Neue', Arial, sans-serif`,
  fontSize: 14,
  fontWeights: {
    regular: 400,
    medium: 500,
    semibold: 600,
    bold: 700,
  },
  h1: {
    fontSize: '2rem',
    fontWeight: 600,
    lineHeight: 1.25,
    letterSpacing: '-0.01em',
  },
  h2: {
    fontSize: '1.5rem',
    fontWeight: 600,
    lineHeight: 1.3,
    letterSpacing: '-0.01em',
  },
  h3: {
    fontSize: '1.25rem',
    fontWeight: 600,
    lineHeight: 1.35,
    letterSpacing: '-0.01em',
  },
  h4: {
    fontSize: '1.125rem',
    fontWeight: 600,
    lineHeight: 1.4,
    letterSpacing: '-0.005em',
  },
  h5: {
    fontSize: '1rem',
    fontWeight: 600,
    lineHeight: 1.5,
    letterSpacing: '0',
  },
  body1: {
    fontSize: '1rem',
    fontWeight: 400,
    lineHeight: 1.5,
    letterSpacing: '0',
  },
  body2: {
    fontSize: '0.875rem',
    fontWeight: 400,
    lineHeight: 1.5,
    letterSpacing: '0.01em',
  },
  button: {
    fontSize: '0.875rem',
    fontWeight: 500,
    lineHeight: 1.5,
    letterSpacing: '0.01em',
    textTransform: 'none',
  },
  caption: {
    fontSize: '0.75rem',
    fontWeight: 400,
    lineHeight: 1.5,
    letterSpacing: '0.02em',
  },
};

// Тени
export const shadows = {
  xs: '0 1px 2px rgba(0, 0, 0, 0.05)',
  sm: '0 1px 3px rgba(0, 0, 0, 0.08)',
  md: '0 4px 6px rgba(0, 0, 0, 0.04), 0 1px 3px rgba(0, 0, 0, 0.08)',
  lg: '0 10px 15px -3px rgba(0, 0, 0, 0.04), 0 4px 6px -2px rgba(0, 0, 0, 0.05)',
  xl: '0 20px 25px -5px rgba(0, 0, 0, 0.04), 0 10px 10px -5px rgba(0, 0, 0, 0.02)',
};

// Размеры и анимации
export const sizes = {
  borderRadius: {
    xs: '2px',
    sm: '4px',
    md: '8px',
    lg: '12px',
    xl: '16px',
    xxl: '24px',
    full: '9999px',
  },
  spacing: {
    xxs: '0.25rem',    // 4px
    xs: '0.5rem',      // 8px
    sm: '0.75rem',     // 12px
    md: '1rem',        // 16px
    lg: '1.5rem',      // 24px
    xl: '2rem',        // 32px
    xxl: '3rem',       // 48px
  },
  container: {
    xs: '600px',
    sm: '900px',
    md: '1200px',
    lg: '1536px',
  },
};

// Анимации и переходы
export const animation = {
  durations: {
    short: '100ms',
    medium: '200ms',
    long: '300ms',
  },
  timingFunctions: {
    standard: 'cubic-bezier(0.4, 0, 0.2, 1)',
    accelerate: 'cubic-bezier(0.4, 0, 1, 1)',
    decelerate: 'cubic-bezier(0, 0, 0.2, 1)',
  },
};

// Объединенная тема
export const theme = {
  colors,
  typography,
  shadows,
  sizes,
  animation,
  sensorColors,
  sensorNames,
  sensorUnits,
};