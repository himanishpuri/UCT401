#include "OperatingSystemLab.h"

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
