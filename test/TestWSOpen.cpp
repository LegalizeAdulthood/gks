#include "TestWSOpen.h"

#include "error.h"

#include <gtest/gtest.h>

using namespace testing;

void TestWSOpen::SetUp()
{
    Test::SetUp();

    g_recordedErrors.clear();
    ASSERT_EQ(GERROR_NONE, gopengks(stderr, 0));
    ASSERT_EQ(GERROR_NONE, gopenws(wsId, connId.c_str(), wsType));
}

void TestWSOpen::TearDown()
{
    ASSERT_EQ(GERROR_NONE, gclosews(wsId));
    ASSERT_EQ(GERROR_NONE, gclosegks());

    Test::TearDown();
}
