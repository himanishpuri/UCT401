#include "include/OperatingSystemLab.h"
#include <unordered_map>
#include <unordered_set>
#include <iomanip>

int getIntInput(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;

        if (std::cin.fail() || value < 0) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Invalid input. Please enter a positive integer.\n";
        }
        else {
            return value;
        }
    }
}

void displayUsageError() {
    LOG_ERROR("Usage: <flag> <ALGORITHM>");
}

void getProcessDetails(std::vector<Process>& processes, std::vector<PProcess>& pProcesses, const std::string& algorithm) {
    int num_processes = getIntInput("Enter the number of processes: ");
    std::unordered_set<int> process_ids;

    for (int i = 0; i < num_processes; ++i) {
        std::cout << "\n--- Enter details for process " << i + 1 << " ---\n";
        int id = getIntInput("  Process ID: ");

        while (process_ids.find(id) != process_ids.end()) {
            std::cerr << "Error: Process ID " << id << " already exists. Please enter a unique PID.\n";
            id = getIntInput("  Process ID: ");
        }

        process_ids.insert(id);

        int arrival_time = getIntInput("  Arrival Time: ");
        int burst_time = getIntInput("  Burst Time: ");

        if (algorithm == "PR") {
            int priority = getIntInput("  Priority: ");
            pProcesses.push_back({ id, arrival_time, burst_time, priority });
        }
        else {
            processes.push_back({ id, arrival_time, burst_time });
        }
    }
}

void executeAlgorithm(const std::string& algorithm, OperatingSystemLab& os, std::vector<Process> processes = {}, std::vector<PProcess> pProcesses = {}) {
    std::unordered_map<std::string, int> algo_map = {
        {"RR", 1},
        {"SJF", 2},
        {"FCFS", 3},
        {"PR", 4},
        {"SRTF", 5}
    };

    switch (algo_map[algorithm]) {
    case 1:
    {
        int time_quantum = getIntInput("Enter the time quantum for Round Robin scheduling: ");
        os.roundRobin(processes, time_quantum);
    }
    break;
    case 2:
        os.shortestJobFirst(processes);
        break;
    case 3:
        os.firstComeFirstServe(processes);
        break;
    case 4:
        if (pProcesses.empty()) {
            std::cerr << "Priority Scheduling requires priority values for each process.\n";
            return;
        }
        os.priorityScheduling(pProcesses);
        break;
    case 5:
        os.shortestRemainingTimeFirst(processes);
        break;
    default:
        LOG_ERROR("Invalid algorithm: %s", algorithm.c_str());
        break;
    }
}

// void handleBankersAlgorithm(OperatingSystemLab& os) {
//     int num_processes = getIntInput("Enter the number of processes: ");
//     int num_resources = getIntInput("Enter the number of resources: ");
//     std::vector<int> available(num_resources);
//     std::cout << "Enter available resources:\n";
//     for (int i = 0; i < num_resources; ++i) {
//         available[i] = getIntInput("  Resource " + std::to_string(i + 1) + ": ");
//     }

//     std::vector<std::vector<int>> max(num_processes, std::vector<int>(num_resources));
//     std::cout << "Enter the maximum matrix (each row corresponds to a process):\n";
//     for (int i = 0; i < num_processes; ++i) {
//         std::cout << "For Process " << i + 1 << ":\n";
//         for (int j = 0; j < num_resources; ++j) {
//             max[i][j] = getIntInput("  Resource " + std::to_string(j + 1) + ": ");
//         }
//     }

//     std::vector<std::vector<int>> allocation(num_processes, std::vector<int>(num_resources));
//     std::cout << "Enter the allocation matrix (each row corresponds to a process):\n";
//     for (int i = 0; i < num_processes; ++i) {
//         std::cout << "For Process " << i + 1 << ":\n";
//         for (int j = 0; j < num_resources; ++j) {
//             allocation[i][j] = getIntInput("  Resource " + std::to_string(j + 1) + ": ");
//         }
//     }

//     os.bankersAlgorithm(num_processes, num_resources, std::vector<int>(num_processes), available, max, allocation);
// }

int main(int argc, const char** argv) {
    if (argc < 3) {
        displayUsageError();
        return 1;
    }

    std::string algorithm = argv[2];
    OperatingSystemLab os;

    if (std::string(argv[1]) == "--schedule") {
        std::vector<Process> processes;
        std::vector<PProcess> pProcesses;

        getProcessDetails(processes, pProcesses, algorithm);
        executeAlgorithm(algorithm, os, processes, pProcesses);
    }
    // else if (std::string(argv[1]) == "--bankers") {
    //     if (algorithm == "bankers-algorithm") {
    //         handleBankersAlgorithm(os);
    //     }
    //     else {
    //         LOG_ERROR("Invalid algorithm: %s", algorithm.c_str());
    //         return 1;
    //     }
    // }
    else {
        std::cerr << "Invalid flag: " << argv[1] << "\n";
        return 1;
    }

    return 0;
}
