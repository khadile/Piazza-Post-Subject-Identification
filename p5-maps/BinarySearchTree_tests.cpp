#include "BinarySearchTree.hpp"
#include "unit_test_framework.hpp"


TEST(test_stub) {
    // Add your tests here
    ASSERT_TRUE(true);
}

TEST(DefaultCtorTest) {
    BinarySearchTree<int> tree;
    ASSERT_TRUE(tree.empty());
    ASSERT_TRUE(tree.size() == 0);
}

TEST(EmptyTest) {
    BinarySearchTree<int> tree;

    ASSERT_TRUE(tree.empty());

    tree.insert(5);
    ASSERT_FALSE(tree.empty());
    tree.insert(2);
    tree.insert(0);
    ASSERT_FALSE(tree.empty());
}

// Test case for size() function
TEST(SizeTest) {
    BinarySearchTree<int> tree;

    ASSERT_EQUAL(tree.size(), 0);

    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    ASSERT_EQUAL(tree.size(), 3);
}

// Test case for height() function
TEST(HeightTest) {
    BinarySearchTree<int> tree;

    ASSERT_EQUAL(tree.height(), 0);

    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    ASSERT_EQUAL(tree.height(), 2);
}

TEST(HeightTest2) {
    BinarySearchTree<int> tree;
    tree.insert(5);

    ASSERT_TRUE(tree.height() == 1);
}

TEST(HeightTest3) {
    BinarySearchTree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    ASSERT_TRUE(tree.height() == 2);
}

TEST(CheckSortingInvariantTest) {
    BinarySearchTree<int> tree;

    ASSERT_TRUE(tree.check_sorting_invariant());

    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    ASSERT_TRUE(tree.check_sorting_invariant());

    tree.insert(10);

    ASSERT_TRUE(tree.check_sorting_invariant());
}
TEST(InsertTest) {
    BinarySearchTree<int> tree;
    tree.insert(42);

    ASSERT_FALSE(tree.empty());
    ASSERT_TRUE(tree.size() == 1);
    ASSERT_TRUE(*tree.begin() == 42);
}

TEST(InsertTest2) {
    BinarySearchTree<int> tree;
    tree.insert(5);
    tree.insert(3);

    ASSERT_TRUE(tree.size() == 2);
    ASSERT_TRUE(tree.height() == 2);
    ASSERT_TRUE(*tree.begin() == 3);
    ASSERT_TRUE(*tree.min_element() == 3);
    ASSERT_TRUE(*tree.max_element() == 5);
}
TEST(InsertTest3) {
    BinarySearchTree<char> tree;
    tree.insert('b');
    tree.insert('a');
    tree.insert('c');

    ASSERT_TRUE(tree.size() == 3);
    ASSERT_TRUE(tree.height() == 2);
    ASSERT_TRUE(*tree.begin() == 'a');
    ASSERT_TRUE(*tree.min_element() == 'a');
    ASSERT_TRUE(*tree.max_element() == 'c');

}

TEST(InsertTest4) {
    BinarySearchTree<std::string> tree;
    tree.insert("apple");
    tree.insert("orange");
    tree.insert("banana");

    ASSERT_TRUE(tree.size() == 3);
    ASSERT_TRUE(*tree.min_element() == "apple");
    ASSERT_TRUE(*tree.max_element() == "orange");

    // Test inserting an element with a different type
    tree.insert("grape");
    ASSERT_TRUE(tree.size() == 4);
    ASSERT_TRUE(tree.check_sorting_invariant());
}

TEST(FindTest) {
    BinarySearchTree<int> tree;

    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    BinarySearchTree<int>::Iterator it = tree.find(3);
    ASSERT_EQUAL(*it, 3);

    BinarySearchTree<int>::Iterator it_not_found = tree.find(10);
    ASSERT_TRUE(it_not_found == tree.end());
}

TEST(FindTest2) {
    BinarySearchTree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    BinarySearchTree<int>::Iterator it = tree.find(3);
    ASSERT_TRUE(it != tree.end());
    ASSERT_TRUE(*it == 3);

    it = tree.find(6);
    ASSERT_TRUE(it == tree.end());
}

TEST(MinGreaterThanTest) {
    BinarySearchTree<int> tree;

    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    BinarySearchTree<int>::Iterator it = tree.min_greater_than(4);
    ASSERT_EQUAL(*it, 5);

    BinarySearchTree<int>::Iterator it_not_found = tree.min_greater_than(7);
    ASSERT_TRUE(it_not_found == tree.end());
}

TEST(MinMaxElementTest) {
    BinarySearchTree<int> tree;

    ASSERT_TRUE(tree.min_element() == tree.end());
    ASSERT_TRUE(tree.max_element() == tree.end());

    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    ASSERT_EQUAL(*tree.min_element(), 3);
    ASSERT_EQUAL(*tree.max_element(), 7);
}


TEST(InorderAndPreorderTest) {
    BinarySearchTree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    std::ostringstream oss_inorder;
    tree.traverse_inorder(oss_inorder);

    std::ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);

    ASSERT_TRUE(oss_inorder.str() == "3 5 7 ");
    ASSERT_TRUE(oss_preorder.str() == "5 3 7 ");

    BinarySearchTree<int> tree2;
    tree2.insert(5);

    std::ostringstream oss_inorder2;
    std::ostringstream oss_preorder2;

    tree2.traverse_preorder(oss_inorder2);
    tree2.traverse_inorder(oss_preorder2);

    ASSERT_TRUE(oss_inorder2.str() == "5 ");
    ASSERT_TRUE(oss_preorder2.str() == "5 ");
}

TEST(LargeTreeTest) {
    BinarySearchTree<int> tree;

    for (int i = 1; i <= 1000; ++i) {
        tree.insert(i);
    }

    ASSERT_TRUE(tree.size() == 1000);
    ASSERT_TRUE(tree.check_sorting_invariant());
    ASSERT_TRUE(*tree.max_element() == 1000);
    ASSERT_TRUE(*tree.min_element() == 1);
}

TEST(EmptyTreeTest) {
    BinarySearchTree<int> tree;

    ASSERT_TRUE(tree.size() == 0);
    ASSERT_TRUE(tree.empty());
    ASSERT_TRUE(tree.check_sorting_invariant());
    ASSERT_TRUE(tree.begin() == tree.end());
}

TEST(CopyCtorTest) {
    BinarySearchTree<int> original;
    original.insert(5);
    original.insert(3);
    original.insert(7);

    BinarySearchTree<int> copy(original);

    ASSERT_TRUE(copy.size() == 3);
    ASSERT_TRUE(copy.height() == 2);
    ASSERT_TRUE(*copy.begin() == 3);
    ASSERT_TRUE(*copy.min_element() == 3);
    ASSERT_TRUE(*copy.max_element() == 7);
    ASSERT_TRUE(copy.find(5) != copy.end());
    ASSERT_TRUE(copy.find(9) == copy.end());
}


TEST(MinGreaterThanTest2) {
    BinarySearchTree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.insert(2);
    tree.insert(4);
    tree.insert(6);
    tree.insert(8);

    BinarySearchTree<int>::Iterator it = tree.min_greater_than(4);
    ASSERT_TRUE(it != tree.end());
    ASSERT_TRUE(*it == 5);

    it = tree.min_greater_than(8);
    ASSERT_TRUE(it == tree.end());
}

TEST(MinGreaterThan3) {
    BinarySearchTree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    BinarySearchTree<int>::Iterator it = tree.min_greater_than(8);
    ASSERT_TRUE(it == tree.end());
}

TEST(DifferentTypes) {
    BinarySearchTree<std::string> tree;

    tree.insert("apple");
    tree.insert("banana");
    tree.insert("orange");

    ASSERT_TRUE(tree.size() == 3);
    ASSERT_TRUE(*tree.min_element() == "apple");
    ASSERT_TRUE(*tree.max_element() == "orange");

}

TEST(SortingInvariant) {
    BinarySearchTree<int> tree;
    tree.insert(2);
    tree.insert(0);
    *tree.begin() = 3;
    ASSERT_FALSE(tree.check_sorting_invariant());

    BinarySearchTree<int> tree2;
    tree2.insert(6);
    tree2.insert(1);
    tree2.insert(8);
    *tree2.find(8) = 3;
    ASSERT_FALSE(tree2.check_sorting_invariant());
}

TEST_MAIN()