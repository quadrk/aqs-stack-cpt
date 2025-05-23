"""Declarative Base для всех ORM-моделей."""
from sqlalchemy.orm import DeclarativeBase

class Base(DeclarativeBase):
    """Базовый класс, от которого наследуются модели."""
    pass