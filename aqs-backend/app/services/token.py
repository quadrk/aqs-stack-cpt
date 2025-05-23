from app.models.token import RefreshToken
from sqlalchemy.ext.asyncio import AsyncSession
from sqlalchemy import select, update
from datetime import datetime


async def store_refresh_token(db: AsyncSession, token: str, email: str) -> None:
    db_token = RefreshToken(token=token, user_email=email)
    db.add(db_token)
    await db.commit()


async def revoke_token(db: AsyncSession, token: str) -> None:
    await db.execute(
        update(RefreshToken).where(RefreshToken.token == token).values(revoked=True)
    )
    await db.commit()


async def is_token_revoked(db: AsyncSession, token: str) -> bool:
    result = await db.execute(select(RefreshToken).where(RefreshToken.token == token))
    db_token = result.scalars().first()
    if db_token is None or db_token.revoked:
        return True
    return False
