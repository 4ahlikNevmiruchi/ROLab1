#include <mpi.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>

// Global variables for process count and rank
int ProcNum = 0; // Number of processes
int ProcRank = 0; // Rank of current process

// Function prototypes
void ProcessInitialization(double* &pMatrix, double* &pVector, double* &pResult,
                          double* &pProcRows, double* &pProcResult,
                          int &Size, int &RowNum);
void DummyDataInitialization(double* pMatrix, double* pVector, int Size);
void PrintMatrix(double* pMatrix, int RowCount, int ColCount);
void PrintVector(double* pVector, int Size);
void ProcessTermination(double* pMatrix, double* pVector, double* pResult,
                        double* pProcRows, double* pProcResult);
void DataDistribution(double* pMatrix, double* pProcRows, double* pVector,
                      int Size, int RowNum);
void TestDistribution(double* pMatrix, double* pVector, double* pProcRows,
                      int Size, int RowNum);
void ParallelResultCalculation(double* pProcRows, double* pVector,
                              double* pProcResult, int Size, int RowNum);
void TestPartialResults(double* pProcResult, int RowNum);
void ResultReplication(double* pProcResult, double* pResult, int Size, int RowNum);
void TestResult(double* pMatrix, double* pVector, double* pResult, int Size);
void SerialResultCalculation(double* pMatrix, double* pVector, double* pSerialResult, int Size);

int main(int argc, char* argv[]) {
    double* pMatrix = nullptr;      // Initial matrix (only on root)
    double* pVector = nullptr;      // Initial vector (all processes)
    double* pResult = nullptr;      // Result vector (all processes)
    double* pProcRows = nullptr;    // Matrix stripe for this process
    double* pProcResult = nullptr;  // Result block for this process
    int Size = 0;                   // Matrix/vector size
    int RowNum = 0;                 // Number of rows for this process

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (ProcRank == 0) {
        printf("Parallel matrix-vector multiplication program\n");
        printf("Number of available processes = %d\n", ProcNum);
    }
    printf("Rank of current process = %d\n", ProcRank);

    // Memory allocation and data initialization
    ProcessInitialization(pMatrix, pVector, pResult, pProcRows, pProcResult, Size, RowNum);

    // Distribute the initial data among the processes
    DataDistribution(pMatrix, pProcRows, pVector, Size, RowNum);

    // Uncomment for debugging data distribution
    // TestDistribution(pMatrix, pVector, pProcRows, Size, RowNum);

    // Parallel matrix-vector multiplication
    ParallelResultCalculation(pProcRows, pVector, pProcResult, Size, RowNum);

    // Uncomment for debugging partial results
    // TestPartialResults(pProcResult, RowNum);

    // Gather the result vector
    ResultReplication(pProcResult, pResult, Size, RowNum);

    // Print the result vector on root
    if (ProcRank == 0) {
        printf("\nResult Vector:\n");
        PrintVector(pResult, Size);
    }

    // Test correctness (compare with serial result)
    TestResult(pMatrix, pVector, pResult, Size);

    // Process termination
    ProcessTermination(pMatrix, pVector, pResult, pProcRows, pProcResult);

    MPI_Finalize();
    return 0;
}

// Function for memory allocation and data initialization
void ProcessInitialization(double* &pMatrix, double* &pVector, double* &pResult,
                          double* &pProcRows, double* &pProcResult,
                          int &Size, int &RowNum) {
    int RestRows;
    int i;
    if (ProcRank == 0) {
        do {
            printf("\nEnter size of the matrix and vector: ");
            fflush(stdout);
            int result = scanf("%d", &Size);
            if (result != 1) {
                printf("Invalid input — please enter a number.\n");
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                Size = 0;
                continue;
            }
            if (Size < ProcNum) {
                printf("Size of the objects must be greater than number of processes!\n");
            }
        } while (Size < ProcNum);
    }
    MPI_Bcast(&Size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Determine the number of matrix rows stored on each process
    RestRows = Size;
    for (i = 0; i < ProcRank; i++)
        RestRows = RestRows - RestRows / (ProcNum - i);
    RowNum = RestRows / (ProcNum - ProcRank);

    // Memory allocation
    pVector = new double[Size];
    pResult = new double[Size];
    pProcRows = new double[RowNum * Size];
    pProcResult = new double[RowNum];

    if (ProcRank == 0) {
        pMatrix = new double[Size * Size];
        DummyDataInitialization(pMatrix, pVector, Size);
    }
}

// Function for distribution of the initial data among the processes
void DataDistribution(double* pMatrix, double* pProcRows, double* pVector,
                      int Size, int RowNum) {
    int* pSendNum = new int[ProcNum];
    int* pSendInd = new int[ProcNum];
    int RestRows = Size;
    int i;

    // Broadcast the vector to all processes
    MPI_Bcast(pVector, Size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Calculate send counts and displacements for matrix rows
    for (i = 0; i < ProcNum; i++) {
        int rows = RestRows / (ProcNum - i);
        pSendNum[i] = rows * Size;
        pSendInd[i] = (i == 0) ? 0 : pSendInd[i - 1] + pSendNum[i - 1];
        RestRows -= rows;
    }

    // Scatter the matrix stripes
    MPI_Scatterv(pMatrix, pSendNum, pSendInd, MPI_DOUBLE,
                 pProcRows, pSendNum[ProcRank], MPI_DOUBLE,
                 0, MPI_COMM_WORLD);

    delete[] pSendNum;
    delete[] pSendInd;
}

// Function for memory deallocation
void ProcessTermination(double* pMatrix, double* pVector, double* pResult,
                        double* pProcRows, double* pProcResult) {
    if (ProcRank == 0)
        delete[] pMatrix;
    delete[] pVector;
    delete[] pResult;
    delete[] pProcRows;
    delete[] pProcResult;
}

// Dummy data initialization (matrix: row number, vector: 1)
void DummyDataInitialization(double* pMatrix, double* pVector, int Size) {
    for (int i = 0; i < Size; i++) {
        pVector[i] = 1;
        for (int j = 0; j < Size; j++) {
            pMatrix[i * Size + j] = i;
        }
    }
}

// Print matrix
void PrintMatrix(double* pMatrix, int RowCount, int ColCount) {
    for (int i = 0; i < RowCount; i++) {
        for (int j = 0; j < ColCount; j++) {
            printf("%7.2f ", pMatrix[i * ColCount + j]);
        }
        printf("\n");
    }
}

// Print vector
void PrintVector(double* pVector, int Size) {
    for (int i = 0; i < Size; i++) {
        printf("%7.2f ", pVector[i]);
    }
    printf("\n");
}

// Test data distribution (debugging)
void TestDistribution(double* pMatrix, double* pVector, double* pProcRows,
                      int Size, int RowNum) {
    if (ProcRank == 0) {
        printf("Initial Matrix:\n");
        PrintMatrix(pMatrix, Size, Size);
        printf("Initial Vector:\n");
        PrintVector(pVector, Size);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    for (int i = 0; i < ProcNum; i++) {
        if (ProcRank == i) {
            printf("\nProcRank = %d\n", ProcRank);
            printf("Matrix Stripe:\n");
            PrintMatrix(pProcRows, RowNum, Size);
            printf("Vector:\n");
            PrintVector(pVector, Size);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
}

// Parallel matrix-vector multiplication (each process computes its block)
void ParallelResultCalculation(double* pProcRows, double* pVector,
                              double* pProcResult, int Size, int RowNum) {
    for (int i = 0; i < RowNum; i++) {
        pProcResult[i] = 0;
        for (int j = 0; j < Size; j++) {
            pProcResult[i] += pProcRows[i * Size + j] * pVector[j];
        }
    }
}

// Test partial results (debugging)
void TestPartialResults(double* pProcResult, int RowNum) {
    for (int i = 0; i < ProcNum; i++) {
        if (ProcRank == i) {
            printf("ProcRank = %d\n", ProcRank);
            printf("Part of result vector:\n");
            PrintVector(pProcResult, RowNum);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
}

// Gather the result vector from all processes
void ResultReplication(double* pProcResult, double* pResult, int Size, int RowNum) {
    int* pReceiveNum = new int[ProcNum];
    int* pReceiveInd = new int[ProcNum];
    int RestRows = Size;
    int i;

    for (i = 0; i < ProcNum; i++) {
        int rows = RestRows / (ProcNum - i);
        pReceiveNum[i] = rows;
        pReceiveInd[i] = (i == 0) ? 0 : pReceiveInd[i - 1] + pReceiveNum[i - 1];
        RestRows -= rows;
    }

    MPI_Allgatherv(pProcResult, pReceiveNum[ProcRank], MPI_DOUBLE,
                   pResult, pReceiveNum, pReceiveInd, MPI_DOUBLE,
                   MPI_COMM_WORLD);

    delete[] pReceiveNum;
    delete[] pReceiveInd;
}

// Serial matrix-vector multiplication for result checking
void SerialResultCalculation(double* pMatrix, double* pVector, double* pSerialResult, int Size) {
    for (int i = 0; i < Size; i++) {
        pSerialResult[i] = 0;
        for (int j = 0; j < Size; j++) {
            pSerialResult[i] += pMatrix[i * Size + j] * pVector[j];
        }
    }
}

// Test the result of parallel matrix-vector multiplication
void TestResult(double* pMatrix, double* pVector, double* pResult, int Size) {
    if (ProcRank == 0) {
        double* pSerialResult = new double[Size];
        SerialResultCalculation(pMatrix, pVector, pSerialResult, Size);
        int equal = 1;
        for (int i = 0; i < Size; i++) {
            if (pResult[i] != pSerialResult[i]) {
                equal = 0;
                break;
            }
        }
        if (equal)
            printf("The results of serial and parallel algorithms are IDENTICAL.\n");
        else
            printf("The results of serial and parallel algorithms are NOT identical. Check your code.\n");
        delete[] pSerialResult;
    }
}