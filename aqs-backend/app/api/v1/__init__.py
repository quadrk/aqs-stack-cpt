from fastapi import APIRouter, Request, Depends, HTTPException, status
from . import auth  # noqa
from .admin import measurements as admin_measurements
from .admin import devices as admin_devices
from .devices import measurements as device_measurements
import os

DOCS_ACCESS_TOKEN = os.getenv("DOCS_ACCESS_TOKEN", "changeme")

api_router = APIRouter()
api_router.include_router(auth.router)

def verify_api_key(request: Request):
    token = request.headers.get("X-API-KEY")
    if token != DOCS_ACCESS_TOKEN:
        raise HTTPException(status_code=status.HTTP_403_FORBIDDEN, detail="Forbidden")

api_router.include_router(
    admin_measurements.router,
    prefix="/admin",
)

api_router.include_router(
    admin_devices.router,
    prefix="/admin",
)

# Роуты для устройств (X-DEVICE-KEY проверяется внутри handler)
api_router.include_router(
    device_measurements.router,
    prefix="/devices"
)
