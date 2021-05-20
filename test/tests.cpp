//
// Created by saman on 10/29/20.
//

#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE my_unit_tests
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "Vec2.h"
#include "ArrayT.h"
#include "MatrixT.h"


BOOST_AUTO_TEST_SUITE(my_testsuite)

    BOOST_AUTO_TEST_CASE(dot_product)
    {
        Vec2 v1(1.0, 2.0);
        Vec2 v2(-1.0, -3.0);
        BOOST_TEST ( v1.Dot(v2) == -7.0);
    }

    BOOST_AUTO_TEST_CASE(assign_to_scalar)
    {
        Vec2 v1(3, 5);
        double val = 3;
        v1 = val;
        BOOST_TEST (v1.x == 3);
        BOOST_TEST (v1.y == 3);
    }
    BOOST_AUTO_TEST_CASE(multiplying_by_scalar)
    {
        Vec2 v1(5, 4);
        double val = 3;
        v1 *= val;
        BOOST_TEST (v1.x == 15);
        BOOST_TEST (v1.y == 12);
    }
    BOOST_AUTO_TEST_CASE(multiplying_by_scalar_on_the_line)
    {
        Vec2 v1(2.0, 3.0);
        double val = 2.0;
        Vec2 v2 = v1 * val;
        BOOST_TEST (v2.x == 4.0);
        BOOST_TEST (v2.y == 6.0);
    }

    BOOST_AUTO_TEST_CASE(large_vector_summation)
    {
        dArrayT V1(5);
        dArrayT V2(5);
        dArrayT V3(5);
        for (int i = 0; i < V1.Length(); i++) {
            V1[i] = i;
            V2[i] = i-1;
            V3[i] = i+1;
        }
        V1.AddArray(V2);
        BOOST_TEST (V1[2] == 3);
        BOOST_TEST (V1[0] == -1);

        dArrayT V4(3);
        for (int j = 0; j < V4.Length(); j++) {
            V4[j] = j;
        }
        V3.AddArray(V4, 1);
        BOOST_TEST (V3[0] == 1);
        BOOST_TEST (V3[1] == 2);
        BOOST_TEST (V3[2] == 4);
    }
BOOST_AUTO_TEST_SUITE_END()

