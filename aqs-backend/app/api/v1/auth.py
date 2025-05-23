from fastapi import APIRouter, Depends, HTTPException, status
from sqlalchemy.ext.asyncio import AsyncSession
from fastapi.security import OAuth2PasswordRequestForm
from jose import JWTError
from pydantic import BaseModel

from app.db.session import get_db
from app.schemas.user import UserCreate, UserOut
from app.services.user import create_user, get_user_by_email
from app.services.token import (
    store_refresh_token,
    is_token_revoked,
    revoke_token
)
from app.core.security import verify_password
from app.core.auth import (
    create_access_token,
    create_refresh_token,
    decode_access_token
)

router = APIRouter(prefix="/auth", tags=["Auth"])

class TokenBody(BaseModel):
    refresh_token: str


@router.post("/register", response_model=UserOut)
async def register(user_in: UserCreate, db: AsyncSession = Depends(get_db)):
    existing = await get_user_by_email(db, user_in.email)
    if existing:
        raise HTTPException(status_code=400, detail="Email already registered")
    return await create_user(db, user_in)


@router.post("/login")
async def login(form_data: OAuth2PasswordRequestForm = Depends(), db: AsyncSession = Depends(get_db)):
    user = await get_user_by_email(db, form_data.username)
    if not user or not verify_password(form_data.password, user.hashed_password):
        raise HTTPException(status_code=401, detail="Invalid credentials")

    access_token = create_access_token({"sub": user.email})
    refresh_token = create_refresh_token({"sub": user.email})
    await store_refresh_token(db, refresh_token, user.email)

    return {
        "access_token": access_token,
        "refresh_token": refresh_token,
        "token_type": "bearer"
    }


@router.post("/refresh")
async def refresh_token(
    body: TokenBody,
    db: AsyncSession = Depends(get_db),
):
    refresh_token = body.refresh_token
    try:
        payload = decode_access_token(refresh_token)
        email = payload.get("sub")
        if not email:
            raise HTTPException(status_code=401, detail="Invalid refresh token")
    except JWTError:
        raise HTTPException(status_code=401, detail="Invalid refresh token")

    if await is_token_revoked(db, refresh_token):
        raise HTTPException(status_code=401, detail="Refresh token has been revoked")

    new_token = create_access_token({"sub": email})
    return {
        "access_token": new_token,
        "token_type": "bearer"
    }


@router.post("/logout")
async def logout(
    body: TokenBody,
    db: AsyncSession = Depends(get_db),
):
    await revoke_token(db, body.refresh_token)
    return {"detail": "logged out"}
