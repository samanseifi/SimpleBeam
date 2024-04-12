#include <vector>
#include <cassert>
#include <algorithm>

struct Matrix {
    std::vector<double> data;
    int rows, cols;

    // deafault constructor
    Matrix() : rows(0), cols(0) {}

    Matrix(int m, int n) : data(m * n), rows(m), cols(n) {}

    double& operator()(int i, int j) {
        return data[i * cols + j];
    }

    void removeRow(int rowToRemove) {
        assert(rowToRemove < rows);
        data.erase(data.begin() + rowToRemove * cols, data.begin() + (rowToRemove + 1) * cols);
        --rows;
    }

    void removeCol(int colToRemove) {
        assert(colToRemove < cols);
        for (int i = rows - 1; i >= 0; --i) {
            data.erase(data.begin() + i * cols + colToRemove);
        }
        --cols;
    }

    void removeCols(std::vector<int> colsToRemove) {
        std::sort(colsToRemove.begin(), colsToRemove.end());

        for (int i = 0; i < colsToRemove.size(); i++) {
            assert(colsToRemove[i] < cols);
            removeCol(colsToRemove[i] - i);
        }        
    }

    void removeRows(std::vector<int> rowsToRemove) {
        std::sort(rowsToRemove.begin(), rowsToRemove.end());

        for (int i = 0; i < rowsToRemove.size(); i++) {
            assert(rowsToRemove[i] < rows);
            removeRow(rowsToRemove[i] - i);
        }
    }

    void transpose() {
        std::vector<double> newData(data.size());
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                newData[j * rows + i] = data[i * cols + j];
            }
        }
        std::swap(data, newData);
        std::swap(rows, cols);
    }

    void MultAB(Matrix& A, Matrix& B) {
        assert(A.cols == B.rows);
        Matrix C(A.rows, B.cols);
        for (int i = 0; i < A.rows; i++) {
            for (int j = 0; j < B.cols; j++) {
                for (int k = 0; k < A.cols; k++) {
                    C(i, j) += A(i, k) * B(k, j);
                }
            }
        }
        std::swap(data, C.data);
        std::swap(rows, C.rows);
        std::swap(cols, C.cols);
    }
};