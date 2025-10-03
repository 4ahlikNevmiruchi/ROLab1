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