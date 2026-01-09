from fastapi import FastAPI, Depends, HTTPException, status, Query
from fastapi.middleware.cors import CORSMiddleware
from sqlalchemy.orm import Session
from typing import List, Optional

from app.database import engine, Base, get_db
from app.models import User, Book, Loan
from app.schemas import (
    UserSignup, UserLogin, UserResponse, Token,
    BookCreate, BookResponse,
    LoanCreate, LoanResponse
)
from app.auth import hash_password, verify_password, create_access_token
from app.dependencies import get_current_user, get_current_admin_user

Base.metadata.create_all(bind=engine)

def create_default_admin():
    """Create default admin user if not exists"""
    from app.database import SessionLocal
    db = SessionLocal()
    try:
        admin = db.query(User).filter(User.username == "admin").first()
        if not admin:
            admin_user = User(
                username="admin",
                email="admin@library.com",
                hashed_password=hash_password("admin"),
                full_name="Administrator",
                is_admin=True
            )
            db.add(admin_user)
            db.commit()
    finally:
        db.close()

create_default_admin()

app = FastAPI(
    title="Library Management System",
    description="RESTful API for Online Library Management",
    version="1.0.0"
)

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

@app.get("/")
def root():
    return {"message": "Welcome to Library Management System API"}

@app.post("/auth/signup", status_code=status.HTTP_201_CREATED)
def signup(user_data: UserSignup, db: Session = Depends(get_db)):
    existing_user = db.query(User).filter(
        (User.username == user_data.username) | (User.email == user_data.email)
    ).first()

    if existing_user:
        raise HTTPException(
            status_code=status.HTTP_400_BAD_REQUEST,
            detail="Username or email already exists"
        )

    new_user = User(
        username=user_data.username,
        email=user_data.email,
        hashed_password=hash_password(user_data.password),
        full_name=user_data.full_name
    )

    db.add(new_user)
    db.commit()
    db.refresh(new_user)

    return {"message": "User created successfully", "user_id": new_user.id}

@app.post("/auth/login", response_model=Token)
def login(login_data: UserLogin, db: Session = Depends(get_db)):
    user = db.query(User).filter(User.username == login_data.username).first()

    if not user or not verify_password(login_data.password, user.hashed_password):
        raise HTTPException(
            status_code=status.HTTP_401_UNAUTHORIZED,
            detail="Incorrect username or password"
        )

    access_token = create_access_token(data={"sub": user.username, "user_id": user.id})
    return {"access_token": access_token, "token_type": "bearer"}

@app.post("/books", response_model=BookResponse, status_code=status.HTTP_201_CREATED)
def create_book(
    book_data: BookCreate,
    db: Session = Depends(get_db),
    current_user: User = Depends(get_current_admin_user)
):
    existing_book = db.query(Book).filter(Book.isbn == book_data.isbn).first()

    if existing_book:
        raise HTTPException(
            status_code=status.HTTP_400_BAD_REQUEST,
            detail="Book with this ISBN already exists"
        )

    new_book = Book(
        title=book_data.title,
        author=book_data.author,
        isbn=book_data.isbn,
        category=book_data.category,
        total_copies=book_data.total_copies,
        available_copies=book_data.total_copies
    )

    db.add(new_book)
    db.commit()
    db.refresh(new_book)

    return new_book

@app.get("/books", response_model=List[BookResponse])
def get_books(
    category: Optional[str] = Query(None),
    available: Optional[bool] = Query(None),
    db: Session = Depends(get_db)
):
    query = db.query(Book)

    if category:
        query = query.filter(Book.category == category)

    if available is not None:
        if available:
            query = query.filter(Book.available_copies > 0)
        else:
            query = query.filter(Book.available_copies == 0)

    books = query.all()
    return books

@app.post("/loans", response_model=LoanResponse, status_code=status.HTTP_201_CREATED)
def create_loan(
    loan_data: LoanCreate,
    db: Session = Depends(get_db),
    current_user: User = Depends(get_current_user)
):
    book = db.query(Book).filter(Book.id == loan_data.book_id).first()

    if not book:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Book not found"
        )

    if book.available_copies <= 0:
        raise HTTPException(
            status_code=status.HTTP_400_BAD_REQUEST,
            detail="Book not available"
        )

    new_loan = Loan(
        book_id=loan_data.book_id,
        user_id=loan_data.user_id,
        status="borrowed"
    )

    book.available_copies -= 1

    db.add(new_loan)
    db.commit()
    db.refresh(new_loan)

    return new_loan

@app.get("/users/me/loans", response_model=List[LoanResponse])
def get_my_loans(
    current_user: User = Depends(get_current_user),
    db: Session = Depends(get_db)
):
    loans = db.query(Loan).filter(Loan.user_id == current_user.id).all()
    return loans

if __name__ == "__main__":
    import uvicorn
    uvicorn.run("app.main:app", host="0.0.0.0", port=8000, reload=True)
