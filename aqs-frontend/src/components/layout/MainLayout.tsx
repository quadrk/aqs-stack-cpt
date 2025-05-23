import React from 'react';

interface MainLayoutProps {
  children: React.ReactNode;
}

/**
 * Компонент основного макета для страниц внутри приложения
 * Применяет стандартный темный фон и ограничивает ширину для лучшего вида
 */
export const MainLayout: React.FC<MainLayoutProps> = ({ children }) => {
  return (
    <div
      style={{
        backgroundColor: 'var(--background-default)',
        minHeight: '100vh',
        width: '100%',
        display: 'flex',
        justifyContent: 'center',
        paddingTop: '20px',
        paddingBottom: '20px'
      }}
    >
      <div
        style={{
          width: '65%',
          maxWidth: '1200px',
          minWidth: '800px',
          backgroundColor: 'var(--background-dark)',
          borderRadius: 'var(--radius-lg)',
          boxShadow: 'var(--shadow-md)',
          border: '1px solid rgba(255, 255, 255, 0.05)',
          display: 'flex',
          flexDirection: 'column',
          overflow: 'hidden'
        }}
      >
        {children}
      </div>
    </div>
  );
};