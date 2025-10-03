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

|Matrix size| Serial algorithm | 2 ps Time |2 ps Speed up| 4 ps Time |4 ps Speed up| 8 ps Time |8 ps Speed up|
|:---------:|:----------------:|:---------:|:-----------:|:---------:|:-----------:|:---------:|:-----------:|
|10         |0.000001          |0.000006   |0.12         |0.000021   |0.04         |0.000049   |0.01         |
|100        |0.000038          |0.000029   |1.29         |0.000022   |1.74         |0.000096   |0.40         |
|1000       |0.003056          |0.001569   |1.95         |0.000801   |3.80         |0.000447   |6.93         |
|2000       |0.012153          |0.006087   |2.00         |0.003075   |3.96         |0.002405   |5.10         |
|3000       |0.027299          |0.013689   |1.99         |0.006862   |3.99         |0.003553   |7.79         |
|4000       |0.048488          |0.024250   |2.00         |0.012167   |3.99         |0.006605   |7.48         |
|5000       |0.075933          |0.038039   |2.00         |0.019092   |3.99         |0.009946   |7.84         |
|6000       |0.109048          |0.055100   |1.98         |0.027380   |4.02         |0.014021   |7.99         |
|7000       |0.150461          |0.074754   |2.01         |0.038746   |3.94         |0.020060   |7.57         |
|8000       |0.193885          |0.096770   |2.00         |0.050173   |3.89         |0.024963   |7.94         |
|9000       |0.245176          |0.122251   |2.01         |0.061540   |4.00         |0.031758   |7.91         |
|10000      |0.303154          |0.151670   |2.00         |0.076170   |3.99         |0.041070   |7.53         |

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