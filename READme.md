# UCT401

Algorithms for my Operating System Course

### Currently has the following algorithms
1. FCFS
2. Non Pre-Emptive SJF
3. Round Robin
4. Non Pre-Emptive Priority Scheduling (Lower Number - Higher Priority)
5. Shortest Remaining Time First (SRTF)
6. Multilevel Queue (MLQ) – three fixed queues: System (FCFS), Interactive (Round Robin), Batch (FCFS)
7. Multilevel Feedback Queue (MLFQ) – adaptive scheduling with demotion on quantum exhaustion and aging-based promotion

## How to Run

To run the algorithms using CMake, follow these steps:

1. Clone the repository:
   ```bash
   git clone https://github.com/himanishpuri/UCT401.git
   ```
2. Navigate to the project directory:
   ```bash
   cd UCT401
   ```
3. Create a build directory and navigate into it:
   ```bash
   mkdir build
   cd build
   ```
4. Run CMake to configure the project:
   ```bash
   cmake ..
   ```
5. Compile the code:
   ```bash
   cmake --build .
   ```
6. Run the executable with `--schedule <ALGORITHM>`, where `<ALGORITHM>` is one of:
   `FCFS`, `SJF`, `RR`, `PR`, `SRTF`, `MLQ`, `MLFQ`

   On Unix:

   ```bash
   ./operating_system_lab --schedule MLQ
   ```

   On Windows:

   ```bash
   operating_system_lab.exe --schedule MLFQ
   ```

   **MLQ notes:** each process is assigned a queue level at input time:
   - `0` = System queue (FCFS, highest priority)
   - `1` = Interactive queue (Round Robin, user-specified quantum)
   - `2` = Batch queue (FCFS, lowest priority)

   **MLFQ notes:** all processes start in Queue 0 (RR, quantum = 2).
   CPU-intensive processes are demoted to Queue 1 (RR, quantum = 4) and then Queue 2 (FCFS).
   Processes waiting ≥ 10 time units in a lower queue are promoted (aging).

7. To clean up the build files:
   ```bash
   cmake --build . --target clean
   ```

