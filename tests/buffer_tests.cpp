#include "lib/CCircularBuffer.h"
#include <gtest/gtest.h>

TEST(CCircularBufferTestSuit, ConstructorTest) {
    CCircularBuffer<float> a(2);

    ASSERT_EQ(a.size(), 0);
    ASSERT_EQ(a.capacity(), 3);
}

TEST(CCircularBufferTestSuit, ConstructorTest2) {
    CCircularBuffer<std::string> b(3, "abc");
    ASSERT_EQ(b.size(), 3);
    ASSERT_EQ(b.capacity(), 4);

    std::string init_a[] = {"abc", "abc", "abc"};

    int n = 0;
    for (auto i = b.begin(); i < b.end(); ++i, ++n) {
        ASSERT_EQ(*i, init_a[n]);
    }
}

TEST(CCircularBufferTestSuit, ConstructorTest3) {
    CCircularBuffer<std::string> c({"abc", "abc", "abc"});

    std::string init_a[] = {"abc", "abc", "abc"};

    int n = 0;
    for (auto i = c.begin(); i < c.end(); ++i, ++n) {
        ASSERT_EQ(*i, init_a[n]);
    }
}

TEST(CCircularBufferTestSuit, ConstructorTest4) {
    CCircularBuffer<int> a;

    ASSERT_EQ(a.capacity(), kDefaultCapacity);
    ASSERT_EQ(a.size(), 0);
}

TEST(CCircularBuffferTestSuit, CopyConstructorTest) {
    CCircularBuffer<int> a({1, 2, 3});
    CCircularBuffer<int> b(a);

    ASSERT_TRUE(a == b);
}

TEST(CCircularBufferTestSuit, IteratorsTest) {
    CCircularBuffer<std::string> a(5, "abc");
    CCircularBuffer<int> b(3);
    b.push_front(1);
    b.push_front(4);
    b.push_front(5);
    b.push_front(3);

    std::sort(b.begin(), b.end());

    for (auto i = a.begin(); i < a.end(); ++i) {
        ASSERT_EQ(*i, "abc");
    }

    int b_values[] = {3, 4, 5};
    int n = 0;
    for (auto i = b.begin(); i < b.end(); ++i, ++n) {
        ASSERT_EQ(*i, b_values[n]);
    }

}

TEST(CircularBufferTestSuit, SortingTest) {
    CCircularBuffer<std::string> a(3);
    a.push_back("ABC");
    a.push_back("CBA");
    a.push_back("BAC");

    std::sort(a.begin(), a.end());

    std::string sorted_a[3] = {"ABC", "BAC", "CBA"};

    for (auto i = 0; i < a.size(); i++) {
        ASSERT_EQ(a[i], sorted_a[i]);
    }
}

TEST(CircularBufferTestSuit, GetOperatorsTest) {
    CCircularBuffer<std::string> a;
    a.push_back("BEG");
    a.push_back("END");

    ASSERT_EQ(*a.begin(), "BEG");

    ASSERT_EQ(a.front(), "BEG");

    ASSERT_EQ(a.back(), "END");
}

TEST(CircularBufferTestSuit, ReserveTest) {
    CCircularBuffer<std::string> a = {"I", "Walk", "this", "empty", "street", "on", "the Boulevard of Broken dreams"};

    a.reserve(12);
    ASSERT_EQ(a.size(), 7);
    ASSERT_EQ(a.capacity(), 13);
}


TEST(CircularBufferTestSuit, PushBackTest) {
    CCircularBuffer<std::string> are = {"Where", "the", "city", "sleeps", "I walk", "alone"};

    std::string init_a[] = {"Where", "the", "city", "sleeps", "I walk", "alone"};
    int n = 0;
    for (auto i = are.begin(); i < are.end(); ++i, ++n) {
        ASSERT_EQ(*i, init_a[n]);
    }
}

TEST(CircularBufferTestSuit, PushFrontTest) {
    CCircularBuffer<std::string> a = {"Walk", "this", "empty", "street", "on", "Boulevard of broken dreams"};
    CCircularBuffer<int> b = {1, 2, 1};

    a.push_front("New start!");
    b.push_front(0);

    ASSERT_EQ(a.front(), "New start!");
    ASSERT_EQ(b.front(), 0);
}

TEST(CircularBufferTestSuit, PopBackTest) {
    CCircularBuffer<std::string> a = {"Walk", "this", "empty", "street", "on", "Back_value",
                                             "This is end..."};
    CCircularBuffer<int> b = {0, 1, 2, 3, 4};

    a.pop_back();
    b.pop_back();

    ASSERT_EQ(a.back(), "Back_value");
    ASSERT_EQ(b.back(), 3);
}

TEST(CircularBufferTestSuit, PopFrontTest) {
    CCircularBuffer<std::string> a = {"12", "Front_value", "Walk", "this", "empty", "street", "on", "Back_value",
                                             "This is end..."};
    CCircularBuffer<int> b = {0, 1, 2, 3, 4};

    a.pop_front();
    b.pop_front();

    ASSERT_EQ(a.front(), "Front_value");
    ASSERT_EQ(b.front(), 1);
}

TEST(CircularBufferTestSuit, InsertTest1) {
    CCircularBufferExt<std::string> k = {"a", "b", "b", "a"};
    k.insert(k.begin(), 3, "v");
    std::string ans;
    for (auto a : k) {
        ans += a;
    }
    ASSERT_EQ(k.capacity(), 10);
    ASSERT_EQ("vvvabba", ans);
}

TEST(CircularBufferTestSuit, InsertTest2) {
    CCircularBufferExt<std::string> k = {"a", "b", "b", "a"};
    k.insert("vv", k.begin() + 2);
    std::string ans;
    for (auto a : k) {
        ans += a;
    }
    ASSERT_EQ(k.capacity(), 10);
    ASSERT_EQ("abvvba", ans);
}

TEST(CircularBufferTestSuit, InsertTest3) {
    CCircularBufferExt<std::string> k(3);
    k.push_back("a");
    k.push_back("b");
    k.push_back("b");
    k.push_back("a");
    k.insert(k.begin(), k.begin(), k.begin() + 2);
    std::string ans;
    for (auto a : k) {
        ans += a;
    }
    ASSERT_EQ(k.capacity(), 8);
    ASSERT_EQ("ababba", ans);
    
}

TEST(CircularBufferTestSuit, EraseTest) {
    CCircularBuffer<std::string> a = {"Yes", "I'm", "DELETED", "man", "of", "devotion", "end..."};

    a.erase(a.begin() + 2);
    std::string init_a[] = {"Yes", "I'm", "man", "of", "devotion", "end..."};


    int n = 0;
    for (auto i = a.begin(); i < a.end(); ++i, ++n) {
        ASSERT_EQ(*i, init_a[n]);
    }
}

TEST(CircularBufferTestSuit, AssignTest) {
    CCircularBuffer<std::string> a = {"I", "Walk", "this", "empty", "street", "on", "the Boulevard of Broken dreams"};

    a.assign({"1", "2", "3", "4"});
    std::string init_a[] = {"1", "2", "3", "4", "street", "on", "the Boulevard of Broken dreams"};


    int n = 0;
    for (auto i = a.begin(); i < a.end(); ++i, ++n) {
        ASSERT_EQ(*i, init_a[n]);
    }
}

TEST(CircularBufferTestSuit, AssignTest2) {
    CCircularBuffer<std::string> a = {"I", "Walk", "this", "empty", "street", "on", "the Boulevard of Broken dreams"};

    a.assign(4, "1");
    std::string init_a[] = {"1", "1", "1", "1", "street", "on", "the Boulevard of Broken dreams"};


    int n = 0;
    for (auto i = a.begin(); i < a.end(); ++i, ++n) {
        ASSERT_EQ(*i, init_a[n]);
    }
}

