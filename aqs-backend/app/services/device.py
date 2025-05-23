from sqlalchemy.ext.asyncio import AsyncSession
from sqlalchemy import select, update
from app.models.device import Device
from app.schemas.device import DeviceCreate, DeviceUpdate
import secrets


async def create_device(db: AsyncSession, data: DeviceCreate, user_email: str) -> Device:
    api_key = secrets.token_hex(32)
    new_device = Device(
        **data.model_dump(),
        api_key=api_key,
        user_email=user_email
    )
    db.add(new_device)
    await db.commit()
    await db.refresh(new_device)
    return new_device


async def get_all_devices(db: AsyncSession) -> list[Device]:
    result = await db.execute(select(Device))
    return result.scalars().all()


async def get_device_by_id(db: AsyncSession, device_id: int) -> Device | None:
    result = await db.execute(select(Device).where(Device.id == device_id))
    return result.scalars().first()


async def update_device(db: AsyncSession, device_id: int, data: DeviceUpdate) -> Device | None:
    device = await get_device_by_id(db, device_id)
    if not device:
        return None
    for field, value in data.model_dump(exclude_unset=True).items():
        setattr(device, field, value)
    await db.commit()
    await db.refresh(device)
    return device


async def deactivate_device(db: AsyncSession, device_id: int) -> bool:
    result = await db.execute(update(Device).where(Device.id == device_id).values(is_active=False))
    await db.commit()
    return result.rowcount > 0
