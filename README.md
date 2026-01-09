# 로봇 응용 SW 개발자 과제

로봇 응용 SW 개발자 직무 면접 과제 구현입니다.

## 프로젝트 구성

- **문제 1**: LogFileManager (C++) - 스마트 포인터를 활용한 리소스 관리
- **문제 2**: CircularBuffer (C++) - 템플릿과 STL을 활용한 컨테이너 설계
- **문제 3**: ParallelProcessor (C++) - 멀티스레딩과 함수형 프로그래밍
- **문제 4**: FastAPI RESTful API (Python) - 온라인 도서관 관리 시스템

---

## 빠른 시작

### Ubuntu / Linux

```bash
# 1. 스크립트 실행 권한 부여 (최초 1회)
chmod +x setup_ubuntu.sh

# 2. 환경 설정 및 모든 프로젝트 빌드 (필수 도구 자동 설치)
./setup_ubuntu.sh

# 3. 프로젝트 실행
cd problem1-log-file-manager/build && ./log_file_manager_test
cd problem2-circular-buffer/build && ./circular_buffer_test
cd problem3-parallel-processor/build && ./parallel_processor_test

cd problem4-restful-api-server
source .venv/bin/activate
python -m uvicorn app.main:app --reload
```

### Windows

```cmd
# 1. 환경 설정 및 모든 프로젝트 빌드 (필수 도구 자동 설치)
setup_windows.bat

# 2. 프로젝트 실행
cd problem1-log-file-manager\build && log_file_manager_test.exe
cd problem2-circular-buffer\build && circular_buffer_test.exe
cd problem3-parallel-processor\build && parallel_processor_test.exe

cd problem4-restful-api-server
.venv\Scripts\activate.bat
python -m uvicorn app.main:app --reload
```

> **참고**:
> - Ubuntu: `chmod +x setup_ubuntu.sh` 명령으로 스크립트 실행 권한을 먼저 부여해야 합니다.
> - 스크립트가 CMake, 컴파일러, Python 등 필요한 모든 도구를 자동으로 확인하고 설치합니다.

---

## 시스템 요구사항

- **운영체제**: Linux (Ubuntu) 또는 Windows 10/11
- **필수 도구**: 모두 자동으로 설치됩니다
  - CMake 3.10+
  - C++ 컴파일러 (g++ 또는 MSVC)
  - Python 3.11+
  - uv 패키지 관리자

---

## 설치 가이드

### 자동 설치 (권장)

**Ubuntu:**
```bash
chmod +x setup_ubuntu.sh
./setup_ubuntu.sh
```

**Windows:**
```cmd
setup_windows.bat
```

스크립트가 다음을 자동으로 수행합니다:
1. 필수 도구 확인 및 설치 (CMake, 컴파일러, Python, uv)
2. 모든 C++ 프로젝트 빌드
3. Python 가상환경 생성 및 의존성 설치

### 수동 설치

필요한 경우 수동으로 빌드할 수 있습니다:

```bash
# C++ 프로젝트 (문제 1, 2, 3)
cd problem<N>-<project-name>
mkdir build && cd build
cmake ..
make                    # Linux
# cmake --build .       # Windows

# Python 프로젝트 (문제 4)
cd problem4-restful-api-server
uv venv --python 3.11
source .venv/bin/activate    # Linux
# .venv\Scripts\activate.bat # Windows
uv pip install -r requirements.txt
```

---

## 문제 4 API 문서 (FastAPI)

> **참고**: 서버 시작 시 관리자 계정(admin/admin)이 자동 생성됩니다.

### API 문서 확인

서버 실행 후 브라우저에서:
- **Swagger UI**: http://localhost:8000/docs
- **ReDoc**: http://localhost:8000/redoc

---

## 프로젝트 구조

```
rgt/
├── README.md                          # 본 문서
├── setup_ubuntu.sh                    # Ubuntu 자동 설정 스크립트
├── setup_windows.bat                  # Windows 자동 설정 스크립트
│
├── problem1-log-file-manager/         # 문제 1: LogFileManager
│   ├── CMakeLists.txt
│   ├── include/LogFileManager.h
│   └── src/
│       ├── LogFileManager.cpp
│       └── main.cpp
│
├── problem2-circular-buffer/          # 문제 2: CircularBuffer
│   ├── CMakeLists.txt
│   ├── include/CircularBuffer.hpp
│   └── src/main.cpp
│
├── problem3-parallel-processor/       # 문제 3: ParallelProcessor
│   ├── CMakeLists.txt
│   ├── include/ParallelProcessor.h
│   └── src/main.cpp
│
└── problem4-restful-api-server/       # 문제 4: FastAPI
    ├── pyproject.toml
    ├── requirements.txt
    ├── test_api.py
    ├── .env.example
    └── app/
        ├── main.py
        ├── database.py
        ├── models.py
        ├── schemas.py
        ├── auth.py
        └── dependencies.py
```
