from fastapi import APIRouter, Depends, HTTPException, status
from sqlalchemy.ext.asyncio import AsyncSession
from app.db.session import get_db
from app.schemas.device import DeviceCreate, DeviceUpdate, DeviceOut
from app.core.auth import get_current_user
from app.models.user import User
from app.services.device import (
    create_device,
    get_all_devices,
    get_device_by_id,
    update_device,
    deactivate_device,
)

router = APIRouter(prefix="/devices", tags=["Admin Devices"])


@router.post("/", response_model=DeviceOut, status_code=status.HTTP_201_CREATED)
async def add_device(
    payload: DeviceCreate,
    db: AsyncSession = Depends(get_db),
    current_user: User = Depends(get_current_user),
):
    return await create_device(db, payload, current_user.email)


@router.get("/", response_model=list[DeviceOut])
async def list_devices(db: AsyncSession = Depends(get_db)):
    return await get_all_devices(db)


@router.get("/{device_id}", response_model=DeviceOut)
async def get_device(device_id: int, db: AsyncSession = Depends(get_db)):
    device = await get_device_by_id(db, device_id)
    if not device:
        raise HTTPException(status_code=404, detail="Device not found")
    return device


@router.patch("/{device_id}", response_model=DeviceOut)
async def update_device_info(
    device_id: int,
    payload: DeviceUpdate,
    db: AsyncSession = Depends(get_db),
):
    device = await update_device(db, device_id, payload)
    if not device:
        raise HTTPException(status_code=404, detail="Device not found")
    return device


@router.delete("/{device_id}", status_code=status.HTTP_204_NO_CONTENT)
async def remove_device(device_id: int, db: AsyncSession = Depends(get_db)):
    success = await deactivate_device(db, device_id)
    if not success:
        raise HTTPException(status_code=404, detail="Device not found")
    return None
