import { useState } from "react";
import axios from "axios";
import { Link, useNavigate } from "react-router-dom";
import { AuthLayout } from "../components/layout";

export default function RegisterPage() {
  const [email, setEmail] = useState("");
  const [password, setPassword] = useState("");
  const [error, setError] = useState("");

  const navigate = useNavigate();

  const handleRegister = async (e: React.FormEvent) => {
    e.preventDefault();
    try {
      await axios.post(`${import.meta.env.VITE_API_URL}/auth/register`, {
        email,
        password,
      },
      {
        headers: {
          "Content-Type": "application/json",
        },
      }
    );
      navigate("/login");
    } catch (err: any) {
      console.error("Chyba registrácie:", err.response?.data);
      const detail = err.response?.data?.detail;
      setError(
        typeof detail === "string"
          ? detail
          : Array.isArray(detail)
          ? detail.map((d) => d.msg).join("; ")
          : "Chyba pri registrácii"
      );
    }
  };

  return (
    <AuthLayout>
      <div className="auth-card">
        <div className="auth-header">
          <h1>Registrácia</h1>
        </div>

        {error && <div className="error-message">{error}</div>}

        <form onSubmit={handleRegister}>
          <div className="form-group">
            <label>Email:</label>
            <input
              type="email"
              value={email}
              onChange={(e) => setEmail(e.target.value)}
              required
              placeholder="Zadajte váš email"
            />
          </div>

          <div className="form-group">
            <label>Heslo:</label>
            <input
              type="password"
              value={password}
              onChange={(e) => setPassword(e.target.value)}
              required
              placeholder="Vytvorte silné heslo"
            />
            <div className="text-sm mt-1" style={{color: "var(--text-secondary)"}}>
              Minimálne 6 znakov
            </div>
          </div>

          <button
            type="submit"
            className="btn mt-2 mb-3"
            style={{width: "100%"}}
          >
            Zaregistrovať sa
          </button>
        </form>

        <div className="text-center mt-2">
          Už máte účet? <Link to="/login">Prihlásiť sa</Link>
        </div>
      </div>
    </AuthLayout>
  );
}
