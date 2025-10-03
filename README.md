mond@mondDesktop:~/VSCode/ROLab1$ mpicxx -o Main_mpi Main.cpp
mond@mondDesktop:~/VSCode/ROLab1$ mpirun -np 8 ./Main_mpi
Parallel matrix-vector multiplication program
Number of available processes = 8
Rank of current process = 0

Enter size of the matrix and vector: Rank of current process = 4
Rank of current process = 5
Rank of current process = 7
Rank of current process = 3
Rank of current process = 6
Rank of current process = 1
Rank of current process = 2
16

Result Vector:
   0.00   16.00   32.00   48.00   64.00   80.00   96.00  112.00  128.00  144.00  160.00  176.00  192.00  208.00  224.00  240.00 
The results of serial and parallel algorithms are IDENTICAL.

THE FILLED OUT TABLE

|Matrix size| Serial algorithm | 2 ps Time |2 ps Speed up| 4 ps Time |4 ps Speed up| 8 ps Time |8 psSpeed up|
|:---------:|:----------------:|:---------:|:-----------:|:---------:|:-----------:|:---------:|:----------:|
|10         |0.000001          |0.000006   |0.12         |0.000021   |0.04         |0.000049   |0.01        |
|100        |
|1000       |
|2000       |
|3000       |
|4000       |
|5000       |
|6000       |
|7000       |
|8000       |
|9000       |
|10000      |

The calculations for this tables have been made with updated output and are done like this one by one:

NOTE that due to caching sometimes terminal prompts might be delayed and shown AFTER user inputs, say, matrix size!

mond@mondDesktop:~/VSCode/ROLab1$ mpirun -np 2 ./Main_mpi
Parallel matrix-vector multiplication program
Number of available processes = 2
Rank of current process = 0

Enter size of the matrix and vector: Rank of current process = 1
10
Size,SerialTime,ParallelTime,Speedup,ProcNum
10,0.000001,0.000006,0.12,2
The results of serial and parallel algorithms are IDENTICAL.
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