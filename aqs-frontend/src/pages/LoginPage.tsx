import { useState } from "react";
import { Link } from "react-router-dom";
import axios from "axios";
import { AuthLayout } from "../components/layout";

export default function LoginPage() {
  const [email, setEmail] = useState("");
  const [password, setPassword] = useState("");
  const [error, setError] = useState("");
  const [isLoading, setIsLoading] = useState(false);

  const handleLogin = async (e: React.FormEvent) => {
    e.preventDefault();
    setError("");
    setIsLoading(true);

    try {
      const params = new URLSearchParams();
      params.append("username", email);
      params.append("password", password);

      const response = await axios.post(
        `${import.meta.env.VITE_API_URL}/auth/login`,
        params,
        {
          headers: {
            "Content-Type": "application/x-www-form-urlencoded",
          },
        }
      );

      const { access_token, refresh_token } = response.data;
      localStorage.setItem("access_token", access_token);
      localStorage.setItem("refresh_token", refresh_token);
      window.location.href = "/"; // перенаправляем на дашборд
    } catch (err: any) {
      console.error(err);
      setError("Nesprávny email alebo heslo");
    } finally {
      setIsLoading(false);
    }
  };

  return (
    <AuthLayout>
      <div className="card" style={{ maxWidth: "400px", width: "100%" }}>
        <div className="text-center mb-lg" style={{ marginTop: "20px" }}>
          <h1 className="mb-xs">Prihlásenie</h1>
          <p className="text-secondary">Systém monitorovania kvality ovzdušia</p>
        </div>

        {error && (
          <div className="alert alert-error mb-md" style={{ margin: "0 20px 20px 20px" }}>
            <div className="alert-content">{error}</div>
          </div>
        )}

        <form onSubmit={handleLogin} style={{ padding: "0 20px 20px 20px" }}>
          <div className="input-container">
            <label className="input-label" htmlFor="email">Email</label>
            <input
              id="email"
              className="input"
              type="email"
              value={email}
              onChange={(e) => setEmail(e.target.value)}
              required
              placeholder="Zadajte email"
              style={{ backgroundColor: 'rgba(255, 255, 255, 0.03)' }}
            />
          </div>

          <div className="input-container">
            <label className="input-label" htmlFor="password">Heslo</label>
            <input
              id="password"
              className="input"
              type="password"
              value={password}
              onChange={(e) => setPassword(e.target.value)}
              required
              placeholder="Zadajte heslo"
              style={{ backgroundColor: 'rgba(255, 255, 255, 0.03)' }}
            />
          </div>

          <button
            type="submit"
            className="btn"
            disabled={isLoading}
            style={{ width: "100%", marginTop: "20px" }}
          >
            {isLoading ? (
              <>
                <div className="loader" style={{ borderTop: "2px solid white", width: "16px", height: "16px" }}></div>
                <span className="ml-xs">Prihlasovanie...</span>
              </>
            ) : (
              "Prihlásiť sa"
            )}
          </button>
          
          <div className="divider" style={{ margin: "20px 0" }}></div>
          
          <div className="text-center text-sm">
            Nemáte účet? <Link to="/register" style={{ color: 'var(--primary-300)' }}>Zaregistrovať sa</Link>
          </div>
        </form>
      </div>
    </AuthLayout>
  );
}