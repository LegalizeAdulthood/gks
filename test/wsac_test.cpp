#include "error.h"

#include <gks/gks.h>
#include <gks/gkserror.h>

#include <gtest/gtest.h>

using namespace testing;

namespace
{

class TestWSActive : public Test
{
public:
    ~TestWSActive() override = default;

protected:
    void SetUp() override;
    void TearDown() override;

    Gint wsId{};
};

void TestWSActive::SetUp()
{
    Test::SetUp();

    g_recordedErrors.clear();
    ASSERT_EQ(GERROR_NONE, gopengks(stderr, 0));
    const Gchar *connId{"tek4105"};
    const Gint wsType{GWSTYPE_TEK4105};
    ASSERT_EQ(GERROR_NONE, gopenws(wsId, connId, wsType));
    ASSERT_EQ(GERROR_NONE, gactivatews(wsId));
    ASSERT_EQ(GWSAC, getGksOpState());
}

void TestWSActive::TearDown()
{
    ASSERT_EQ(GWSAC, getGksOpState());
    ASSERT_EQ(GERROR_NONE, gdeactivatews(wsId));
    ASSERT_EQ(GERROR_NONE, gclosews(wsId));
    ASSERT_EQ(GERROR_NONE, gclosegks());

    Test::TearDown();
}

} // namespace

TEST_F(TestWSActive, openGKS)
{
    const Gint status = gopengks(stderr, 0);

    expectError(status, GERROR_NOT_STATE_GKCL, GFN_OPEN_GKS);
}

TEST_F(TestWSActive, closeGKS)
{
    const Gint status = gclosegks();

    expectError(status, GERROR_NOT_STATE_GKOP, GFN_CLOSE_GKS);
}
