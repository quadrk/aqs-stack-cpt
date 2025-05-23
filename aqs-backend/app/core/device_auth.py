from fastapi import Request, HTTPException, status, Depends
from sqlalchemy.ext.asyncio import AsyncSession
from sqlalchemy import select
from app.models.device import Device
from app.db.session import get_db

async def verify_device_key(
    request: Request,
    db: AsyncSession = Depends(get_db),
) -> Device:
    key = request.headers.get("X-DEVICE-KEY")
    if not key:
        raise HTTPException(status_code=status.HTTP_403_FORBIDDEN, detail="Missing device key")

    result = await db.execute(select(Device).where(Device.api_key == key, Device.is_active == True))
    device = result.scalars().first()
    if not device:
        raise HTTPException(status_code=status.HTTP_403_FORBIDDEN, detail="Invalid or inactive device key")

    return device
