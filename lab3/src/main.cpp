#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <mpi.h>

using namespace std;

vector<vector<int>> readFile(int size, const string& path) {
    ifstream file(path);
    vector<vector<int>> matrix;
    string line;

    while (getline(file, line)) {
        vector<int> row;
        istringstream iss(line);
        int num;
        while (iss >> num) {
            row.push_back(num);
        }
        matrix.push_back(row);
    }

    return matrix;
}

void printAndSaveMatrix(const vector<vector<int>>& matrix, const string& path) {
    ofstream outFile(path);

    for (const auto& row : matrix) {
        for (int num : row) {
            cout << num << " ";
            outFile << num << " ";
        }
        cout << endl;
        outFile << endl;
    }

    cout << "Результат сохранен в файл: " << path << endl;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size_proc;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size_proc);



    int N = stoi(argv[1]);
    string A_file = argv[2], B_file = argv[3], C_file = argv[4];

    vector<vector<int>> A, B;

    if (rank == 0) {
        A = readFile(N, A_file);
        B = readFile(N, B_file);
    }

    vector<int> A_flat(N * N), B_flat(N * N);
    if (rank == 0) {
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                A_flat[i * N + j] = A[i][j];

        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                B_flat[i * N + j] = B[i][j];
    }

    MPI_Bcast(B_flat.data(), N * N, MPI_INT, 0, MPI_COMM_WORLD);

    int rows_per_proc = N / size_proc;
    int extra = N % size_proc;

    int start_row = rank * rows_per_proc + min(rank, extra);
    int local_rows = rows_per_proc + (rank < extra ? 1 : 0);

    vector<int> A_local(local_rows * N);
    vector<int> C_local(local_rows * N, 0);

    if (rank == 0) {
        for (int p = 0; p < size_proc; ++p) {
            int s_row = p * rows_per_proc + min(p, extra);
            int l_rows = rows_per_proc + (p < extra ? 1 : 0);

            if (p == 0) {
                for (int i = 0; i < l_rows; ++i)
                    for (int j = 0; j < N; ++j)
                        A_local[i * N + j] = A[s_row + i][j];
            }
            else {
                MPI_Send(&A_flat[s_row * N], l_rows * N, MPI_INT, p, 0, MPI_COMM_WORLD);
            }
        }
    }
    else {
        MPI_Recv(A_local.data(), local_rows * N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    for (int i = 0; i < local_rows; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                C_local[i * N + j] += A_local[i * N + k] * B_flat[k * N + j];
            }
        }
    }

    vector<int> C_flat;
    if (rank == 0)
        C_flat.resize(N * N);

    vector<int> recvcounts(size_proc), displs(size_proc);
    for (int p = 0; p < size_proc; ++p) {
        int l_rows = rows_per_proc + (p < extra ? 1 : 0);
        recvcounts[p] = l_rows * N;
        displs[p] = (p * rows_per_proc + min(p, extra)) * N;
    }

    MPI_Gatherv(C_local.data(), local_rows * N, MPI_INT,
        C_flat.data(), recvcounts.data(), displs.data(), MPI_INT,
        0, MPI_COMM_WORLD);

    if (rank == 0) {
        vector<vector<int>> C(N, vector<int>(N));
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                C[i][j] = C_flat[i * N + j];

        printAndSaveMatrix(C, C_file);
    }

    MPI_Finalize();
    return 0;
}
