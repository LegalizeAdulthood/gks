#include "error.h"

#include <gks/gks.h>
#include <gks/gkserror.h>

#include <gtest/gtest.h>

using namespace testing;

namespace
{

class TestWSOpen : public Test
{
public:
    ~TestWSOpen() override = default;

protected:
    void SetUp() override;
    void TearDown() override;

    Gint wsId{0};
};

void TestWSOpen::SetUp()
{
    Test::SetUp();

    g_recordedErrors.clear();
    ASSERT_EQ(GERROR_NONE, gopengks(stderr, 0));
    const Gchar *connId{"tek4105"};
    const Gint wsType{GWSTYPE_TEK4105};
    ASSERT_EQ(GERROR_NONE, gopenws(wsId, connId, wsType));
}

void TestWSOpen::TearDown()
{
    ASSERT_EQ(GERROR_NONE, gclosews(wsId));
    ASSERT_EQ(GERROR_NONE, gclosegks());

    Test::TearDown();
}

} // namespace

TEST_F(TestWSOpen, gopengks)
{
    const Gint status = gopengks(stderr, 0);

    ASSERT_TRUE(expectError(status, GERROR_NOT_STATE_GKCL, GFN_OPEN_GKS));
}

TEST_F(TestWSOpen, gclosews)
{
    const Gint status = gclosegks();

    ASSERT_TRUE(expectError(status, GERROR_NOT_STATE_GKOP, GFN_CLOSE_GKS));
}

TEST_F(TestWSOpen, gdeactivatews)
{
    Gint wsId{0};

    const Gint status = gdeactivatews(wsId);

    ASSERT_TRUE(expectError(status, GERROR_NOT_STATE_WSAC, GFN_DEACTIVATE_WORKSTATION));
}

TEST_F(TestWSOpen, gcellarray)
{
    Grect rect =
    {
        {0.25f, 0.75f},
        {0.75f, 0.25f}
    };
    Gidim dim =
    {
        128, 128
    };
    Gint colors[] = { 0, 1 };

    const Gint status = gcellarray(&rect, &dim, colors);

    ASSERT_TRUE(expectError(status, GERROR_NOT_STATE_WSAC_SGOP, GFN_CELL_ARRAY));
}

TEST_F(TestWSOpen, gfillarea)
{
    const Gint status = gfillarea(0, nullptr);

    ASSERT_TRUE(expectError(status, GERROR_NOT_STATE_WSAC_SGOP, GFN_FILL_AREA));
}

TEST_F(TestWSOpen, ggdp)
{
    const Gint status = ggdp(0, nullptr, 0, nullptr);

    ASSERT_TRUE(expectError(status, GERROR_NOT_STATE_WSAC_SGOP, GFN_GDP));
}

TEST_F(TestWSOpen, gpolyline)
{
    const Gint status = gpolyline(0, nullptr);

    ASSERT_TRUE(expectError(status, GERROR_NOT_STATE_WSAC_SGOP, GFN_POLYLINE));
}

TEST_F(TestWSOpen, gpolymarker)
{
    const Gint status = gpolymarker(0, nullptr);

    ASSERT_TRUE(expectError(status, GERROR_NOT_STATE_WSAC_SGOP, GFN_POLYMARKER));
}

TEST_F(TestWSOpen, gtext)
{
    const Gint status = gtext(nullptr, nullptr);

    ASSERT_TRUE(expectError(status, GERROR_NOT_STATE_WSAC_SGOP, GFN_TEXT));
}
