#pragma once
#include "Process.h"
#include <vector>

class SystemMonitor {
public:
    std::vector<Process> getProcesses();
    double getTotalCpuUsage();
    double getMemoryUsagePercent();
    void killProcess(int pid);
};
