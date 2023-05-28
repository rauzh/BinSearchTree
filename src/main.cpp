#include <iostream>
#include <gtest/gtest.h>

#include "Tree.hpp"
#include "TreeTests.hpp"

#include <ranges>

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

    // Tree<int> tree = {4, 3, 5, -2, 7};

    // for (auto elem : tree) {
    //     cout << elem << "\n";
    // }

    // cout << "--------\n";

    // std::reverse(tree.begin(), tree.end());
    // for (auto elem : tree) {
    //     cout << elem << "\n";
    // }
}
