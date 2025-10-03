# THE FILLED OUT TABLE

|Matrix size| Serial algorithm | 2 ps Time |2 ps Speed up| 4 ps Time |4 ps Speed up| 8 ps Time |8 ps Speed up|
|:---------:|:----------------:|:---------:|:-----------:|:---------:|:-----------:|:---------:|:-----------:|
|10         |0.000001          |0.000006   |0.12         |0.000021   |0.04         |0.000049   |0.01         |
|100        |0.000038          |0.000029   |1.29         |0.000022   |1.74         |0.000096   |0.40         |
|1 000      |0.003056          |0.001569   |1.95         |0.000801   |3.80         |0.000447   |6.93         |
|2 000      |0.012153          |0.006087   |2.00         |0.003075   |3.96         |0.002405   |5.10         |
|3 000      |0.027299          |0.013689   |1.99         |0.006862   |3.99         |0.003553   |7.79         |
|4 000      |0.048488          |0.024250   |2.00         |0.012167   |3.99         |0.006605   |7.48         |
|5 000      |0.075933          |0.038039   |2.00         |0.019092   |3.99         |0.009946   |7.84         |
|6 000      |0.109048          |0.055100   |1.98         |0.027380   |4.02         |0.014021   |7.99         |
|7 000      |0.150461          |0.074754   |2.01         |0.038746   |3.94         |0.020060   |7.57         |
|8 000      |0.193885          |0.096770   |2.00         |0.050173   |3.89         |0.024963   |7.94         |
|9 000      |0.245176          |0.122251   |2.01         |0.061540   |4.00         |0.031758   |7.91         |
|10 000     |0.303154          |0.151670   |2.00         |0.076170   |3.99         |0.041070   |7.53         |
|**20 000** |1.207712          |0.608027   |1.99         |0.305711   |3.98         |0.157440   |7.86         |
|**40 000** |4.761221          |3.067902   |1.55         |1.206030   |4.02         |0.619308   |7.95         |

I don't seem to be able to go over 40 000 matrix size due to RAM limitations. My PC has bricked on 45 000

---

# How It Works

#### 1. **MPI Initialization**

- The program starts by initializing the MPI environment and determining the total number of processes (`ProcNum`) and the rank of each process (`ProcRank`).

#### 2. **User Input and Configuration**

- The root process (rank 0) prompts the user to choose whether to run the serial (single-process) algorithm for timing and correctness checking.
- The user is then prompted to enter the size of the matrix and vector. This size is broadcast to all processes.

#### 3. **Memory Allocation and Data Initialization**

- The root process allocates and initializes the full matrix and vector. The matrix is filled such that each row contains its row index, and the vector is filled with ones.
- All processes allocate memory for their local portion (stripe) of the matrix and for their portion of the result vector.

#### 4. **Data Distribution**

- The matrix is divided into horizontal stripes, and each process receives a contiguous block of rows using `MPI_Scatterv`.
- The vector is broadcast to all processes using `MPI_Bcast`.

#### 5. **Parallel Computation**

- Each process performs matrix-vector multiplication on its assigned rows, computing its portion of the result vector independently.

#### 6. **Result Gathering**

- The partial results from all processes are gathered into the full result vector on all processes using `MPI_Allgatherv`.

#### 7. **Serial Computation (Optional)**

- If enabled, the root process also computes the result using the serial algorithm for comparison and timing.

#### 8. **Correctness Check**

- The program compares the parallel result with the serial result (if computed) to verify correctness.

#### 9. **Performance Reporting**

- The program measures and reports the execution time for both serial and parallel computations, as well as the speedup achieved by parallelization.

#### 10. **Cleanup**

- All dynamically allocated memory is freed, and the MPI environment is finalized.

---

# Terminal output on 8 processes and vector size 16

mpicxx -o Main_mpi Main.cpp
mpirun -np 8 ./Main_mpi

Parallel matrix-vector multiplication program
Number of available processes = 8
Rank of current process = 0

Enter size of the matrix and vector: 16

Rank of current process = 4
Rank of current process = 5
Rank of current process = 7
Rank of current process = 3
Rank of current process = 6
Rank of current process = 1
Rank of current process = 2

Result Vector:
   0.00   16.00   32.00   48.00   64.00   80.00   96.00  112.00  128.00  144.00  160.00  176.00  192.00  208.00  224.00  240.00 
The results of serial and parallel algorithms are IDENTICAL.

---

# Terminal output on calculations

The calculations for this tables have been made with updated output and are done like this one by one:

**NOTE** that due to caching sometimes terminal prompts might be delayed and shown AFTER user inputs, say, matrix size!

## Lower are calculations for matrix size 10 as they were made to fill out the table

### 2 processes

<pre>
mond@mondDesktop:~/VSCode/ROLab1$ mpirun -np 2 ./Main_mpi
Parallel matrix-vector multiplication program
Number of available processes = 2
Rank of current process = 0

Enter size of the matrix and vector: Rank of current process = 1
10
Size,SerialTime,ParallelTime,Speedup,ProcNum
10,0.000001,0.000006,0.12,2
The results of serial and parallel algorithms are IDENTICAL.
</pre>

### 4 processes

<pre>
mond@mondDesktop:~/VSCode/ROLab1$ mpirun -np 4 ./Main_mpi
Rank of current process = 1
Rank of current process = 2
Parallel matrix-vector multiplication program
Number of available processes = 4
Rank of current process = 0

Enter size of the matrix and vector: Rank of current process = 3
10
Size,SerialTime,ParallelTime,Speedup,ProcNum
10,0.000001,0.000021,0.04,4
The results of serial and parallel algorithms are IDENTICAL.
</pre>

### 8 processes

<pre>
mond@mondDesktop:~/VSCode/ROLab1$ mpirun -np 8 ./Main_mpi
Rank of current process = 4
Parallel matrix-vector multiplication program
Number of available processes = 8
Rank of current process = 0

Enter size of the matrix and vector: Rank of current process = 6
Rank of current process = 3
Rank of current process = 1
Rank of current process = 7
Rank of current process = 2
Rank of current process = 5
10
Size,SerialTime,ParallelTime,Speedup,ProcNum
10,0.000001,0.000049,0.01,8
The results of serial and parallel algorithms are IDENTICAL.
</pre>

# Conclusions

***We could clearly observe how:***
- *Speed up* factor grows to be near identical with **amount** of processes with **increase** of *matrix size**.
- The more **processes** there is the more **Matrix size** it takes for statement one to become true.
- The main **limitation** to how big of a matrix it can handle is **RAM** size.
