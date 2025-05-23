from fastapi import APIRouter, Depends, status
from sqlalchemy.ext.asyncio import AsyncSession
from app.db.session import get_db
from app.schemas.measurement import MeasurementOut
from app.services.measurement import get_all_measurements
from app.models.user import User
from app.core.auth import get_current_user

router = APIRouter(prefix="/measurements", tags=["Admin Measurements"])

@router.get("/", response_model=list[MeasurementOut])
async def list_measurements(
    db: AsyncSession = Depends(get_db),
    current_user: User = Depends(get_current_user),
) -> list[MeasurementOut]:
    return await get_all_measurements(db)
