#!/bin/bash

set -e

echo "======================================"
echo "Assignment Environment Setup (Ubuntu)"
echo "======================================"

PROJECT_ROOT=$(pwd)

echo ""
echo "[1/5] Checking system tools..."

if ! command -v cmake &> /dev/null; then
    echo "Installing CMake..."
    sudo apt-get update
    sudo apt-get install -y cmake
fi
echo "[OK] CMake: $(cmake --version | head -1)"

if ! command -v g++ &> /dev/null; then
    echo "Installing g++..."
    sudo apt-get install -y build-essential
fi
echo "[OK] g++: $(g++ --version | head -1)"

echo ""
echo "[2/5] Problem 1: Building LogFileManager..."
cd "$PROJECT_ROOT/problem1-log-file-manager"
rm -rf build
mkdir build
cd build
cmake ..
make
echo "[OK] Problem 1 build complete"

echo ""
echo "[3/5] Problem 2: Building CircularBuffer..."
cd "$PROJECT_ROOT/problem2-circular-buffer"
rm -rf build
mkdir build
cd build
cmake ..
make
echo "[OK] Problem 2 build complete"

echo ""
echo "[4/5] Problem 3: Building ParallelProcessor..."
cd "$PROJECT_ROOT/problem3-parallel-processor"
rm -rf build
mkdir build
cd build
cmake ..
make
echo "[OK] Problem 3 build complete"

echo ""
echo "[5/5] Problem 4: Setting up FastAPI environment..."
cd "$PROJECT_ROOT/problem4-restful-api-server"

if ! command -v python3 &> /dev/null; then
    echo "Installing Python3..."
    sudo apt-get install -y python3 python3-venv python3-pip
fi
echo "[OK] Python: $(python3 --version)"

if ! command -v uv &> /dev/null; then
    echo "Installing uv..."
    curl -LsSf https://astral.sh/uv/install.sh | sh
    export PATH="$HOME/.local/bin:$PATH"
fi

if command -v uv &> /dev/null; then
    echo "[OK] uv: $(uv --version)"
else
    echo "[!] uv installation complete. Please run:"
    echo "    source \$HOME/.cargo/env"
    echo "    Then run this script again."
    exit 0
fi

echo "Creating virtual environment..."
rm -rf .venv
uv venv --python 3.11

echo "Installing dependencies..."
source .venv/bin/activate
uv pip install -r requirements.txt
deactivate

echo "[OK] Problem 4 environment setup complete"

echo ""
echo "======================================"
echo "[OK] All environment setup and build complete!"
echo "======================================"
echo ""
echo "How to run:"
echo "1. cd problem1-log-file-manager/build && ./log_file_manager_test"
echo "2. cd problem2-circular-buffer/build && ./circular_buffer_test"
echo "3. cd problem3-parallel-processor/build && ./parallel_processor_test"
echo "4. cd problem4-restful-api-server"
echo "   source .venv/bin/activate"
echo "   python -m uvicorn app.main:app --reload"
echo ""
