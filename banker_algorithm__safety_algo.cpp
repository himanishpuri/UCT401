#include <iostream>
#include "util/logger.h"

using namespace std;

const int P = 4; // Processes
const int R = 3; // Resources

bool isSafeState(int processes[], int available[], int max[][R], int allocation[][R]) {
    int need[P][R];

    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            need[i][j] = max[i][j] - allocation[i][j];

    bool finish[P] = { false };
    int safeSequence[P];
    int work[R];
    for (int i = 0; i < R; i++)
        work[i] = available[i];

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
    for (int i = 0; i < P; i++) {
        sequence += std::to_string(safeSequence[i]) + " ";
    }
    LOG_INFO("%s", sequence.c_str());

    return true;
}

int main() {
    Logger::getInstance().setLogLevel(INFO);

    int processes[P] = { 0, 1, 2, 3 };

    int available[R] = { 0, 0, 1 };

    int max[P][R] = {
        {1, 1, 2},
        {2, 1, 0},
        {0, 1, 1},
        {0, 3, 0}
    };

    int allocation[P][R] = {
        {1, 0, 1},
        {1, 1, 0},
        {0, 1, 0},
        {0, 1, 0}
    };

    LOG_INFO("Starting Banker's Algorithm execution...");
    isSafeState(processes, available, max, allocation);

    return 0;
}
