# 🖥️ System Monitor Tool (C++)

A lightweight C++ system monitor similar to `top`, showing live CPU and memory usage for all running processes.

---

## 🚀 Features
- Displays **real-time system stats**
- Shows **CPU%** and **Memory%** per process
- Allows **sorting** by CPU or memory usage
- Lets user **kill processes** by PID
- Auto-refresh every few seconds (real-time)

---

## 🧩 Requirements
- Linux environment (uses `/proc`)
- `g++` compiler
- `cmake` (for build)
- `ncurses` library (`sudo apt install libncurses5-dev libncursesw5-dev`)

---

## ⚙️ Build & Run
```bash
# Clone this repository
git clone https://github.com/<your-username>/system-monitor-tool.git
cd system-monitor-tool

# Create build folder
mkdir build && cd build

# Build with cmake
cmake ..
make

# Run the monitor
./system_monitor
