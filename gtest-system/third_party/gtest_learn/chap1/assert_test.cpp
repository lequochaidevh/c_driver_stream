#include <iostream>
#include <gtest/gtest.h>

using namespace std;

TEST(TestName1, Subtest1) { ASSERT_TRUE(1 == 2); }
TEST(TestName2, Subtest2) { ASSERT_FALSE(1 == 2); }

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/**
 * BUILD:
 * g++ assert_test.cpp -o test.out -L../../googletest/build/lib -lgtest \
-lgtest_main -pthread -I../../googletest/googletest/include --std=c++17
 * RUN:
 * ./test.out
 */