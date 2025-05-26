#include "TestGKSOpen.h"

#include "error.h"

#include <gks/gks.h>

void TestGKSOpen::SetUp()
{
    Test::SetUp();

    g_recordedErrors.clear();
    ASSERT_EQ(GERROR_NONE, gopengks(stderr, 0));
}

void TestGKSOpen::TearDown()
{
    ASSERT_EQ(GGKOP, getGksOpState());
    ASSERT_EQ(GERROR_NONE, gclosegks());

    Test::TearDown();
}
