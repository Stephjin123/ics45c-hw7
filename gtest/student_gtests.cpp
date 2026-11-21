#include <gtest/gtest.h>

#include <sstream>
#include <string>
#include <utility>

#include "int_array.hpp"

// ---------- length ----------

TEST(StudentIntArray, DefaultIsEmpty) {
    Array a;
    EXPECT_EQ(a.length(), 0);
}

TEST(StudentIntArray, LengthMatchesConstructor) {
    Array a{5};
    EXPECT_EQ(a.length(), 5);
    Array one{1};
    EXPECT_EQ(one.length(), 1);
}

// ---------- fill ----------

TEST(StudentIntArray, FillSetsEveryElement) {
    Array a{4};
    a.fill(7);
    for (int i = 0; i < a.length(); ++i) {
        EXPECT_EQ(a[i], 7);
    }
    a.fill(-3);
    EXPECT_EQ(a[0], -3);
    EXPECT_EQ(a[3], -3);
}

TEST(StudentIntArray, FillOnEmptyDoesNothing) {
    Array a;
    EXPECT_NO_THROW(a.fill(1));
    EXPECT_EQ(a.length(), 0);
}

// ---------- operator[] valid range ----------

TEST(StudentIntArray, SubscriptBothEnds) {
    Array a{3};
    a.fill(0);
    a[0] = 10;
    a[2] = 30;
    EXPECT_EQ(a[0], 10);
    EXPECT_EQ(a[1], 0);
    EXPECT_EQ(a[2], 30);
}

TEST(StudentIntArray, ConstSubscriptReads) {
    Array a{2};
    a[0] = 1;
    a[1] = 2;
    const Array& c = a;
    EXPECT_EQ(c[0], 1);
    EXPECT_EQ(c[1], 2);
}

// ---------- operator[] out of range ----------

TEST(StudentIntArray, ThrowsJustOutside) {
    Array a{3};
    EXPECT_ANY_THROW(a[-1]);
    EXPECT_ANY_THROW(a[3]);
    EXPECT_NO_THROW(a[0]);
    EXPECT_NO_THROW(a[2]);
}

TEST(StudentIntArray, ConstThrowsJustOutside) {
    Array a{3};
    const Array& c = a;
    EXPECT_ANY_THROW(c[-1]);
    EXPECT_ANY_THROW(c[3]);
    EXPECT_NO_THROW(c[0]);
    EXPECT_NO_THROW(c[2]);
}

TEST(StudentIntArray, ExceptionIsStringWithMessage) {
    Array a{3};
    try {
        a[10];
        FAIL() << "expected an exception";
    } catch (const std::string& e) {
        EXPECT_NE(e.find("Exception operator[](10) Out Of Range"), std::string::npos);
    }
    try {
        a[-1];
        FAIL() << "expected an exception";
    } catch (const std::string& e) {
        EXPECT_NE(e.find("Exception operator[](-1) Out Of Range"), std::string::npos);
    }
}

TEST(StudentIntArray, ConstExceptionIsStringWithMessage) {
    const Array a{2};
    try {
        a[2];
        FAIL() << "expected an exception";
    } catch (const std::string& e) {
        EXPECT_NE(e.find("Exception operator[](2) Out Of Range"), std::string::npos);
    }
}

TEST(StudentIntArray, UsableAfterException) {
    Array a{2};
    a.fill(4);
    EXPECT_ANY_THROW(a[5]);
    EXPECT_EQ(a.length(), 2);
    EXPECT_EQ(a[1], 4);
}

// ---------- empty and one-element arrays ----------

TEST(StudentIntArray, EmptyArrayAlwaysThrows) {
    Array a;
    EXPECT_ANY_THROW(a[0]);
    EXPECT_ANY_THROW(a[-1]);
    Array z{0};
    EXPECT_EQ(z.length(), 0);
    EXPECT_ANY_THROW(z[0]);
}

TEST(StudentIntArray, OneElementArray) {
    Array a{1};
    a[0] = 42;
    EXPECT_EQ(a[0], 42);
    EXPECT_ANY_THROW(a[1]);
    EXPECT_ANY_THROW(a[-1]);
}

// ---------- copy is deep ----------

TEST(StudentIntArray, CopyConstructorIsDeep) {
    Array a{3};
    a.fill(1);
    Array b{a};
    EXPECT_EQ(b.length(), 3);
    EXPECT_EQ(b[0], 1);
    EXPECT_EQ(b[2], 1);
    b[0] = 99;
    b[2] = 98;
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[2], 1);
    a[1] = 50;
    EXPECT_EQ(b[1], 1);
}

TEST(StudentIntArray, CopyOfEmpty) {
    Array a;
    Array b{a};
    EXPECT_EQ(b.length(), 0);
}

TEST(StudentIntArray, CopyAssignmentIsDeep) {
    Array a{3};
    a.fill(5);
    Array b{1};
    b.fill(0);
    b = a;
    EXPECT_EQ(b.length(), 3);
    EXPECT_EQ(b[2], 5);
    b[2] = 6;
    EXPECT_EQ(a[2], 5);
    a[0] = 7;
    EXPECT_EQ(b[0], 5);
}

TEST(StudentIntArray, CopyAssignmentToSelf) {
    Array a{2};
    a[0] = 3;
    a[1] = 4;
    Array& ref = a;
    a = ref;
    EXPECT_EQ(a.length(), 2);
    EXPECT_EQ(a[0], 3);
    EXPECT_EQ(a[1], 4);
}

// ---------- move ----------

TEST(StudentIntArray, MoveConstructorTransfersContents) {
    Array a{3};
    a.fill(8);
    Array b{std::move(a)};
    EXPECT_EQ(b.length(), 3);
    EXPECT_EQ(b[0], 8);
    EXPECT_EQ(b[2], 8);
}

TEST(StudentIntArray, MoveAssignmentTransfersContents) {
    Array a{2};
    a.fill(9);
    Array b{4};
    b.fill(0);
    b = std::move(a);
    EXPECT_EQ(b.length(), 2);
    EXPECT_EQ(b[0], 9);
    EXPECT_EQ(b[1], 9);
    EXPECT_ANY_THROW(b[2]);
}

// ---------- swap ----------

TEST(StudentIntArray, SwapExchangesContents) {
    Array a{1};
    a[0] = 1;
    Array b{3};
    b.fill(2);
    using std::swap;
    swap(a, b);
    EXPECT_EQ(a.length(), 3);
    EXPECT_EQ(a[2], 2);
    EXPECT_EQ(b.length(), 1);
    EXPECT_EQ(b[0], 1);
}

// ---------- streams ----------

TEST(StudentIntArray, ReadFromStream) {
    Array a{3};
    std::istringstream in{"4 5 6"};
    in >> a;
    EXPECT_EQ(a[0], 4);
    EXPECT_EQ(a[1], 5);
    EXPECT_EQ(a[2], 6);
}
