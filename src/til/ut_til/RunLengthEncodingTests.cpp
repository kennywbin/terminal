// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#include "precomp.h"

#include "til/rle.h"

using namespace WEX::Common;
using namespace WEX::Logging;
using namespace WEX::TestExecution;

class RunLengthEncodingTests
{
    TEST_CLASS(RunLengthEncodingTests);

    TEST_METHOD(ConstructDefaultLength)
    {
        til::rle<unsigned int> rle(86, 9);
        auto foo = rle.begin();
    }

    TEST_METHOD(ConstructVerySmall)
    {
        const til::rle<unsigned short, unsigned char> rle(12, 37);
    }

    TEST_METHOD(Size)
    {
        const til::rle<unsigned short> rle(19, 12);
        VERIFY_ARE_EQUAL(19, rle.size());
    }

    TEST_METHOD(AtPos)
    {
        til::rle<int> rle(10, 10);
        rle.insert(3, 0, 4);
        rle.insert(7, 4, 2);
        rle.insert(11, 6, 3);
        rle.insert(4, 9, 1);

        VERIFY_ARE_EQUAL(3, rle.at(0));
        VERIFY_ARE_EQUAL(3, rle.at(1));
        VERIFY_ARE_EQUAL(3, rle.at(2));
        VERIFY_ARE_EQUAL(3, rle.at(3));
        VERIFY_ARE_EQUAL(7, rle.at(4));
        VERIFY_ARE_EQUAL(7, rle.at(5));
        VERIFY_ARE_EQUAL(11, rle.at(6));
        VERIFY_ARE_EQUAL(11, rle.at(7));
        VERIFY_ARE_EQUAL(11, rle.at(8));
        VERIFY_ARE_EQUAL(4, rle.at(9));
    }

    TEST_METHOD(AtPosApplies)
    {
        til::rle<int> rle(10, 10);
        rle.insert(3, 0, 4);
        rle.insert(7, 4, 2);
        rle.insert(11, 6, 3);
        rle.insert(4, 9, 1);

        size_t appliesExpected = 4;
        size_t applies = 0;
        VERIFY_ARE_EQUAL(3, rle.at(0, applies));
        VERIFY_ARE_EQUAL(appliesExpected, applies);
        --appliesExpected;
        VERIFY_ARE_EQUAL(3, rle.at(1, applies));
        VERIFY_ARE_EQUAL(appliesExpected, applies);
        --appliesExpected;
        VERIFY_ARE_EQUAL(3, rle.at(2, applies));
        VERIFY_ARE_EQUAL(appliesExpected, applies);
        --appliesExpected;
        VERIFY_ARE_EQUAL(3, rle.at(3, applies));
        VERIFY_ARE_EQUAL(appliesExpected, applies);
        appliesExpected = 2;
        VERIFY_ARE_EQUAL(7, rle.at(4, applies));
        VERIFY_ARE_EQUAL(appliesExpected, applies);
        --appliesExpected;
        VERIFY_ARE_EQUAL(7, rle.at(5, applies));
        VERIFY_ARE_EQUAL(appliesExpected, applies);
        appliesExpected = 3;
        VERIFY_ARE_EQUAL(11, rle.at(6, applies));
        VERIFY_ARE_EQUAL(appliesExpected, applies);
        --appliesExpected;
        VERIFY_ARE_EQUAL(11, rle.at(7, applies));
        VERIFY_ARE_EQUAL(appliesExpected, applies);
        --appliesExpected;
        VERIFY_ARE_EQUAL(11, rle.at(8, applies));
        VERIFY_ARE_EQUAL(appliesExpected, applies);
        appliesExpected = 1;
        VERIFY_ARE_EQUAL(4, rle.at(9, applies));
        VERIFY_ARE_EQUAL(appliesExpected, applies);
    }

    TEST_METHOD(Replace)
    {
        til::rle<int> actual(20, 10);
        actual.insert(3, 0, 4);
        actual.insert(7, 4, 2);
        actual.insert(11, 6, 3);
        actual.insert(4, 9, 1);
        actual.insert(7, 10, 5);
        actual.insert(11, 15, 2);
        actual.insert(9, 17, 3);

        actual.replace(7, 49);
        actual.replace(9, 81);
        actual.replace(3, 9);

        til::rle<int> expected(20, 10);
        expected.insert(9, 0, 4);
        expected.insert(49, 4, 2);
        expected.insert(11, 6, 3);
        expected.insert(4, 9, 1);
        expected.insert(49, 10, 5);
        expected.insert(11, 15, 2);
        expected.insert(81, 17, 3);
    }

    TEST_METHOD(ResizeShrink)
    {
        til::rle<int> actual(10, 10);
        actual.insert(3, 0, 4);
        actual.insert(7, 4, 2);
        actual.insert(11, 6, 3);
        actual.insert(4, 9, 1);

        // 3 3 3 3 7 7 11 11 11 4
        // 3 for 4, 7 for 2, 11 for 3, 4 for 1.

        til::rle<int> expected(7, 10);
        expected.insert(3, 0, 4);
        expected.insert(7, 4, 2);
        expected.insert(11, 6, 1);

        // 3 3 3 3 7 7 11
        // 3 for 4, 7 for 2, 11 for 1

        actual.resize(7);

        VERIFY_ARE_EQUAL(expected, actual);
    }

    TEST_METHOD(ResizeGrow)
    {
        til::rle<int> actual(10, 10);
        actual.insert(3, 0, 4);
        actual.insert(7, 4, 2);
        actual.insert(11, 6, 3);
        actual.insert(4, 9, 1);

        // 3 3 3 3 7 7 11 11 11 4
        // 3 for 4, 7 for 2, 11 for 3, 4 for 1.

        til::rle<int> expected(13, 10);
        expected.insert(3, 0, 4);
        expected.insert(7, 4, 2);
        expected.insert(11, 6, 3);
        expected.insert(4, 9, 4);

        // 3 3 3 3 7 7 11 11 11 4 4 4 4
        // 3 for 4, 7 for 2, 11 for 3, 4 for 4.

        actual.resize(13);

        VERIFY_ARE_EQUAL(expected, actual);
    }

    TEST_METHOD(FillAll)
    {
        til::rle<int> actual(10, 10);
        actual.insert(3, 0, 4);
        actual.insert(7, 4, 2);
        actual.insert(11, 6, 3);
        actual.insert(4, 9, 1);
        actual.fill(20);

        til::rle<int> expected(10, 20);

        VERIFY_ARE_EQUAL(expected, actual);
    }

    TEST_METHOD(FillFrom)
    {
        til::rle<int> actual(10, 10);
        actual.insert(3, 0, 4);
        actual.insert(7, 4, 2);
        actual.insert(11, 6, 3);
        actual.insert(4, 9, 1);
        actual.fill(20, 2);

        til::rle<int> expected(10, 20);
        expected.insert(3, 0, 2);

        VERIFY_ARE_EQUAL(expected, actual);
    }

    TEST_METHOD(Insert)
    {
        til::rle<int> actual(10, 10);
        actual.insert(4, 9); // insert single, implicit length

        til::rle<int> expected(10, 4);
        expected.insert(10, 0, 9); // insert multiple, say length

        VERIFY_ARE_EQUAL(expected, actual);
    }

    TEST_METHOD(Equal)
    {
        til::rle<int> actual(10, 10);
        til::rle<int> expected(10, 10);

        VERIFY_ARE_EQUAL(expected, actual);
    }

    TEST_METHOD(NotEqualValue)
    {
        til::rle<int> actual(10, 9);
        til::rle<int> expected(10, 10);

        VERIFY_ARE_NOT_EQUAL(expected, actual);
    }

    TEST_METHOD(NotEqualLength)
    {
        til::rle<int> actual(5, 10);
        til::rle<int> expected(10, 10);

        VERIFY_ARE_NOT_EQUAL(expected, actual);
    }
};