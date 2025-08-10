FROM ubuntu:22.04

# Avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Set working directory
WORKDIR /app

# Install system dependencies
RUN apt-get update && apt-get install -y \
    # C++ development tools
    build-essential \
    cmake \
    g++ \
    # Packet capture libraries
    libpcap-dev \
    tcpdump \
    # Python and pip
    python3 \
    python3-pip \
    python3-dev \
    # Node.js for frontend
    nodejs \
    npm \
    # PostgreSQL client
    postgresql-client \
    # Network tools
    net-tools \
    iproute2 \
    # Additional utilities
    curl \
    wget \
    git \
    && rm -rf /var/lib/apt/lists/*

# Install Python dependencies
COPY requirements.txt* ./
RUN if [ -f requirements.txt ]; then pip3 install --no-cache-dir -r requirements.txt; fi

# Install common Python packages for ML and web development
RUN pip3 install --no-cache-dir \
    flask \
    django \
    numpy \
    pandas \
    scikit-learn \
    tensorflow \
    psycopg2-binary \
    websockets 


# Copy application code
COPY app/ ./app/
COPY docs/ ./docs/

# Build C++ packet capture component
WORKDIR /app/app/backend/cpp_packet_capture
RUN if [ -f CMakeLists.txt ]; then \
    mkdir -p build && \
    cd build && \
    cmake .. && \
    make; \
    fi

# Install frontend dependencies if package.json exists
WORKDIR /app/app/frontend/dashboard
RUN if [ -f package.json ]; then npm install; fi

# Set main working directory
WORKDIR /app

# Create necessary directories
RUN mkdir -p /app/logs /app/data /app/config

# Set environment variables
ENV PYTHONPATH=/app/app/backend/python_backend
ENV NODE_ENV=production

# Expose ports
EXPOSE 5000 8000 3000 8080

# Create startup script
RUN echo '#!/bin/bash\n\
# Start the application services\n\
echo "Starting Network Packet Analyzer and Security Scanner..."\n\
\n\
# Start Python backend (adjust as needed)\n\
if [ -f /app/app/backend/python_backend/app.py ]; then\n\
    python3 /app/app/backend/python_backend/app.py &\n\
fi\n\
\n\
# Start frontend dashboard (adjust as needed)\n\
if [ -f /app/app/frontend/dashboard/server.js ]; then\n\
    cd /app/app/frontend/dashboard && node server.js &\n\
fi\n\
\n\
# Keep container running\n\
tail -f /dev/null\n\
' > /app/start.sh && chmod +x /app/start.sh

# Set proper permissions for packet capture (requires privileged mode)
RUN groupadd pcap && usermod -a -G pcap root

CMD ["/app/start.sh"]