#include "error.h"

#include <gks/gks.h>
#include <gks/gkserror.h>

#include <gtest/gtest.h>

using namespace testing;

namespace
{

class TestGKSOpen : public Test
{
public:
    ~TestGKSOpen() override = default;

protected:
    void SetUp() override;
    void TearDown() override;
};

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

AssertionResult expectError(Gint status, GError errNum, GFunction function)
{
    if (status != errNum)
    {
        return AssertionFailure() << "expected " << errNum << ", got " << g_recordedErrors[0].errNum;
    }
    if (g_recordedErrors.size() != 1U)
    {
        return AssertionFailure() << "expected one error, got " << g_recordedErrors.size();
    }
    if (g_recordedErrors[0].errNum != errNum)
    {
        return AssertionFailure() << "expected " << errNum << ", got " << g_recordedErrors[0].errNum;
    }
    if (g_recordedErrors[0].function != function)
    {
        return AssertionFailure() << "expected " << function << ", got " << g_recordedErrors[0].function;
    }
    return AssertionSuccess();
}

} // namespace

TEST_F(TestGKSOpen, alreadyOpen)
{
    const Gint status = gopengks(stderr, 0);

    EXPECT_TRUE(expectError(status, GERROR_NOT_STATE_GKCL, GFN_OPEN_GKS));
}

TEST_F(TestGKSOpen, closeWorkstation)
{
    const Gint wsId{0};

    const Gint status = gclosews(wsId);

    expectError(status, GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_CLOSE_WORKSTATION);
}

TEST_F(TestGKSOpen, activateWorkstation)
{
    Gint wsId{0};

    const Gint status = gactivatews(wsId);

    expectError(status, GERROR_NOT_STATE_WSOP_WSAC, GFN_ACTIVATE_WORKSTATION);
}

TEST_F(TestGKSOpen, gclearws)
{
    Gint wsId{0};

    const Gint status = gclearws(wsId, GALWAYS);

    expectError(status, GERROR_NOT_STATE_WSOP_WSAC, GFN_CLEAR_WORKSTATION);
}

TEST_F(TestGKSOpen, gdeactivatews)
{
    Gint wsId{0};

    const Gint status = gdeactivatews(wsId);

    expectError(status, GERROR_NOT_STATE_WSAC, GFN_DEACTIVATE_WORKSTATION);
}

TEST_F(TestGKSOpen, gupdatews)
{
    Gint wsId{0};

    const Gint status = gupdatews(wsId, GPERFORM);

    expectError(status, GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_UPDATE_WORKSTATION);
}

TEST_F(TestGKSOpen, gcellarray)
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

    expectError(status, GERROR_NOT_STATE_WSAC_SGOP, GFN_CELL_ARRAY);
}

TEST_F(TestGKSOpen, gfillarea)
{
    Gpoint points[] = {{0.0f, 0.0f}, {1.0f, 1.0f}};

    const Gint status = gfillarea(numOf(points), points);

    expectError(status, GERROR_NOT_STATE_WSAC_SGOP, GFN_FILL_AREA);
}

TEST_F(TestGKSOpen, ggdp)
{
    const Gint status = ggdp(0, nullptr, 0, nullptr);

    expectError(status, GERROR_NOT_STATE_WSAC_SGOP, GFN_GDP);
}

TEST_F(TestGKSOpen, gpolyline)
{
    Gpoint points[] = {{0.0f, 0.0f}, {1.0f, 1.0f}};

    const Gint status = gpolyline(numOf(points), points);

    expectError(status, GERROR_NOT_STATE_WSAC_SGOP, GFN_POLYLINE);
}

TEST_F(TestGKSOpen, gpolymarker)
{
    Gpoint points[] = {{0.0f, 0.0f}, {1.0f, 1.0f}};

    const Gint status = gpolymarker(numOf(points), points);

    expectError(status, GERROR_NOT_STATE_WSAC_SGOP, GFN_POLYMARKER);
}

TEST_F(TestGKSOpen, gtext)
{
    Gpoint pos{0.1f, 0.1f};

    const Gint status = gtext(&pos, "Hello, world!");

    expectError(status, GERROR_NOT_STATE_WSAC_SGOP, GFN_TEXT);
}

class TestInquire : public TestGKSOpen
{
protected:
    Gint wsId{0};
    Gint status{};
};

TEST_F(TestInquire, colorIndices)
{
    Gint values[1];
    Gintlist indices{1, values};
    Gint numIndices{};
    status = ginqcolorindices(wsId, sizeof(values), 0, &indices, &numIndices);

    expectError(status, GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_INQUIRE_COLOR_INDICES);
}

TEST_F(TestInquire, colorRepresentation)
{
    Gint index{1};
    Gcobundl value{};
    status = ginqcolorrep(wsId, index, &value);

    expectError(status, GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_INQUIRE_COLOR_REPRESENTATION);
}

TEST_F(TestInquire, textExtent)
{
    Gpoint pos{};
    Gextent value{};
    status = ginqtextextent(wsId, &pos, "Hello, world!", &value);

    expectError(status, GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_INQUIRE_TEXT_EXTENT);
}

TEST_F(TestInquire, ginqwsconntype)
{
    Gwsct ct{};
    Gint buffSize{};
    Gint numCts{};
    status = ginqwsconntype(wsId + 1, buffSize, &numCts, &ct);

    expectError(status, GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_INQUIRE_WORKSTATION_CONNECTION_AND_TYPE);
}

TEST_F(TestInquire, ginqwstran)
{
    Gwsti transform{};
    status = ginqwstran(wsId, &transform);

    expectError(status, GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_INQUIRE_WORKSTATION_TRANSFORMATION);
}

TEST_F(TestGKSOpen, gsetcolorrep)
{
    Gint wsId{0};
    Gint index{1};
    Gcobundl value{};
    const Gint status = gsetcolorrep(wsId, index, &value);

    expectError(status, GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_SET_COLOR_REPRESENTATION);
}

TEST_F(TestGKSOpen, gsetwswindow)
{
    Gint wsId{0};
    Glimit window{0.0f, 1.0f, 0.0f, 1.0f};
    const Gint status = gsetwswindow(wsId, &window);

    expectError(status, GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_SET_WORKSTATION_WINDOW);
}

TEST_F(TestGKSOpen, gsetwsviewport)
{
    Gint wsId{0};
    Glimit viewport{};
    const Gint status = gsetwsviewport(wsId, &viewport);

    expectError(status, GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_SET_WORKSTATION_VIEWPORT);
}
