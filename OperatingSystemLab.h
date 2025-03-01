#ifndef OPERATING_SYSTEM_LAB_H
#define OPERATING_SYSTEM_LAB_H
#pragma once

#include <vector>
#include "util/logger.h"

class OperatingSystemLab {
public:
    OperatingSystemLab();
    bool bankersAlgorithm(int P, int R,
                          const std::vector<int>& processes,
                          const std::vector<int>& available,
                          const std::vector<std::vector<int>>& max,
                          const std::vector<std::vector<int>>& allocation);
};

#endif // OPERATING_SYSTEM_LAB_H
