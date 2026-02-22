#include "../include/OperatingSystemLab.h"
#include <iostream>
#include <algorithm>
#include <deque>

OperatingSystemLab::OperatingSystemLab() {
    LOG_INFO("Operating System Lab Initialized.");
}

// bool OperatingSystemLab::bankersAlgorithm(int P, int R,
//                                           const std::vector<int>& processes,
//                                           const std::vector<int>& available,
//                                           const std::vector<std::vector<int>>& max,
//                                           const std::vector<std::vector<int>>& allocation) {

//     std::vector<std::vector<int>> need(P, std::vector<int>(R));
//     for (int i = 0; i < P; i++)
//         for (int j = 0; j < R; j++)
//             need[i][j] = max[i][j] - allocation[i][j];

//     std::vector<bool> finish(P, false);
//     std::vector<int> safeSequence(P);
//     std::vector<int> work = available;

//     int count = 0;
//     while (count < P) {
//         bool found = false;
//         for (int i = 0; i < P; i++) {
//             if (!finish[i]) {
//                 int j;
//                 for (j = 0; j < R; j++)
//                     if (need[i][j] > work[j])
//                         break;

//                 if (j == R) {
//                     for (int k = 0; k < R; k++)
//                         work[k] += allocation[i][k];

//                     safeSequence[count++] = processes[i];
//                     finish[i] = true;
//                     found = true;

//                     LOG_INFO("Process %d has been allocated resources and finished execution.", processes[i]);
//                 }
//             }
//         }
//         if (!found) {
//             LOG_ERROR("System is in an unsafe state!");
//             return false;
//         }
//     }

//     LOG_INFO("System is in a safe state.");
//     std::string sequence = "Safe sequence: ";
//     for (int i = 0; i < P; i++)
//         sequence += std::to_string(safeSequence[i]) + " ";
//     LOG_INFO("%s", sequence.c_str());

//     return true;
// }


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
            ganttChart.push_back(pid + 1);
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
        LOG_INFO("%d\t%d\t%d\t%d\t%d\t%d", processes[i].pid, processes[i].burst, processes[i].arrival, completionTime[i], tat, wt);
    }

    LOG_INFO("Average Turnaround Time: %.2f", totalTAT / processes.size());
    LOG_INFO("Average Waiting Time: %.2f", totalWT / processes.size());

    std::string gantt = "Gantt Chart: ";
    reverse(ganttChart.begin(), ganttChart.end());
    for (const int& pid : ganttChart) {
        gantt += std::to_string(pid) + " ";
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
    std::vector<int> completionTime(n, 0);

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
        completionTime[i] = currentTime;
    }

    float avgWT = 0, avgTAT = 0;
    LOG_INFO("PID\tWT\tTAT\tAT\tBT\tCT");
    for (size_t i = 0; i < n; i++) {
        avgWT += waitingTime[i];
        avgTAT += turnaroundTime[i];
        LOG_INFO("%d\t%d\t%d\t%d\t%d\t%d", processes[i].pid, waitingTime[i], turnaroundTime[i], processes[i].arrival, processes[i].burst, completionTime[i]);
    }

    avgWT /= n;
    avgTAT /= n;

    LOG_INFO("Average Waiting Time: %.2f", avgWT);
    LOG_INFO("Average Turnaround Time: %.2f", avgTAT);

    std::string ganttChart = "Gantt Chart: | ";
    for (const auto& p : processes) {
        ganttChart += std::to_string(p.pid) + " | ";
    }

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

void OperatingSystemLab::shortestRemainingTimeFirst(std::vector<Process> processes) {
    LOG_INFO("Starting Shortest Remaining Time First (SRTF) Scheduling");

    int n = processes.size();
    std::vector<int> remainingTime(n);
    std::transform(processes.begin(), processes.end(), remainingTime.begin(), [](const Process& pro) {
        return pro.burst;
                   });

    std::vector<int> completionTime(n, 0);
    std::vector<int> waitingTime(n, 0);
    std::vector<int> turnAroundTime(n, 0);
    std::vector<bool> completed(n, false);
    int currentTime = 0;
    int completedProcesses = 0;
    std::vector<int> ganttChart;

    while (completedProcesses < n) {
        int minTime = std::numeric_limits<int>::max();
        int minIndex = -1;

        for (int i = 0; i < n; i++) {
            if (!completed[i] && processes[i].arrival <= currentTime && remainingTime[i] < minTime) {
                minTime = remainingTime[i];
                minIndex = i;
            }
        }

        if (minIndex == -1) {
            currentTime++;
            continue;
        }

        int pid = minIndex;
        ganttChart.push_back(processes[pid].pid);
        currentTime++;
        remainingTime[pid]--;

        if (remainingTime[pid] == 0) {
            completed[pid] = true;
            completedProcesses++;
            completionTime[pid] = currentTime;
            turnAroundTime[pid] = completionTime[pid] - processes[pid].arrival;
            waitingTime[pid] = turnAroundTime[pid] - processes[pid].burst;
        }
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

    std::string gantt = "Gantt Chart: | ";
    for (const int& pid : ganttChart) {
        gantt += "P" + std::to_string(pid + 1) + " | ";
    }
    LOG_INFO("%s", gantt.c_str());
}

void OperatingSystemLab::multilevelQueue(std::vector<MLQProcess> processes, int timeQuantum) {
    LOG_INFO("Starting Multilevel Queue (MLQ) Scheduling");
    LOG_INFO("Queue 0: System (FCFS), Queue 1: Interactive (RR, quantum=%d), Queue 2: Batch (FCFS)", timeQuantum);

    int n = processes.size();

    std::sort(processes.begin(), processes.end(), [](const MLQProcess& a, const MLQProcess& b) {
        return (a.arrival == b.arrival) ? (a.pid < b.pid) : (a.arrival < b.arrival);
              });

    std::vector<int> remainingBurst(n);
    std::vector<int> completionTime(n, 0);
    std::vector<bool> completed(n, false);
    std::vector<bool> hasArrived(n, false);
    std::vector<int> ganttChart;

    for (int i = 0; i < n; i++) remainingBurst[i] = processes[i].burst;

    // Three ready queues: System (FCFS), Interactive (RR), Batch (FCFS)
    std::queue<int> q0, q1, q2;

    int currentTime = 0;
    int completedCount = 0;
    int nextToArrive = 0;

    auto enqueueArrivals = [&]() {
        while (nextToArrive < n && processes[nextToArrive].arrival <= currentTime) {
            int idx = nextToArrive++;
            hasArrived[idx] = true;
            if (processes[idx].queue_level == 0) q0.push(idx);
            else if (processes[idx].queue_level == 1) q1.push(idx);
            else q2.push(idx);
        }
    };

    while (completedCount < n) {
        enqueueArrivals();

        if (!q0.empty()) {
            // Queue 0: FCFS – run selected process to completion
            int idx = q0.front(); q0.pop();
            ganttChart.push_back(processes[idx].pid);
            currentTime += remainingBurst[idx];
            completionTime[idx] = currentTime;
            completed[idx] = true;
            completedCount++;
            enqueueArrivals();
        } else if (!q1.empty()) {
            // Queue 1: Round Robin
            int idx = q1.front(); q1.pop();
            int slice = std::min(remainingBurst[idx], timeQuantum);
            ganttChart.push_back(processes[idx].pid);
            currentTime += slice;
            remainingBurst[idx] -= slice;
            enqueueArrivals();
            if (remainingBurst[idx] == 0) {
                completionTime[idx] = currentTime;
                completed[idx] = true;
                completedCount++;
            } else {
                q1.push(idx);
            }
        } else if (!q2.empty()) {
            // Queue 2: FCFS – run selected process to completion
            int idx = q2.front(); q2.pop();
            ganttChart.push_back(processes[idx].pid);
            currentTime += remainingBurst[idx];
            completionTime[idx] = currentTime;
            completed[idx] = true;
            completedCount++;
            enqueueArrivals();
        } else {
            // CPU idle – jump to next arrival
            if (nextToArrive < n) currentTime = processes[nextToArrive].arrival;
            else break;
        }
    }

    float totalTAT = 0, totalWT = 0;
    LOG_INFO("PID\tBT\tAT\tCT\tTAT\tWT\tQueue");
    for (int i = 0; i < n; i++) {
        int tat = completionTime[i] - processes[i].arrival;
        int wt = tat - processes[i].burst;
        totalTAT += tat;
        totalWT += wt;
        LOG_INFO("%d\t%d\t%d\t%d\t%d\t%d\t%d",
                 processes[i].pid, processes[i].burst, processes[i].arrival,
                 completionTime[i], tat, wt, processes[i].queue_level);
    }
    LOG_INFO("Average Turnaround Time: %.2f", totalTAT / n);
    LOG_INFO("Average Waiting Time: %.2f", totalWT / n);

    std::string gantt = "Gantt Chart: | ";
    for (int pid : ganttChart) {
        gantt += "P" + std::to_string(pid) + " | ";
    }
    LOG_INFO("%s", gantt.c_str());
}

void OperatingSystemLab::multilevelFeedbackQueue(std::vector<Process> processes) {
    LOG_INFO("Starting Multilevel Feedback Queue (MLFQ) Scheduling");
    LOG_INFO("Q0: RR quantum=2 | Q1: RR quantum=4 | Q2: FCFS");
    LOG_INFO("Demotion on quantum exhaustion; Aging threshold: 10 time units");

    int n = processes.size();
    const int NUM_QUEUES = 3;
    const int quantums[NUM_QUEUES] = {2, 4, 0}; // 0 = FCFS (run to completion)
    const int AGING_THRESHOLD = 10;

    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return (a.arrival == b.arrival) ? (a.pid < b.pid) : (a.arrival < b.arrival);
              });

    std::vector<int> remainingBurst(n);
    std::vector<int> completionTime(n, 0);
    std::vector<bool> completed(n, false);
    std::vector<int> queueLevel(n, 0);
    std::vector<int> queueEntryTime(n, 0);
    std::vector<int> ganttChart;

    for (int i = 0; i < n; i++) remainingBurst[i] = processes[i].burst;

    std::deque<int> queues[NUM_QUEUES];

    int currentTime = 0;
    int completedCount = 0;
    int nextToArrive = 0;

    auto enqueueArrivals = [&]() {
        while (nextToArrive < n && processes[nextToArrive].arrival <= currentTime) {
            int idx = nextToArrive++;
            queueLevel[idx] = 0;
            queueEntryTime[idx] = currentTime;
            queues[0].push_back(idx);
        }
    };

    auto checkAging = [&]() {
        for (int ql = 1; ql < NUM_QUEUES; ql++) {
            std::deque<int> remaining;
            for (int idx : queues[ql]) {
                if (currentTime - queueEntryTime[idx] >= AGING_THRESHOLD) {
                    int newLevel = ql - 1;
                    LOG_INFO("Process %d promoted from Q%d to Q%d (aging)", processes[idx].pid, ql, newLevel);
                    queueLevel[idx] = newLevel;
                    queueEntryTime[idx] = currentTime;
                    queues[newLevel].push_back(idx);
                } else {
                    remaining.push_back(idx);
                }
            }
            queues[ql] = remaining;
        }
    };

    while (completedCount < n) {
        enqueueArrivals();
        checkAging();

        // Find highest-priority non-empty queue
        int selectedQueue = -1;
        for (int ql = 0; ql < NUM_QUEUES; ql++) {
            if (!queues[ql].empty()) {
                selectedQueue = ql;
                break;
            }
        }

        if (selectedQueue == -1) {
            if (nextToArrive < n) currentTime = processes[nextToArrive].arrival;
            else break;
            continue;
        }

        int idx = queues[selectedQueue].front();
        queues[selectedQueue].pop_front();

        int quantum = quantums[selectedQueue];
        int slice = (quantum == 0) ? remainingBurst[idx] : std::min(remainingBurst[idx], quantum);

        ganttChart.push_back(processes[idx].pid);
        currentTime += slice;
        remainingBurst[idx] -= slice;

        enqueueArrivals();

        if (remainingBurst[idx] == 0) {
            completionTime[idx] = currentTime;
            completed[idx] = true;
            completedCount++;
        } else {
            // Demote if the process used its full quantum (only for time-sliced queues)
            bool usedFullQuantum = (quantum > 0) && (slice == quantum);
            int newLevel = (usedFullQuantum && selectedQueue < NUM_QUEUES - 1)
                           ? selectedQueue + 1
                           : selectedQueue;
            if (newLevel != selectedQueue) {
                LOG_INFO("Process %d demoted from Q%d to Q%d", processes[idx].pid, selectedQueue, newLevel);
            }
            queueLevel[idx] = newLevel;
            queueEntryTime[idx] = currentTime;
            queues[newLevel].push_back(idx);
        }
    }

    float totalTAT = 0, totalWT = 0;
    LOG_INFO("PID\tBT\tAT\tCT\tTAT\tWT");
    for (int i = 0; i < n; i++) {
        int tat = completionTime[i] - processes[i].arrival;
        int wt = tat - processes[i].burst;
        totalTAT += tat;
        totalWT += wt;
        LOG_INFO("%d\t%d\t%d\t%d\t%d\t%d",
                 processes[i].pid, processes[i].burst, processes[i].arrival,
                 completionTime[i], tat, wt);
    }
    LOG_INFO("Average Turnaround Time: %.2f", totalTAT / n);
    LOG_INFO("Average Waiting Time: %.2f", totalWT / n);

    std::string gantt = "Gantt Chart: | ";
    for (int pid : ganttChart) {
        gantt += "P" + std::to_string(pid) + " | ";
    }
    LOG_INFO("%s", gantt.c_str());
}
