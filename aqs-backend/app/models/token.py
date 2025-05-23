from sqlalchemy.orm import Mapped, mapped_column
from sqlalchemy import String, DateTime, Boolean, ForeignKey
from datetime import datetime
from app.db.base import Base


class RefreshToken(Base):
    __tablename__ = "refresh_tokens"

    id: Mapped[int] = mapped_column(primary_key=True)
    token: Mapped[str] = mapped_column(String, unique=True, index=True)
    user_email: Mapped[str] = mapped_column(ForeignKey("users.email"))
    created_at: Mapped[datetime] = mapped_column(default=datetime.utcnow)
    revoked: Mapped[bool] = mapped_column(default=False)
