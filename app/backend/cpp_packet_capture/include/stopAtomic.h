#pragma once
#include <atomic>

// Declare the global stop flag (no memory allocated here)
extern std::atomic<bool> stopCapture;