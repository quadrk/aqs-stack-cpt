"""Таблица измерений — одна строка = одно значение с устройства."""
import datetime as dt

from sqlalchemy import Index
from sqlalchemy.orm import Mapped, mapped_column

from app.db.base import Base
from sqlalchemy import DateTime

class Measurement(Base):
    __tablename__ = "measurements"

    id: Mapped[int] = mapped_column(primary_key=True)
    device_id: Mapped[str]          # UUID или серийный номер устройства
    sensor_id: Mapped[str]          # код сенсора (T, RH, PM25, CO2…)
    ts: Mapped[dt.datetime] = mapped_column(
    DateTime(timezone=True), index=True
)
    value: Mapped[float]            # всегда храним в базовых единицах

    # индекс для быстрого поиска последних значений по устройству
    __table_args__ = (Index("ix_measurements_device_ts", "device_id", "ts"),)
