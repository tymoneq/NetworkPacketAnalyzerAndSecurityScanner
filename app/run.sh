#! /bin/bash

# Colors for output
GREEN='\033[0;32m'
RED='\033[0:31m'
BLUE='\033[0;34m'
YELLOW='\033[1:33m'
NC='\033[0m' #no color


# Get the directory where the script is located
APP_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
CPP_DIR="$APP_DIR/backend/cpp_packet_capture"
PYTHON_DIR="$APP_DIR/backend/python_backend"
VENV_DIR="$APP_DIR/.venv"

# Function to check if command was successful
check_status(){
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}$1 successful${NC}"
        return 0
    else
        echo -e "${RED}$1 failed${NC}"
        return 1
    fi
}

# Function to run tests

run_tests(){
    echo -e "\n${BLUE}Running C++ tests...${NC}"

    # Run the tests 
    ./tests/packetCaptureTests
    local test_result=$?
    
    if [ $test_result -eq 0 ]; then
        echo -e "${GREEN}All test passed!${NC}"
        return 0

    else
        echo -e "${RED}Tests failed!${NC}"

        # if --skip-on-fail flag is not set, exit
        if [ "$1" != "--skip-on-fail" ]; then
            echo -e "${RED}Exiting due to test failure${NC}"
            exit 1

        else
            echo -e "${YELLOW}Continuing despite test failure (--skip-on-fail flag set)${NC}"
            return 1

        fi
    
    fi
}

#Check the sudo privileges
if [ "$EUID" -ne 0 ]; then
    echo -e "${RED}Please run with sudo for packet capture capabilities${NC}"
    exit 1
fi

echo -e "${BLUE}Starting Network Packet Analyzer and Security Scanner...${NC}"

# Activate Python venv if it exists
if [ -d "$VENV_DIR" ]; then
    echo -e "${BLUE}Activating Python virtual environment...${NC}"
    source "$VENV_DIR/bin/activate"
    check_status "Venv activation" || exit 1
else
    echo -e "${YELLOW}No venv found at $VENV_DIR, using system Python${NC}"
fi


#Build C++ component
echo -e "\n${BLUE}Building C++ component...${NC}"
cd "$CPP_DIR"
mkdir -p build
rm -rf build
mkdir -p build
cd build 

# Use venv Python for CMake
VENV_PYTHON=$(which python)
PYTHON_INCLUDE=$(python -c "from sysconfig import get_paths as p; print(p()['include'])")
PYTHON_LIBRARY=$(python -c "import sysconfig; print(sysconfig.get_config_var('LIBDIR')+'/libpython'+sysconfig.get_config_var('VERSION')+'.so')")


cmake .. \
    -DPython3_EXECUTABLE="$VENV_PYTHON" \
    -DPython3_INCLUDE_DIR="$PYTHON_INCLUDE" \
    -DPython3_LIBRARY="$PYTHON_LIBRARY"

check_status "CMake configuration" || exit 1

make -j$(nproc)
check_status "C++ build" || exit 1

# Always run tests
run_tests $1

# If we get here, tests either passed or using --skip-on-fail
echo -e "\n${BLUE}Starting Python backend...${NC}"
cd "$PYTHON_DIR"

# Run Python script
python3 main.py
check_status "Python execution"

echo -e "${GREEN}All components completed${NC}"





