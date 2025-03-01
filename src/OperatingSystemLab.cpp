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
