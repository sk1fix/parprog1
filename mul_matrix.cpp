#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<stdexcept>

using namespace std;


vector<vector<int>> readFile(int size,const string &path){
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

    cout << "Результат умножения матриц:\n";
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


vector<vector<int>> multiplyMatrices(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int rowsA = A.size();
    int colsA = A[0].size();
    int colsB = B[0].size();

    vector<vector<int>> result(rowsA, vector<int>(colsB, 0));

    for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsB; ++j) {
            for (int k = 0; k < colsA; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return result;
}


int main(int argc, char* argv[]){
    int size = stoi(argv[1]);
    string A_file = argv[2];
    string B_file = argv[3];
    string C_file = argv[4];
    vector<vector<int>> A = readFile(size, A_file);
    vector<vector<int>> B = readFile(size, B_file);
    vector<vector<int>> C = multiplyMatrices(A, B);
    printAndSaveMatrix(C, C_file);
    cout << "Результат сохранён в файл: " << C_file << endl;
}