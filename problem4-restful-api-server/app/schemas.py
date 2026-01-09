from pydantic import BaseModel, EmailStr
from datetime import datetime
from typing import Optional

class UserSignup(BaseModel):
    username: str
    email: EmailStr
    password: str
    full_name: str

class UserLogin(BaseModel):
    username: str
    password: str

class UserResponse(BaseModel):
    id: int
    username: str
    email: str
    full_name: str
    is_admin: bool

    class Config:
        from_attributes = True

class Token(BaseModel):
    access_token: str
    token_type: str

class BookCreate(BaseModel):
    title: str
    author: str
    isbn: str
    category: str
    total_copies: int

class BookResponse(BaseModel):
    id: int
    title: str
    author: str
    isbn: str
    category: str
    total_copies: int
    available_copies: int

    class Config:
        from_attributes = True

class LoanCreate(BaseModel):
    book_id: int
    user_id: int

class LoanResponse(BaseModel):
    id: int
    book_id: int
    user_id: int
    borrowed_at: datetime
    returned_at: Optional[datetime]
    status: str

    class Config:
        from_attributes = True
