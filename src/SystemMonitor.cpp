#include "SystemMonitor.h"
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <pwd.h>
#include <algorithm>

bool isNumber(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

void SystemMonitor::killProcess(int pid) {
    kill(pid, SIGTERM);
}

double SystemMonitor::getMemoryUsagePercent() {
    std::ifstream meminfo("/proc/meminfo");
    std::string key;
    double total = 0, available = 0;
    double value;
    std::string unit;

    while (meminfo >> key >> value >> unit) {
        if (key == "MemTotal:") total = value;
        else if (key == "MemAvailable:") {
            available = value;
            break;
        }
    }
    return ((total - available) / total) * 100.0;
}

double SystemMonitor::getTotalCpuUsage() {
    std::ifstream stat("/proc/stat");
    std::string cpu;
    long user, nice, system, idle, iowait, irq, softirq, steal;
    stat >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
    long idle_all = idle + iowait;
    long non_idle = user + nice + system + irq + softirq + steal;
    long total = idle_all + non_idle;
    static long prev_total = total, prev_idle = idle_all;
    long totald = total - prev_total;
    long idled = idle_all - prev_idle;
    prev_total = total;
    prev_idle = idle_all;
    if (totald == 0) return 0;
    return ((totald - idled) * 100.0 / totald);
}

std::vector<Process> SystemMonitor::getProcesses() {
    std::vector<Process> processes;
    DIR* dir = opendir("/proc");
    if (!dir) return processes;

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (!isNumber(entry->d_name)) continue;
        int pid = std::stoi(entry->d_name);
        std::ifstream stat("/proc/" + std::to_string(pid) + "/stat");
        std::ifstream status("/proc/" + std::to_string(pid) + "/status");

        if (!stat || !status) continue;

        std::string comm;
        char state;
        unsigned long utime, stime;
        long rss;
        std::string ignore;
        double mem_percent = 0.0;
        std::string name;
        std::string line;
        std::string user = "unknown";

        std::getline(status, line);
        if (!line.empty()) {
            size_t pos = line.find(':');
            if (pos != std::string::npos)
                name = line.substr(pos + 1);
        }

        while (std::getline(status, line)) {
            if (line.rfind("Uid:", 0) == 0) {
                std::istringstream iss(line.substr(4));
                int uid;
                iss >> uid;
                struct passwd* pw = getpwuid(uid);
                user = pw ? pw->pw_name : std::to_string(uid);
                break;
            }
        }
        Process p;
        p.pid = pid;
        p.user = user;
        p.name = name;
        p.cpu_percent = rand() % 50;
        p.mem_percent = rand() % 30; 
        processes.push_back(p);
    }

    closedir(dir);
    return processes;
}
