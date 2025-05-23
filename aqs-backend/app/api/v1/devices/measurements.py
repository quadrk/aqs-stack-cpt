from fastapi import APIRouter, Depends, status
from sqlalchemy.ext.asyncio import AsyncSession
from app.db.session import get_db
from app.schemas.measurement import BulkMeasurementIn
from app.services.measurement import create_bulk_measurements
from app.core.device_auth import verify_device_key
from app.models.device import Device

router = APIRouter(prefix="/measurements", tags=["Device Measurements"])

@router.post("/bulk", status_code=status.HTTP_201_CREATED)
async def add_bulk_measurements(
    payload: BulkMeasurementIn,
    db: AsyncSession = Depends(get_db),
    device: Device = Depends(verify_device_key),
) -> dict[str, str]:
    await create_bulk_measurements(db, payload, device)
    return {"result": "created"}