import React from 'react';

interface AuthLayoutProps {
  children: React.ReactNode;
}

/**
 * Компонент макета для страниц аутентификации
 * Центрирует содержимое и применяет темный фон
 */
export const AuthLayout: React.FC<AuthLayoutProps> = ({ children }) => {
  return (
    <div 
      style={{ 
        backgroundColor: 'var(--background-default)',
        minHeight: '100vh',
        width: '100%',
        display: 'flex',
        justifyContent: 'center',
        alignItems: 'center',
        padding: '0 20px'
      }}
    >
      {children}
    </div>
  );
};