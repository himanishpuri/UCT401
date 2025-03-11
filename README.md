# UCT401

Algorithms for my Operating System Course

### Currently has the following algorithms
1. FCFS
2. Non Pre-Emptive SJF
3. Round Robin
4. Safety Algorithm (Banker Algorithm)
5. Non Pre-Emptive Priority Scheduling (Lower Number - Higher Priority)
6. Shortest Remaining Time First (SRTF)

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
6. Run the executable:

   On Unix:

   ```bash
   ./operating_system_lab
   ```

   On Windows:

   ```bash
   operating_system_lab.exe
   ```

7. To clean up the build files:
   ```bash
   cmake --build . --target clean
   ```

