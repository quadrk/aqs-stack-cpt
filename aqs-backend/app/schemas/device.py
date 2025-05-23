from pydantic import BaseModel, Field
from typing import Optional
from datetime import datetime


class DeviceBase(BaseModel):
    name: str = Field(..., min_length=3, max_length=100)
    location: Optional[str] = None
    description: Optional[str] = None
    is_active: Optional[bool] = True


class DeviceCreate(DeviceBase):
    pass  # генерируем api_key автоматически


class DeviceUpdate(BaseModel):
    location: Optional[str] = None
    description: Optional[str] = None
    is_active: Optional[bool] = None


class DeviceOut(DeviceBase):
    id: int
    device_id: Optional[str]
    api_key: str
    user_email: str
    created_at: datetime

    class Config:
        from_attributes = True