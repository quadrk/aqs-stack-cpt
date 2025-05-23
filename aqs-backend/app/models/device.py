from sqlalchemy.orm import Mapped, mapped_column
from sqlalchemy import String, Boolean, DateTime, func
from app.db.base import Base
from datetime import datetime
from sqlalchemy import ForeignKey
from app.models.user import User

class Device(Base):
    __tablename__ = "devices"

    id: Mapped[int] = mapped_column(primary_key=True)
    name: Mapped[str] = mapped_column(String(100), unique=True, nullable=False)
    api_key: Mapped[str] = mapped_column(String(128), unique=True, index=True, nullable=False)
    location: Mapped[str] = mapped_column(String(255), nullable=True)
    description: Mapped[str] = mapped_column(String(255), nullable=True)
    is_active: Mapped[bool] = mapped_column(default=True)
    created_at: Mapped[datetime] = mapped_column(DateTime, server_default=func.now())
    device_id: Mapped[str] = mapped_column(String(64), unique=True, index=True, nullable=True)

    user_email: Mapped[str] = mapped_column(ForeignKey("users.email"))
