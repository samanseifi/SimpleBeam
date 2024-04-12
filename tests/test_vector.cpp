#include "../src/Vector.hpp"
#include <gtest/gtest.h>

TEST(VectorTests, AccessElement) {
    Vector v(5);
    v[0] = 1;
    v[1] = 2;
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
}

TEST(VectorTests, RemoveElement) {
    Vector v(5);
    for (int i = 0; i < 5; ++i) {
        v[i] = i;
    }
    v.remove(2);
    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[2], 3);
}

TEST(VectorTests, RemoveMultipleElements) {
    Vector v(5);
    for (int i = 0; i < 5; ++i) {
        v[i] = i;
    }
    v.remove(std::vector<int>{1, 3});
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 4);
}

TEST(VectorTests, InvalidRemove) {
    Vector v(5);
    EXPECT_THROW(v.remove(5), std::out_of_range);
    EXPECT_THROW(v.remove(-1), std::out_of_range);
}

TEST(VectorTests, InvalidMultipleRemove) {
    Vector v(5);
    EXPECT_THROW(v.remove(std::vector<int>{1, 5}), std::out_of_range);
    EXPECT_THROW(v.remove(std::vector<int>{-1, 0}), std::out_of_range);
}
