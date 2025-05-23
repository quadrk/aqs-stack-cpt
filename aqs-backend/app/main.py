from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from fastapi.openapi.utils import get_openapi
from app.api.v1 import api_router
from app.db.base import Base
from sqlalchemy.ext.asyncio import create_async_engine
from app.core.config import settings
from app.db import init_models
engine = create_async_engine(settings.db_url, echo=False, future=True)

origins = [
    "http://localhost:5173",  # vite dev
    "http://127.0.0.1:5173",
    "http://localhost:3000",  # vite preview / serve dist
    "http://127.0.0.1:3000",
]

app = FastAPI(
    title="Air-Quality System API",
    version="0.1.0",
)

app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# Кастомный OpenAPI с JWT авторизацией
def custom_openapi():
    if app.openapi_schema:
        return app.openapi_schema

    openapi_schema = get_openapi(
        title="Air-Quality System API",
        version="0.1.0",
        description="API for air quality monitoring system with JWT authentication",
        routes=app.routes,
    )

    openapi_schema["components"]["securitySchemes"] = {
        "BearerAuth": {
            "type": "http",
            "scheme": "bearer",
            "bearerFormat": "JWT"
        }
    }

    for path in openapi_schema["paths"].values():
        for method in path.values():
            method.setdefault("security", [{"BearerAuth": []}])

    app.openapi_schema = openapi_schema
    return app.openapi_schema

app.openapi = custom_openapi

# Роуты API
app.include_router(api_router, prefix="/api/v1")

@app.get("/", status_code=200)
async def root() -> dict[str, str]:
    return {"status": "ok"}

@app.on_event("startup")
async def on_startup():
    async with engine.begin() as conn:
        await conn.run_sync(Base.metadata.create_all)