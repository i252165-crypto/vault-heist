#include "../src/core/IntArray.h"
#include "../src/core/NodeQueue.h"
#include "../src/core/NodeStack.h"
#include "test_harness.h"

void test_IntArray()
{
    IntArray arr;
    for (int i = 0; i < 10000; ++i)
    {
        arr.push(i);
    }
    CHECK_EQ(arr.size(), 10000);
    CHECK_EQ(arr.get(150), 150);
    CHECK_EQ(arr.get(9999), 9999);

    IntArray copy(arr);
    copy[0] = -1;
    CHECK_EQ(arr[0], 0);

    IntArray assigned;
    assigned.push(50);
    assigned = arr;
    assigned[0] = -2;
    CHECK_EQ(arr[0], 0);

    assigned = assigned;
    CHECK_EQ(assigned.size(), 10000);

    IntArray popTest;
    popTest.push(10);
    popTest.push(20);
    popTest.pop();
    CHECK_EQ(popTest.size(), 1);
    popTest.pop();
    CHECK_EQ(popTest.isEmpty(), 1);

    popTest.push(5);
    popTest.push(15);
    popTest.push(25);
    CHECK_EQ(popTest.indexOf(15), 1);
    CHECK_EQ(popTest.indexOf(99), -1);
}

void test_NodeStack()
{
    NodeStack stack;
    TreeNode *n1 = reinterpret_cast<TreeNode *>(1);
    TreeNode *n2 = reinterpret_cast<TreeNode *>(2);
    TreeNode *n3 = reinterpret_cast<TreeNode *>(3);

    stack.push(n1);
    stack.push(n2);
    stack.push(n3);

    CHECK_EQ(stack.size(), 3);
    CHECK_EQ(stack.top() == n3, 1);

    stack.pop();
    CHECK_EQ(stack.size(), 2);
    CHECK_EQ(stack.top() == n2, 1);

    stack.clear();
    CHECK_EQ(stack.isEmpty(), 1);
}

void test_NodeQueue()
{
    NodeQueue q;

    for (long i = 1; i <= 6; ++i)
    {
        q.enqueue(reinterpret_cast<TreeNode *>(i));
    }

    for (int i = 0; i < 4; ++i)
    {
        q.dequeue();
    }

    for (long i = 7; i <= 16; ++i)
    {
        q.enqueue(reinterpret_cast<TreeNode *>(i));
    }

    CHECK_EQ(q.size(), 12);
    CHECK_EQ(q.front() == reinterpret_cast<TreeNode *>(5), 1);

    q.dequeue();
    CHECK_EQ(q.front() == reinterpret_cast<TreeNode *>(6), 1);
}

void runCoreTests()
{
    test_IntArray();
    test_NodeStack();
    test_NodeQueue();
}
