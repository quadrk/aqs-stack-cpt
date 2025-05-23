import React from 'react';
import { Navigate } from "react-router-dom";

interface PrivateRouteProps {
  children: React.ReactNode;
}

export default function PrivateRoute({ children }: PrivateRouteProps) {
  const hasToken = localStorage.getItem("access_token");
  return hasToken ? children : <Navigate to="/login" />;
}