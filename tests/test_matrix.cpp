#include "../src/Matrix.hpp"
#include <gtest/gtest.h>

TEST(MatrixTests, Constructor) {
    Matrix m(2, 3);
    EXPECT_EQ(m.rows, 2);
    EXPECT_EQ(m.cols, 3);
    EXPECT_EQ(m.data.size(), 6);  // 2 * 3
}

TEST(MatrixTests, AccessElements) {
    Matrix m(3, 3);
    m(1, 1) = 5.0;
    EXPECT_DOUBLE_EQ(m(1, 1), 5.0);
}

TEST(MatrixTests, RemoveRow) {
    Matrix m(3, 2);
    m(0, 0) = 1; m(0, 1) = 2;
    m(1, 0) = 3; m(1, 1) = 4;
    m(2, 0) = 5; m(2, 1) = 6;

    m.removeRow(1);

    EXPECT_EQ(m.rows, 2);
    EXPECT_DOUBLE_EQ(m(1, 0), 5);
    EXPECT_DOUBLE_EQ(m(1, 1), 6);
}

TEST(MatrixTests, RemoveCol) {
    Matrix m(2, 3);
    m(0, 0) = 1; m(0, 1) = 2; m(0, 2) = 3;
    m(1, 0) = 4; m(1, 1) = 5; m(1, 2) = 6;

    m.removeCol(1);

    EXPECT_EQ(m.cols, 2);
    EXPECT_DOUBLE_EQ(m(0, 1), 3);
    EXPECT_DOUBLE_EQ(m(1, 1), 6);
}

TEST(MatrixTests, RemoveMultipleRows) {
    Matrix m(4, 2);
    m(0, 0) = 1; m(0, 1) = 2;
    m(1, 0) = 3; m(1, 1) = 4;
    m(2, 0) = 5; m(2, 1) = 6;
    m(3, 0) = 7; m(3, 1) = 8;

    m.removeRows(std::vector<int>{0, 2});

    EXPECT_EQ(m.rows, 2);
    EXPECT_DOUBLE_EQ(m(0, 0), 3);
    EXPECT_DOUBLE_EQ(m(0, 1), 4);
}

TEST(MatrixTests, RemoveMultipleCols) {
    Matrix m(2, 4);
    m(0, 0) = 1; m(0, 1) = 2; m(0, 2) = 3; m(0, 3) = 4;
    m(1, 0) = 5; m(1, 1) = 6; m(1, 2) = 7; m(1, 3) = 8;

    m.removeCols(std::vector<int>{0, 2});

    EXPECT_EQ(m.cols, 2);
    EXPECT_DOUBLE_EQ(m(0, 0), 2);
    EXPECT_DOUBLE_EQ(m(0, 1), 4);
}

TEST(MatrixTests, Transpose) {
    Matrix m(2, 3);
    m(0, 0) = 1; m(0, 1) = 2; m(0, 2) = 3;
    m(1, 0) = 4; m(1, 1) = 5; m(1, 2) = 6;

    m.transpose();

    EXPECT_EQ(m.rows, 3);
    EXPECT_EQ(m.cols, 2);
    EXPECT_DOUBLE_EQ(m(2, 1), 6);
}

TEST(MatrixTests, MatrixMultiplication) {
    Matrix A(2, 3);
    Matrix B(3, 2);
    A(0, 0) = 1; A(0, 1) = 2; A(0, 2) = 3;
    A(1, 0) = 4; A(1, 1) = 5; A(1, 2) = 6;

    B(0, 0) = 7; B(0, 1) = 8;
    B(1, 0) = 9; B(1, 1) = 10;
    B(2, 0) = 11; B(2, 1) = 12;

    Matrix C;
    C.MultAB(A, B);

    EXPECT_EQ(C.rows, 2);
    EXPECT_EQ(C.cols, 2);
    EXPECT_DOUBLE_EQ(C(0, 0), 58);  // 1*7 + 2*9 + 3*11
    EXPECT_DOUBLE_EQ(C(0, 1), 64);  // 1*8 + 2*10 + 3*12
}

