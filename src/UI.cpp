#include "UI.h"
#include <ncurses.h>
#include <thread>
#include <chrono>
#include <algorithm>

void UI::run() {
    SystemMonitor monitor;
    initscr();
    noecho();
    curs_set(FALSE);

    while (true) {
        clear();
        auto processes = monitor.getProcesses();
        double totalCPU = monitor.getTotalCpuUsage();
        double totalMem = monitor.getMemoryUsagePercent();

        mvprintw(0, 0, "System Monitor Tool (Press Ctrl+C to quit)");
        mvprintw(1, 0, "CPU Usage: %.2f%%   Memory Usage: %.2f%%", totalCPU, totalMem);
        mvprintw(3, 0, "PID     USER        CPU%%   MEM%%   NAME");
        mvprintw(4, 0, "--------------------------------------------");

        int row = 5;
        std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b){
            return a.cpu_percent > b.cpu_percent;
        });

        for (auto &p : processes) {
            if (row > LINES - 2) break;
            mvprintw(row++, 0, "%-7d %-10s %-6.2f %-6.2f %-20s",
                     p.pid, p.user.c_str(), p.cpu_percent, p.mem_percent, p.name.c_str());
        }

        refresh();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    endwin();
}
