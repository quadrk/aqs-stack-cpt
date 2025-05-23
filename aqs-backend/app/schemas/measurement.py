"""Pydantic-схемы для измерений."""
from datetime import datetime
from typing import Optional, List
from pydantic import BaseModel, Field

class MeasurementEntry(BaseModel):
    sensor_id: str = Field(..., min_length=1)
    value: float

class BulkMeasurementIn(BaseModel):
    ts: datetime
    measurements: List[MeasurementEntry]

class MeasurementOut(MeasurementEntry):
    id: int
    device_id: str
    ts: datetime
    location: Optional[str]