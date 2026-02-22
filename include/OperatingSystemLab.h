#ifndef OPERATING_SYSTEM_LAB_H
#define OPERATING_SYSTEM_LAB_H

#include <vector>
#include <queue>
#include "../util/logger.h"

struct Process {
    int pid;
    int arrival;
    int burst;
};

struct PProcess
{
    int pid;
    int arrival;
    int burst;
    int priority;
};

struct MLQProcess
{
    int pid;
    int arrival;
    int burst;
    int queue_level; // 0=System (FCFS), 1=Interactive (RR), 2=Batch (FCFS)
};

class OperatingSystemLab {
public:
    OperatingSystemLab();
    // bool bankersAlgorithm(int P, int R,
    //                       const std::vector<int>& processes,
    //                       const std::vector<int>& available,
    //                       const std::vector<std::vector<int>>& max,
    //                       const std::vector<std::vector<int>>& allocation);
    void roundRobin(std::vector<Process> processes, int timeQuantum);
    void shortestJobFirst(std::vector<Process> processes);
    void firstComeFirstServe(std::vector<Process> processes);
    void priorityScheduling(std::vector<PProcess> processes);
    void shortestRemainingTimeFirst(std::vector<Process> processes);
    void multilevelQueue(std::vector<MLQProcess> processes, int timeQuantum);
    void multilevelFeedbackQueue(std::vector<Process> processes);
};

#endif // OPERATING_SYSTEM_LAB_H
