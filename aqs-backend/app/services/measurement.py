from sqlalchemy.ext.asyncio import AsyncSession
from sqlalchemy import insert, select
from app.models.measurement import Measurement
from app.schemas.measurement import BulkMeasurementIn, MeasurementOut
from app.models.device import Device
import uuid

async def create_bulk_measurements(
    db: AsyncSession,
    data: BulkMeasurementIn,
    device: Device,
) -> None:
    if not device.device_id:
        device.device_id = str(uuid.uuid4())
        await db.commit()
        await db.refresh(device)

    values = [
        {
            "device_id": device.device_id,
            "sensor_id": m.sensor_id,
            "ts": data.ts,
            "value": m.value
        }
        for m in data.measurements
    ]

    stmt = insert(Measurement).values(values)
    await db.execute(stmt)
    await db.commit()

async def get_all_measurements(db: AsyncSession) -> list[MeasurementOut]:
    stmt = (
        select(
            Measurement.id,
            Measurement.device_id,
            Measurement.sensor_id,
            Measurement.ts,
            Measurement.value,
            Device.location,
        )
        .join(Device, Device.device_id == Measurement.device_id)
    )
    result = await db.execute(stmt)
    rows = result.mappings().all()

    return [
        MeasurementOut(
            id=row["id"],
            device_id=row["device_id"],
            sensor_id=row["sensor_id"],
            ts=row["ts"],
            value=row["value"],
            location=row["location"],
        )
        for row in rows
    ]