/**
 * Общие стили для компонентов
 */

// Стили для контейнеров
export const containerStyles = {
  pageContainer: {
    maxWidth: "1000px",
    margin: "0 auto",
    padding: "1rem",
    width: "100%",
  },
};

// Стили для заголовков
export const headerStyles = {
  pageHeader: {
    display: "flex",
    justifyContent: "space-between",
    alignItems: "center",
    marginBottom: "1.5rem",
  },
  title: {
    fontSize: "1.5rem",
    margin: 0,
    fontWeight: 600,
  },
  subtitle: {
    fontSize: "1.2rem",
    margin: 0,
    fontWeight: 500,
  },
};

// Стили для карточек
export const cardStyles = {
  card: {
    padding: "1.5rem",
    borderRadius: "8px",
    boxShadow: "0 1px 3px rgba(0, 0, 0, 0.1)",
    backgroundColor: "white",
    marginBottom: "1.5rem",
  },
  chartCard: {
    padding: "1.5rem",
    borderRadius: "8px",
    boxShadow: "0 1px 3px rgba(0, 0, 0, 0.1)",
    backgroundColor: "white",
    marginBottom: "1.5rem",
  },
};

// Стили для кнопок
export const buttonStyles = {
  primary: {
    backgroundColor: "#4a7aaf",
    color: "white",
    border: "none",
    borderRadius: "6px",
    padding: "0.6rem 1.2rem",
    cursor: "pointer",
  },
  secondary: {
    backgroundColor: "#5d8a60",
    color: "white",
    border: "none",
    borderRadius: "6px",
    padding: "0.6rem 1.2rem",
    cursor: "pointer",
  },
  error: {
    backgroundColor: "#d45f5b",
    color: "white",
    border: "none",
    borderRadius: "6px",
    padding: "0.6rem 1.2rem",
    cursor: "pointer",
  },
};