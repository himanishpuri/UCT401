#include "../include/OperatingSystemLab.h"
#include <iostream>
#include <algorithm>

OperatingSystemLab::OperatingSystemLab() {
    LOG_INFO("Operating System Lab Initialized.");
}

bool OperatingSystemLab::bankersAlgorithm(int P, int R,
                                          const std::vector<int>& processes,
                                          const std::vector<int>& available,
                                          const std::vector<std::vector<int>>& max,
                                          const std::vector<std::vector<int>>& allocation) {

    std::vector<std::vector<int>> need(P, std::vector<int>(R));
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            need[i][j] = max[i][j] - allocation[i][j];

    std::vector<bool> finish(P, false);
    std::vector<int> safeSequence(P);
    std::vector<int> work = available;

    int count = 0;
    while (count < P) {
        bool found = false;
        for (int i = 0; i < P; i++) {
            if (!finish[i]) {
                int j;
                for (j = 0; j < R; j++)
                    if (need[i][j] > work[j])
                        break;

                if (j == R) {
                    for (int k = 0; k < R; k++)
                        work[k] += allocation[i][k];

                    safeSequence[count++] = processes[i];
                    finish[i] = true;
                    found = true;

                    LOG_INFO("Process %d has been allocated resources and finished execution.", processes[i]);
                }
            }
        }
        if (!found) {
            LOG_ERROR("System is in an unsafe state!");
            return false;
        }
    }

    LOG_INFO("System is in a safe state.");
    std::string sequence = "Safe sequence: ";
    for (int i = 0; i < P; i++)
        sequence += std::to_string(safeSequence[i]) + " ";
    LOG_INFO("%s", sequence.c_str());

    return true;
}
void OperatingSystemLab::roundRobin(std::vector<Process> processes, int timeQuantum) {
    LOG_INFO("Starting Round Robin Scheduling with Time Quantum: %d", timeQuantum);

    std::sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return (a.arrival == b.arrival) ? (a.pid < b.pid) : (a.arrival < b.arrival);
              });

    std::queue<int> readyQueue;
    std::vector<int> completionTime(processes.size(), 0);
    std::vector<int> ganttChart;
    std::vector<int> remainingBurst(processes.size());

    for (size_t i = 0; i < processes.size(); i++) {
        remainingBurst[i] = processes[i].burst;
    }

    int currentTime = 0;
    size_t completed = 0;
    size_t index = 0;

    while (completed < processes.size()) {
        while (index < processes.size() && processes[index].arrival <= currentTime) {
            readyQueue.push(index);
            index++;
        }

        if (readyQueue.empty()) {
            currentTime = processes[index].arrival;
            continue;
        }

        int pid = readyQueue.front();
        readyQueue.pop();

        if (remainingBurst[pid] > timeQuantum) {
            currentTime += timeQuantum;
            remainingBurst[pid] -= timeQuantum;
            ganttChart.push_back(pid);
        }
        else {
            currentTime += remainingBurst[pid];
            remainingBurst[pid] = 0;
            completionTime[pid] = currentTime;
            ganttChart.push_back(pid);
            completed++;
        }

        while (index < processes.size() && processes[index].arrival <= currentTime) {
            readyQueue.push(index);
            index++;
        }

        if (remainingBurst[pid] > 0) {
            readyQueue.push(pid);
        }
    }

    float totalTAT = 0, totalWT = 0;
    LOG_INFO("PID\tBT\tAT\tCT\tTAT\tWT");
    for (size_t i = 0; i < processes.size(); i++) {
        int tat = completionTime[i] - processes[i].arrival;
        int wt = tat - processes[i].burst;
        totalTAT += tat;
        totalWT += wt;
        LOG_INFO("%d\t%d\t%d\t%d\t%d\t%d", processes[i].pid + 1, processes[i].burst, processes[i].arrival, completionTime[i], tat, wt);
    }

    LOG_INFO("Average Turnaround Time: %.2f", totalTAT / processes.size());
    LOG_INFO("Average Waiting Time: %.2f", totalWT / processes.size());

    std::string gantt = "Gantt Chart: ";
    for (const int& pid : ganttChart) {
        gantt += std::to_string(pid + 1) + " ";
    }
    LOG_INFO("%s", gantt.c_str());
}


void OperatingSystemLab::shortestJobFirst(std::vector<Process> processes) {
    int n = processes.size();
    std::vector<int> completionTime(n, 0);
    std::vector<int> waitingTime(n, 0);
    std::vector<int> turnAroundTime(n, 0);
    std::vector<int> ganttChart;

    std::vector<bool> completed(n, false);
    int currentTime = 0, completedProcesses = 0;

    while (completedProcesses < n) {
        int minBurstIndex = -1;
        int minBurstTime = std::numeric_limits<int>::max();

        for (int i = 0; i < n; i++) {
            if (!completed[i] && processes[i].arrival <= currentTime) {
                if (processes[i].burst < minBurstTime) {
                    minBurstTime = processes[i].burst;
                    minBurstIndex = i;
                }
            }
        }

        if (minBurstIndex == -1) {
            currentTime++;
            continue;
        }

        int pid = minBurstIndex;
        ganttChart.push_back(processes[pid].pid);
        currentTime += processes[pid].burst;
        completionTime[pid] = currentTime;
        turnAroundTime[pid] = completionTime[pid] - processes[pid].arrival;
        waitingTime[pid] = turnAroundTime[pid] - processes[pid].burst;
        completed[pid] = true;
        completedProcesses++;
    }

    float totalWT = 0, totalTAT = 0;
    LOG_INFO("PID\tBT\tAT\tCT\tTAT\tWT");

    for (int i = 0; i < n; i++) {
        totalWT += waitingTime[i];
        totalTAT += turnAroundTime[i];

        LOG_INFO("%d\t%d\t%d\t%d\t%d\t%d",
                 processes[i].pid + 1,
                 processes[i].burst,
                 processes[i].arrival,
                 completionTime[i],
                 turnAroundTime[i],
                 waitingTime[i]);
    }

    LOG_INFO("Average Turnaround Time: %.2f", totalTAT / n);
    LOG_INFO("Average Waiting Time: %.2f", totalWT / n);

    std::string gantt = "Gantt Chart: ";
    for (const int& pid : ganttChart)
        gantt += std::to_string(pid + 1) + " ";
    LOG_INFO("%s", gantt.c_str());
}


void OperatingSystemLab::firstComeFirstServe(std::vector<Process> processes) {
    LOG_INFO("Starting First-Come, First-Serve (FCFS) scheduling...");

    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return (a.arrival == b.arrival) ? (a.pid < b.pid) : (a.arrival < b.arrival);
              });

    size_t n = processes.size();
    std::vector<int> waitingTime(n, 0);
    std::vector<int> turnaroundTime(n, 0);
    int currentTime = 0;

    LOG_INFO("Processes sorted based on arrival time.");

    for (size_t i = 0; i < n; i++) {
        if (currentTime < processes[i].arrival) {
            LOG_WARN("CPU is idle from time %d to %d", currentTime, processes[i].arrival);
            currentTime = processes[i].arrival;
        }

        waitingTime[i] = currentTime - processes[i].arrival;
        turnaroundTime[i] = waitingTime[i] + processes[i].burst;
        currentTime += processes[i].burst;

        LOG_INFO("Process %d executed -> WT: %d, TAT: %d, CT: %d",
                 processes[i].pid, waitingTime[i], turnaroundTime[i], currentTime);
    }

    float avgWT = 0, avgTAT = 0;
    for (size_t i = 0; i < n; i++) {
        avgWT += waitingTime[i];
        avgTAT += turnaroundTime[i];
    }

    avgWT /= n;
    avgTAT /= n;

    LOG_INFO("Average Waiting Time: %.2f", avgWT);
    LOG_INFO("Average Turnaround Time: %.2f", avgTAT);

    std::string ganttChart = "Gantt Chart: ";
    for (const auto& p : processes) {
        ganttChart += "| P" + std::to_string(p.pid) + " ";
    }
    ganttChart += "|";

    LOG_INFO("%s", ganttChart.c_str());
}

void OperatingSystemLab::priorityScheduling(std::vector<PProcess> processes) {
    LOG_INFO("Starting Priority Scheduling");

    size_t n = processes.size();
    size_t completed = 0;
    std::vector<int> completionTime(n, 0);
    std::vector<bool> done(n, false);
    int curTime = 0;
    std::vector<int> ganttChart;

    std::sort(processes.begin(), processes.end(), [](const PProcess& a, const PProcess& b) {
        if (a.arrival == b.arrival)
            return a.priority < b.priority; // Lower number = higher priority
        return a.arrival < b.arrival;
              });

    while (completed != n) {
        int highestPriorityIndex = -1;
        for (size_t i = 0; i < n; i++) {
            if (!done[i] && processes[i].arrival <= curTime) {
                if (highestPriorityIndex == -1 || processes[i].priority < processes[highestPriorityIndex].priority) {
                    highestPriorityIndex = i;
                }
            }
        }

        if (highestPriorityIndex == -1) {
            curTime++;
            continue;
        }

        PProcess& p = processes[highestPriorityIndex];
        ganttChart.push_back(p.pid);
        curTime += p.burst;
        completionTime[highestPriorityIndex] = curTime;
        done[highestPriorityIndex] = true;
        completed++;
    }

    LOG_INFO("PID\tBT\tAT\tCT\tTAT\tWT\tPriority");
    float avgTAT = 0, avgWT = 0;
    for (size_t i = 0; i < n; i++) {
        int tat = completionTime[i] - processes[i].arrival;
        int wt = tat - processes[i].burst;
        avgTAT += tat;
        avgWT += wt;
        LOG_INFO("%d\t%d\t%d\t%d\t%d\t%d\t%d", processes[i].pid, processes[i].burst, processes[i].arrival, completionTime[i], tat, wt, processes[i].priority);
    }
    LOG_INFO("Average Turnaround Time: %.2f", avgTAT / n);
    LOG_INFO("Average Waiting Time: %.2f", avgWT / n);

    LOG_INFO("Gantt Chart:");
    std::string gantt = "| ";
    for (int pid : ganttChart) {
        gantt += "P" + std::to_string(pid) + " | ";
    }
    LOG_INFO("%s", gantt.c_str());
}
