#include "error.h"
#include "TestGKSOpen.h"
#include "TestWSOpen.h"

#include <gks/gks.h>

#include <gtest/gtest.h>

#include <algorithm>
#include <cstdio>
#include <string>

using namespace testing;

TEST(TestGKS, initiallyClosed)
{
    ASSERT_EQ(GGKCL, getGksOpState());
}

TEST(TestGKS, opened)
{
    ASSERT_EQ(GERROR_NONE, gopengks(stderr, 0L));

    ASSERT_EQ(GGKOP, getGksOpState());

    ASSERT_EQ(GERROR_NONE, gclosegks());
}

TEST(TestGKS, openedAndClosed)
{
    ASSERT_EQ(GERROR_NONE, gopengks(stderr, 0L));

    gclosegks();

    ASSERT_EQ(GGKCL, getGksOpState());
}

TEST(TestGKS, errorFileMustBeNonNull)
{
    g_recordedErrors.clear();

    const Gint status = gopengks(nullptr, 0L);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_ERROR_FILE, GFN_OPEN_GKS));
    ASSERT_EQ(GGKCL, getGksOpState());
}

TEST(TestGKS, errorHandling)
{
    gerrorhand(GERROR_NOT_STATE_GKCL, GFN_OPEN_GKS, stderr);

    ASSERT_TRUE(expectError(GERROR_NOT_STATE_GKCL, GERROR_NOT_STATE_GKCL, GFN_OPEN_GKS));

    g_recordedErrors.clear();
}

TEST_F(TestGKSOpen, gksLevel)
{
    Glevel level{};

    const Gint status = ginqlevelgks(&level);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(GLMA, level);
}

TEST_F(TestGKSOpen, availableWorkstationTypes)
{
    const Gint numWsTypes{10};
    Gint start{};
    int wsTypeValues[numWsTypes];
    for (int &wsTypeValue : wsTypeValues)
    {
        wsTypeValue = -1;
    }
    Gintlist wsTypes{0, wsTypeValues};
    Gint numAvailTypes{};

    const Gint status = ginqavailwstypes(numWsTypes, start, &wsTypes, &numAvailTypes);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(1, numAvailTypes);
    ASSERT_EQ(1, wsTypes.number);
    ASSERT_EQ(GWSTYPE_TEK4105, wsTypes.integers[0]);
}

TEST_F(TestGKSOpen, numberOfNormalizationTransforms)
{
    Gint numTransforms{-1};

    const Gint status = ginqmaxntrannum(&numTransforms);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(2, numTransforms);
}

TEST_F(TestGKSOpen, normalizationTransformZeroIsIdentity)
{
    Gtran transform{};

    const Gint status = ginqntran(1, &transform);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(0.0f, transform.w.xmin);
    ASSERT_EQ(1.0f, transform.w.xmax);
    ASSERT_EQ(0.0f, transform.w.ymin);
    ASSERT_EQ(1.0f, transform.w.ymax);
    ASSERT_EQ(0.0f, transform.v.xmin);
    ASSERT_EQ(1.0f, transform.v.xmax);
    ASSERT_EQ(0.0f, transform.v.ymin);
    ASSERT_EQ(1.0f, transform.v.ymax);
}

TEST_F(TestGKSOpen, setViewport)
{
    const Gint tranId = 1;
    const Gfloat xmin{0.2f};
    const Gfloat xmax{0.5f};
    const Gfloat ymin{0.4f};
    const Gfloat ymax{0.6f};
    Glimit viewport{xmin, xmax, ymin, ymax};

    const Gint status = gsetviewport(tranId, &viewport);

    ASSERT_EQ(GERROR_NONE, status);
    Gtran transform{};
    ginqntran(tranId, &transform);
    ASSERT_EQ(xmin, transform.v.xmin);
    ASSERT_EQ(xmax, transform.v.xmax);
    ASSERT_EQ(ymin, transform.v.ymin);
    ASSERT_EQ(ymax, transform.v.ymax);
}

TEST_F(TestGKSOpen, setWindow)
{
    const Gint tranId = 1;
    const Gfloat xmin{0.2f};
    const Gfloat xmax{0.5f};
    const Gfloat ymin{0.4f};
    const Gfloat ymax{0.6f};
    Glimit window{xmin, xmax, ymin, ymax};

    const Gint status = gsetwindow(tranId, &window);

    ASSERT_EQ(GERROR_NONE, status);
    Gtran transform{};
    ginqntran(tranId, &transform);
    ASSERT_EQ(xmin, transform.w.xmin);
    ASSERT_EQ(xmax, transform.w.xmax);
    ASSERT_EQ(ymin, transform.w.ymin);
    ASSERT_EQ(ymax, transform.w.ymax);
}

TEST_F(TestGKSOpen, maximumWorkstationNumbers)
{
    Gwsmax value{};

    const Gint status = ginqwsmaxnum(&value);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(1, value.open);
    ASSERT_EQ(1, value.active);
    ASSERT_EQ(0, value.assoc);
}

TEST_F(TestGKSOpen, initialClippingIndicator)
{
    Gcliprect clipping{};

    const Gint status = ginqclip(&clipping);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(GCLIP, clipping.ind);
    ASSERT_EQ(0.0f, clipping.rec.xmin);
    ASSERT_EQ(1.0f, clipping.rec.xmax);
    ASSERT_EQ(0.0f, clipping.rec.ymin);
    ASSERT_EQ(1.0f, clipping.rec.ymax);
}

TEST_F(TestGKSOpen, initialSetOfOpenWorkstationsIsEmpty)
{
    const Gint numBuffIds = 10;
    Gint ids[numBuffIds]{};
    Gintlist idList{numBuffIds, ids};
    Gint numIds{-1};

    const int status = ginqopenws(numBuffIds, 0, &idList, &numIds);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(0, numIds);
}

namespace
{

class TestWorkstationDescTable : public TestGKSOpen
{
public:
    ~TestWorkstationDescTable() override = default;

protected:
    Gwstype wsType{GWSTYPE_TEK4105};
};

}

TEST_F(TestWorkstationDescTable, colorFacilities)
{
    Gcofac facil{};
    Gint buffSize{};
    Gint numColors{};

    const Gint status = ginqcolorfacil(wsType, buffSize, &numColors, &facil);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(16, numColors);
    ASSERT_EQ(16, facil.colors);
    ASSERT_EQ(GCOLOR, facil.coavail);
    ASSERT_EQ(16, facil.predefined);
}

TEST_F(TestWorkstationDescTable, displaySpaceSize)
{
    Gdspsize size{};

    const Gint status = ginqdisplaysize(wsType, &size);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(GDC_OTHER, size.units);
    ASSERT_EQ(640, size.raster.x);
    ASSERT_EQ(480, size.raster.y);
}

TEST_F(TestWorkstationDescTable, fillAreaFacilities)
{
    Gflinter interiorStyles[4]{};
    Gint hatchStyles[10]{};
    Gflfac facil{};
    facil.interiors = interiorStyles;
    facil.hatches = hatchStyles;
    Gint numAvailHatches{};

    const Gint status = ginqfillfacil(wsType, numOf(hatchStyles), &numAvailHatches, &facil);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(1, facil.n_interiors);
    ASSERT_EQ(GHOLLOW, facil.interiors[0]);
    ASSERT_EQ(1, facil.predefined);
    ASSERT_EQ(0, numAvailHatches);
}

TEST_F(TestWorkstationDescTable, lineFacilities)
{
    Gint lineTypes[10]{};
    Gint numLineTypes{};
    Glnfac facil{};
    facil.types.number = numOf(lineTypes);
    facil.types.integers = lineTypes;

    const Gint status = ginqlinefacil(wsType, numOf(lineTypes), &numLineTypes, &facil);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_GE(numLineTypes, 1);
    ASSERT_NE(std::end(lineTypes), std::find(std::begin(lineTypes), std::end(lineTypes), GLN_SOLID));
    ASSERT_EQ(1, facil.widths);
    ASSERT_EQ(1.0f, facil.nom_width);
    ASSERT_EQ(1.0f, facil.min_width);
    ASSERT_EQ(1.0f, facil.max_width);
}

TEST_F(TestWorkstationDescTable, markerFacilities)
{
    Gint markerTypes[10]{};
    Gint numMarkerTypes{};
    Gmkfac facil{};
    facil.types.number = numOf(markerTypes);
    facil.types.integers = markerTypes;

    const Gint status = ginqmarkerfacil(wsType, numOf(markerTypes), &numMarkerTypes, &facil);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_GE(numMarkerTypes, 5);
    Gint expectedTypes[]{GMK_DOT, GMK_PLUS, GMK_STAR, GMK_CIRCLE, GMK_DIAGONAL_CROSS};
    std::sort(std::begin(expectedTypes), std::end(expectedTypes));
    std::sort(markerTypes, markerTypes + numMarkerTypes);
    ASSERT_TRUE(std::equal(markerTypes, markerTypes + numMarkerTypes, std::begin(expectedTypes)));
    ASSERT_EQ(1, facil.sizes);
    ASSERT_EQ(1.0f, facil.nom_size);
    ASSERT_EQ(1.0f, facil.min_size);
    ASSERT_EQ(1.0f, facil.max_size);
}

TEST_F(TestWorkstationDescTable, textFacilities)
{
    Gtxfp fontPrec[10]{};
    Gint buffSize{numOf(fontPrec)};
    Gint numFontPrec{};
    Gtxfac facil{};
    facil.fp_list = fontPrec;

    const Gint status = ginqtextfacil(wsType, buffSize, &numFontPrec, &facil);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(1, numFontPrec);
    ASSERT_EQ(1, facil.fps);
    ASSERT_EQ(1, facil.fp_list[0].font);
    ASSERT_EQ(GP_STRING, facil.fp_list[0].prec);
}

TEST_F(TestWorkstationDescTable, workstationStateTableLengths)
{
    Gwstables tables{-1, -1, -1, -1, -1, -1};

    const Gint status = ginqmaxwssttables(wsType, &tables);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(0, tables.line);
    ASSERT_EQ(0, tables.mark);
    ASSERT_EQ(0, tables.text);
    ASSERT_EQ(0, tables.fill);
    ASSERT_EQ(0, tables.pat);
    ASSERT_EQ(16, tables.color);
}

namespace
{

class TestWorkstationDescTableError : public TestGKSOpen
{
public:
    ~TestWorkstationDescTableError() override = default;

protected:
    Gwstype badWsType{GWSTYPE_TEK4105 + 1};
};

} // namespace

TEST_F(TestWorkstationDescTableError, colorFacilities)
{
    Gcofac facil{};
    Gint buffSize{};
    Gint numColors{};

    const Gint status = ginqcolorfacil(badWsType, buffSize, &numColors, &facil);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_WSTYPE, GFN_INQUIRE_COLOR_FACILITIES));
}

TEST_F(TestWorkstationDescTableError, displaySpaceSize)
{
    Gdspsize size{};

    const Gint status = ginqdisplaysize(badWsType, &size);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_WSTYPE, GFN_INQUIRE_DISPLAY_SPACE_SIZE));
}

TEST_F(TestWorkstationDescTableError, fillAreaFacilities)
{
    Gflinter interiorStyles[4]{};
    Gint hatchStyles[10]{};
    Gflfac facil{};
    facil.interiors = interiorStyles;
    facil.hatches = hatchStyles;
    Gint numAvailHatches{};

    const Gint status = ginqfillfacil(badWsType, numOf(hatchStyles), &numAvailHatches, &facil);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_WSTYPE, GFN_INQUIRE_FILL_FACILITIES));
}

TEST_F(TestWorkstationDescTableError, lineFacilities)
{
    Gint lineTypes[10]{};
    Gint numLineTypes{};
    Glnfac value{};

    const Gint status = ginqlinefacil(badWsType, numOf(lineTypes), &numLineTypes, &value);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_WSTYPE, GFN_INQUIRE_LINE_FACILITIES));
}

TEST_F(TestWorkstationDescTableError, markerFacilities)
{
    Gint markerTypes[10]{};
    Gint numMarkerTypes{};
    Gmkfac value{};

    const Gint status = ginqmarkerfacil(badWsType, numOf(markerTypes), &numMarkerTypes, &value);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_WSTYPE, GFN_INQUIRE_MARKER_FACILITIES));
}

TEST_F(TestWorkstationDescTableError, textFacilities)
{
    Gtxfp fontPrec[10]{};
    Gint buffSize{numOf(fontPrec)};
    Gint numFontPrec{};
    Gtxfac facil{};
    facil.fp_list = fontPrec;

    const Gint status = ginqtextfacil(badWsType, buffSize, &numFontPrec, &facil);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_WSTYPE, GFN_INQUIRE_TEXT_FACILITIES));
}

TEST_F(TestWorkstationDescTableError, workstationStateTableLengths)
{
    Gwstables tables{-1, -1, -1, -1, -1, -1};

    const Gint status = ginqmaxwssttables(badWsType, &tables);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_WSTYPE, GFN_INQUIRE_MAXIMUM_LENGTH_OF_WORKSTATION_STATE_TABLES));
}

TEST_F(TestGKSOpen, setClippingIndicator)
{
    const Gint status = gsetclip(GNOCLIP);

    ASSERT_EQ(GERROR_NONE, status);
    Gcliprect clipping{};
    ASSERT_EQ(GERROR_NONE, ginqclip(&clipping));
    ASSERT_EQ(GNOCLIP, clipping.ind);
}

TEST_F(TestWSOpen, operatingStateIsGSOP)
{
    ASSERT_EQ(GWSOP, getGksOpState());
}

TEST_F(TestWSOpen, afterActivateWorkstationStateIsGWSAC)
{
    gactivatews(wsId);

    ASSERT_EQ(GWSAC, getGksOpState());

    gdeactivatews(wsId);
}

TEST_F(TestWSOpen, deactivatelastactivatedworkstationstateisGWSOP)
{
    gactivatews(wsId);
    gdeactivatews(wsId);

    ASSERT_EQ(GWSOP, getGksOpState());
}

TEST_F(TestWSOpen, clearworkstation)
{
    gactivatews(wsId);

    ASSERT_EQ(GERROR_NONE, gclearws(wsId, GALWAYS));

    gdeactivatews(wsId);
}

TEST_F(TestWSOpen, initialWorkstationTransformationIsIdentity)
{
    Gwsti transform{};

    const Gint status = ginqwstran(wsId, &transform);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(GNOTPENDING, transform.wstus);
    ASSERT_EQ(0.0f, transform.request.v.xmin);
    ASSERT_EQ(1.0f, transform.request.v.xmax);
    ASSERT_EQ(0.0f, transform.request.v.ymin);
    ASSERT_EQ(1.0f, transform.request.v.ymax);
    ASSERT_EQ(0.0f, transform.request.w.xmin);
    ASSERT_EQ(1.0f, transform.request.w.xmax);
    ASSERT_EQ(0.0f, transform.request.w.ymin);
    ASSERT_EQ(1.0f, transform.request.w.ymax);
    ASSERT_EQ(0.0f, transform.current.v.xmin);
    ASSERT_EQ(1.0f, transform.current.v.xmax);
    ASSERT_EQ(0.0f, transform.current.v.ymin);
    ASSERT_EQ(1.0f, transform.current.v.ymax);
    ASSERT_EQ(0.0f, transform.current.w.xmin);
    ASSERT_EQ(1.0f, transform.current.w.xmax);
    ASSERT_EQ(0.0f, transform.current.w.ymin);
    ASSERT_EQ(1.0f, transform.current.w.ymax);
}

TEST_F(TestWSOpen, connectionIdAndType)
{
    Gint numCt{};
    Gwsct ct{};

    const Gint status = ginqwsconntype(wsId, 1, &numCt, &ct);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(1, numCt);
    ASSERT_EQ(connId, ct.conn);
    ASSERT_EQ(wsType, ct.type);
}

TEST_F(TestWSOpen, updateWorkstation)
{
    ASSERT_EQ(GERROR_NONE, gupdatews(wsId, GPERFORM));
}

TEST_F(TestWSOpen, setWorkstationViewport)
{
    const Gfloat xmin = 0.2f;
    const Gfloat xmax = 0.4f;
    const Gfloat ymin = 0.3f;
    const Gfloat ymax = 0.5f;
    Glimit viewport{xmin, xmax, ymin, ymax};

    ASSERT_EQ(GERROR_NONE, gsetwsviewport(wsId, &viewport));

    Gwsti transform{};
    ASSERT_EQ(GERROR_NONE, ginqwstran(wsId, &transform));
    ASSERT_EQ(GNOTPENDING, transform.wstus);
    ASSERT_EQ(xmin, transform.current.v.xmin);
    ASSERT_EQ(xmax, transform.current.v.xmax);
    ASSERT_EQ(ymin, transform.current.v.ymin);
    ASSERT_EQ(ymax, transform.current.v.ymax);
}

TEST_F(TestWSOpen, setWorkstationWindow)
{
    const Gfloat xmin = 0.2f;
    const Gfloat xmax = 0.4f;
    const Gfloat ymin = 0.3f;
    const Gfloat ymax = 0.5f;
    Glimit window{xmin, xmax, ymin, ymax};

    const Gint status = gsetwswindow(wsId, &window);

    ASSERT_EQ(GERROR_NONE, status);
    Gwsti transform{};
    ASSERT_EQ(GERROR_NONE, ginqwstran(wsId, &transform));
    ASSERT_EQ(GNOTPENDING, transform.wstus);
    ASSERT_EQ(xmin, transform.current.w.xmin);
    ASSERT_EQ(xmax, transform.current.w.xmax);
    ASSERT_EQ(ymin, transform.current.w.ymin);
    ASSERT_EQ(ymax, transform.current.w.ymax);
}

TEST_F(TestWSOpen, textExtent)
{
    Gpoint pos{0.1f, 0.1f};
    Gextent value{};

    const Gint status = ginqtextextent(wsId, &pos, "Hello, World!", &value);

    ASSERT_EQ(GERROR_NONE, status);
}

TEST(TestWorkstation, closeLastWorkstationEntersGGKOPState)
{
    gopengks(stderr, 0L);
    Gint wsId{0};
    const Gchar *connId{"tek4105"};
    Gint wsType{GWSTYPE_TEK4105};
    gopenws(wsId, connId, wsType);

    Gopst before{};
    ginqopst(&before);
    gclosews(wsId);
    Gopst after{};
    ginqopst(&after);

    ASSERT_EQ(GWSOP, before);
    ASSERT_EQ(GGKOP, after);

    gclosegks();
}

namespace
{

class TestOpenWorkstationError : public TestGKSOpen
{
public:
    ~TestOpenWorkstationError() override = default;

protected:
    void TearDown() override;

    Gint wsId{};
    const Gchar *connId{"tek4105"};
    Gwstype wsType{GWSTYPE_TEK4105};
};

void TestOpenWorkstationError::TearDown()
{
    ASSERT_EQ(GGKOP, getGksOpState());

    TestGKSOpen::TearDown();
}

} // namespace

TEST_F(TestOpenWorkstationError, workstationIdNegative)
{
    wsId = -1;

    const Gint status = gopenws(wsId, connId, wsType);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_WSID, GFN_OPEN_WORKSTATION));
}

TEST_F(TestOpenWorkstationError, workstationIdTooLarge)
{
    const Gint maxOpenWorkstations{1};
    wsId = maxOpenWorkstations;

    const Gint status = gopenws(wsId, connId, wsType);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_WSID, GFN_OPEN_WORKSTATION));
}

TEST_F(TestOpenWorkstationError, invalidWorkstationType)
{
    ++wsType;
    const Gint status = gopenws(wsId, connId, wsType);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_WSTYPE, GFN_OPEN_WORKSTATION));
}

TEST_F(TestOpenWorkstationError, workstationAlreadyOpen)
{
    gopenws(wsId, connId, wsType);
    ASSERT_EQ(GWSOP, getGksOpState());

    const Gint status = gopenws(wsId, connId, wsType);

    ASSERT_TRUE(expectError(status, GERROR_WS_IS_OPEN, GFN_OPEN_WORKSTATION));

    gclosews(wsId);
}

namespace
{

class TestCloseWorkstationError : public TestWSOpen
{
public:
    ~TestCloseWorkstationError() override = default;

protected:
    void SetUp() override;
    void TearDown() override;
};

void TestCloseWorkstationError::SetUp()
{
    TestWSOpen::SetUp();

    ASSERT_EQ(GWSOP, getGksOpState());
    ASSERT_TRUE(g_recordedErrors.empty());
}

void TestCloseWorkstationError::TearDown()
{
    ASSERT_EQ(GWSOP, getGksOpState());

    TestWSOpen::TearDown();
}

} // namespace

TEST_F(TestCloseWorkstationError, workstationIdNegative)
{
    Gint badWsId{-1};

    const Gint status = gclosews(badWsId);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_WSID, GFN_CLOSE_WORKSTATION));
}

TEST_F(TestCloseWorkstationError, workstationIdTooLarge)
{
    const Gint maxOpenWorkstations{1};
    Gint badWsId{maxOpenWorkstations};

    const Gint status = gclosews(badWsId);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_WSID, GFN_CLOSE_WORKSTATION));
}

TEST_F(TestCloseWorkstationError, workstationIsActive)
{
    gactivatews(wsId);

    const Gint status = gclosews(wsId);

    ASSERT_TRUE(expectError(status, GERROR_WS_IS_ACTIVE, GFN_CLOSE_WORKSTATION));

    gdeactivatews(wsId);
}

namespace
{

class TestActivateWorkstationError : public TestWSOpen
{
public:
    ~TestActivateWorkstationError() override = default;

protected:
    void SetUp() override;
    void TearDown() override;
};

void TestActivateWorkstationError::SetUp()
{
    TestWSOpen::SetUp();

    ASSERT_EQ(GWSOP, getGksOpState());
    ASSERT_TRUE(g_recordedErrors.empty());
}

void TestActivateWorkstationError::TearDown()
{
    ASSERT_EQ(GWSOP, getGksOpState());

    TestWSOpen::TearDown();
}

} // namespace

TEST_F(TestActivateWorkstationError, workstationIdNegative)
{
    Gint badWsId{-1};

    const Gint status = gactivatews(badWsId);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_WSID, GFN_ACTIVATE_WORKSTATION));
}

TEST_F(TestActivateWorkstationError, workstationIdTooLarge)
{
    const Gint maxOpenWorkstations{1};
    const Gint badWsId{maxOpenWorkstations};

    const Gint status = gactivatews(badWsId);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_WSID, GFN_ACTIVATE_WORKSTATION));
}

TEST_F(TestActivateWorkstationError, workstationIsActive)
{
    gactivatews(wsId);

    const Gint status = gactivatews(wsId);

    ASSERT_TRUE(expectError(status, GERROR_WS_IS_ACTIVE, GFN_ACTIVATE_WORKSTATION));

    gdeactivatews(wsId);
}

namespace
{

class TestDeactivateWorkstationError : public TestWSOpen
{
public:
    ~TestDeactivateWorkstationError() override = default;

protected:
    void SetUp() override;
    void TearDown() override;
};

void TestDeactivateWorkstationError::SetUp()
{
    TestWSOpen::SetUp();

    ASSERT_EQ(GERROR_NONE, gactivatews(wsId));
    ASSERT_EQ(GWSAC, getGksOpState());
    ASSERT_TRUE(g_recordedErrors.empty());
}

void TestDeactivateWorkstationError::TearDown()
{
    ASSERT_EQ(GWSAC, getGksOpState());
    ASSERT_EQ(GERROR_NONE, gdeactivatews(wsId));

    TestWSOpen::TearDown();
}

} // namespace

TEST_F(TestDeactivateWorkstationError, workstationIdNegative)
{
    Gint badWsId{-1};

    const Gint status = gdeactivatews(badWsId);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_WSID, GFN_DEACTIVATE_WORKSTATION));
    ASSERT_EQ(GWSAC, getGksOpState());
}

TEST_F(TestDeactivateWorkstationError, workstationIdTooLarge)
{
    const Gint maxOpenWorkstations{1};
    Gint badWsId{maxOpenWorkstations};

    const Gint status = gdeactivatews(badWsId);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_WSID, GFN_DEACTIVATE_WORKSTATION));
}

TEST_F(TestDeactivateWorkstationError, workstationNotActivated)
{
    // This test requires support for multiple open workstations
}

namespace
{

class TestClearWorkstationError : public TestWSOpen
{
public:
    ~TestClearWorkstationError() override = default;

protected:
    void SetUp() override;
};

void TestClearWorkstationError::SetUp()
{
    TestWSOpen::SetUp();

    ASSERT_EQ(GWSOP, getGksOpState());
    ASSERT_TRUE(g_recordedErrors.empty());
}

} // namespace

TEST_F(TestClearWorkstationError, workstationIdNegative)
{
    Gint badWsId{-1};

    const Gint status = gclearws(badWsId, GALWAYS);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_WSID, GFN_CLEAR_WORKSTATION));
}

TEST_F(TestClearWorkstationError, workstationIdTooLarge)
{
    const Gint maxOpenWorkstations{1};
    Gint badWsId{maxOpenWorkstations};

    const Gint status = gclearws(badWsId, GALWAYS);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_WSID, GFN_CLEAR_WORKSTATION));
}

TEST_F(TestClearWorkstationError, workstationNotActivated)
{
    // This test requires support for multiple open workstations
}

namespace
{

class TestUpdateWorkstationError : public TestWSOpen
{
public:
    ~TestUpdateWorkstationError() override = default;

protected:
    void SetUp() override;
};

void TestUpdateWorkstationError::SetUp()
{
    TestWSOpen::SetUp();

    ASSERT_EQ(GWSOP, getGksOpState());
    ASSERT_TRUE(g_recordedErrors.empty());
}

} // namespace

TEST_F(TestUpdateWorkstationError, workstationIdNegative)
{
    Gint badWsId{-1};

    const Gint status = gupdatews(badWsId, GPERFORM);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_WSID, GFN_UPDATE_WORKSTATION));
}

TEST_F(TestUpdateWorkstationError, workstationIdTooLarge)
{
    const Gint maxOpenWorkstations{1};
    Gint badWsId{maxOpenWorkstations};

    const Gint status = gupdatews(badWsId, GPERFORM);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_WSID, GFN_UPDATE_WORKSTATION));
}

TEST_F(TestUpdateWorkstationError, workstationNotOpen)
{
    // This test requires multiple workstations to be open
}

namespace
{

class TestOutputPrimitives : public TestWSOpen
{
public:
    ~TestOutputPrimitives() override = default;

protected:
    void SetUp() override;
    void TearDown() override;
};

void TestOutputPrimitives::SetUp()
{
    TestWSOpen::SetUp();

    EXPECT_EQ(GERROR_NONE, gactivatews(wsId));
}

void TestOutputPrimitives::TearDown()
{
    ASSERT_EQ(GERROR_NONE, gdeactivatews(wsId));

    TestWSOpen::TearDown();
}

} // namespace

TEST_F(TestOutputPrimitives, polyline)
{
    Gpoint points[] =
    {
    { 0.0f, 0.5f },
    { 0.5f, 0.5f }
    };

    const Gint status = gpolyline(sizeof(points)/sizeof(points[0]), points);

    ASSERT_EQ(GERROR_NONE, status);
}

TEST_F(TestOutputPrimitives, polymarker)
{
    Gpoint points[] =
    {
    { 0.0f, 0.5f },
    { 0.5f, 0.5f }
    };

    const Gint status = gpolymarker(sizeof(points)/sizeof(points[0]), points);

    ASSERT_EQ(GERROR_NONE, status);
}

TEST_F(TestOutputPrimitives, text)
{
    Gpoint start{0.0f, 0.5f};

    const Gint status = gtext(&start, "Hello, world!");

    ASSERT_EQ(GERROR_NONE, status);
}

TEST_F(TestOutputPrimitives, fillArea)
{
    Gpoint points[] = 
    {
    {0.25f, 0.25f},
    {0.50f, 0.25f},
    {0.50f, 0.50f},
    {0.25f, 0.50f}
    };

    const Gint status = gfillarea(sizeof(points)/sizeof(points[0]), points);

    ASSERT_EQ(GERROR_NONE, status);
}

TEST_F(TestOutputPrimitives, cellArray)
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

    ASSERT_EQ(GERROR_NONE, status);
}

TEST_F(TestOutputPrimitives, generalizedDrawingPrimitive)
{
    Gpoint points[] =
    {
    {0.1f, 0.1f},
    {0.2f, 0.2f}
    };
    Gint gdpId = 1;
    Ggdprec data{};

    const Gint status = ggdp(sizeof(points)/sizeof(points[0]), points, gdpId, &data);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_GDP_ID, GFN_GENERALIZED_DRAWING_PRIMITIVE));
}

namespace
{

class TestOutputPrimitiveErrors : public TestOutputPrimitives
{
protected:
    Gpoint points[4]{{0.25f, 0.25f}, {0.50f, 0.25f}, {0.50f, 0.50f}, {0.25f, 0.50f}};
};

} // namespace

TEST_F(TestOutputPrimitiveErrors, polylineWithTooFewPoints)
{
    const Gint status = gpolyline(1, points);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_NUM_POINTS, GFN_POLYLINE));
}

TEST_F(TestOutputPrimitiveErrors, polymarkerWithTooFewPoints)
{
    const Gint status = gpolymarker(0, points);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_NUM_POINTS, GFN_POLYMARKER));
}

TEST_F(TestOutputPrimitiveErrors, fillAreaWithTooFewPoints)
{
    const Gint status = gfillarea(2, points);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_NUM_POINTS, GFN_FILL_AREA));
}

TEST_F(TestOutputPrimitiveErrors, cellArrayInvalidXDimension)
{
    Grect rect = {{0.25f, 0.75f}, {0.75f, 0.25f}};
    Gidim dim = {0, 1};
    Gint colors[] = {0, 1};

    const Gint status = gcellarray(&rect, &dim, colors);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_COLOR_DIMENSIONS, GFN_CELL_ARRAY));
}

TEST_F(TestOutputPrimitiveErrors, generalizedDrawingPrimitive)
{
    Gpoint points[] =
    {
        {0.1f, 0.1f},
        {0.2f, 0.2f}
    };
    Gint gdpId = 1;
    Ggdprec data{};

    const Gint status = ggdp(sizeof(points)/sizeof(points[0]), points, gdpId, &data);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_GDP_ID, GFN_GENERALIZED_DRAWING_PRIMITIVE));
}

namespace
{

class TestGKSInitialStateList : public TestGKSOpen
{
protected:
    Gint value{-1};
};

}//

TEST_F(TestGKSInitialStateList, lineType)
{
    const Gint status = ginqlinetype(&value);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(GLN_SOLID, value);
}

TEST_F(TestGKSInitialStateList, lineColorIndex)
{
    const Gint status = ginqlinecolorind(&value);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(1, value);
}

TEST_F(TestGKSInitialStateList, lineIndex)
{
    const Gint status = ginqlineind(&value);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(1, value);
}

TEST_F(TestGKSInitialStateList, markerType)
{
    const Gint status = ginqmarkertype(&value);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(GMK_STAR, value);
}

TEST_F(TestGKSInitialStateList, markerSize)
{
    Gfloat size{};

    const Gint status = ginqmarkersize(&size);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(1.0f, size);
}

TEST_F(TestGKSInitialStateList, markerColorIndex)
{
    const Gint status = ginqmarkercolorind(&value);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(1, value);
}

TEST_F(TestGKSInitialStateList, markerIndex)
{
    const Gint status = ginqmarkerind(&value);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(1, value);
}

TEST_F(TestGKSInitialStateList, textHeight)
{
    Gfloat height{};
    const Gint status = ginqcharheight(&height);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(0.01f, height);
}

TEST_F(TestGKSInitialStateList, charBaseVector)
{
    Gpoint base{-1.0f, -1.0f};
    const Gint status = ginqcharbase(&base);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(1.0f, base.x);
    ASSERT_EQ(0.0f, base.y);
}

TEST_F(TestGKSInitialStateList, charUpVector)
{
    Gpoint up{-1.0f, -1.0f};
    const Gint status = ginqcharup(&up);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(0.0f, up.x);
    ASSERT_EQ(1.0f, up.y);
}

TEST_F(TestGKSInitialStateList, characterWidth)
{
    Gfloat width{-1.0f};
    const Gint status = ginqcharwidth(&width);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(0.01f, width);
}

TEST_F(TestGKSInitialStateList, textColorIndex)
{
    const Gint status = ginqtextcolorind(&value);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(1, value);
}

TEST_F(TestGKSInitialStateList, textIndex)
{
    const Gint status = ginqtextind(&value);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(1, value);
}

TEST_F(TestGKSInitialStateList, textAlignment)
{
    Gtxalign align{};
    const Gint status = ginqtextalign(&align);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(GAH_NORMAL, align.hor);
    ASSERT_EQ(GAV_NORMAL, align.ver);
}

TEST_F(TestGKSInitialStateList, textFontAndPrecision)
{
    Gtxfp fontPrec{};
    const Gint status = ginqtextfontprec(&fontPrec);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(1, fontPrec.font);
    ASSERT_EQ(GP_STRING, fontPrec.prec);
}

TEST_F(TestGKSInitialStateList, charExpansionFactor)
{
    Gfloat expandFactor{};
    const Gint status = ginqcharexpan(&expandFactor);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(1.0f, expandFactor);
}

TEST_F(TestGKSInitialStateList, charSpacing)
{
    Gfloat spacing{-1.0f};
    const Gint status = ginqcharspace(&spacing);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(0.0f, spacing);
}

TEST_F(TestGKSInitialStateList, fillAreaInteriorStyle)
{
    Gflinter style{GSOLID};
    const Gint status = ginqfillstyle(&style);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(GHOLLOW, style);
}

TEST_F(TestGKSInitialStateList, fillAreaInteriorStyleIndex)
{
    const Gint status = ginqfillstyleind(&value);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(1, value);
}

TEST_F(TestGKSInitialStateList, fillAreaColorIndex)
{
    const Gint status = ginqfillcolorind(&value);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(1, value);
}

TEST_F(TestGKSInitialStateList, fillAreaIndex)
{
    const Gint status = ginqfillind(&value);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(1, value);
}

TEST_F(TestGKSInitialStateList, patternHeight)
{
    Gpoint vec{};
    const Gint status = ginqpatheight(&vec);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(0.0f, vec.x);
    ASSERT_EQ(1.0f, vec.y);
}

TEST_F(TestGKSInitialStateList, patternWidth)
{
    Gpoint vec{};
    const Gint status = ginqpatwidth(&vec);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(1.0f, vec.x);
    ASSERT_EQ(0.0f, vec.y);
}

TEST_F(TestGKSInitialStateList, patternReferencePoint)
{
    Gpoint refPt{1.0f, 1.0f};
    const Gint status = ginqpatrefpt(&refPt);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(0.0f, refPt.x);
    ASSERT_EQ(0.0f, refPt.y);
}

TEST_F(TestGKSInitialStateList, attributeSourceFlags)
{
    Gasfs asfs{};
    const Gint status = ginqasf(&asfs);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(GINDIVIDUAL, asfs.ln_type);
    ASSERT_EQ(GINDIVIDUAL, asfs.ln_width);
    ASSERT_EQ(GINDIVIDUAL, asfs.ln_color);
    ASSERT_EQ(GINDIVIDUAL, asfs.mk_type);
    ASSERT_EQ(GINDIVIDUAL, asfs.mk_size);
    ASSERT_EQ(GINDIVIDUAL, asfs.mk_color);
    ASSERT_EQ(GINDIVIDUAL, asfs.tx_fp);
    ASSERT_EQ(GINDIVIDUAL, asfs.tx_exp);
    ASSERT_EQ(GINDIVIDUAL, asfs.tx_space);
    ASSERT_EQ(GINDIVIDUAL, asfs.tx_color);
    ASSERT_EQ(GINDIVIDUAL, asfs.fl_inter);
    ASSERT_EQ(GINDIVIDUAL, asfs.fl_style);
    ASSERT_EQ(GINDIVIDUAL, asfs.fl_color);
}

TEST_F(TestGKSInitialStateList, currentNormalizationTransform)
{
    Gint transform{-1};
    const Gint status = ginqcurrntrannum(&transform);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(0, transform);
}

namespace
{

class TestGKSSetStateList : public TestGKSOpen
{
public:
    ~TestGKSSetStateList() override = default;

protected:
    Gint value{-1};
};

}

TEST_F(TestGKSSetStateList, aspectSourceFlags)
{
    Gasfs asfs =
    {
        GBUNDLED,
        GINDIVIDUAL,
        GBUNDLED,
        GINDIVIDUAL,
        GBUNDLED,
        GINDIVIDUAL,
        GBUNDLED,
        GINDIVIDUAL,
        GBUNDLED,
        GINDIVIDUAL,
        GBUNDLED,
        GINDIVIDUAL,
        GBUNDLED,
    };
    const Gint status = gsetasf(&asfs);

    ASSERT_EQ(GERROR_NONE, status);
    Gasfs current{};
    ginqasf(&current);
    ASSERT_EQ(asfs.ln_type, current.ln_type);
    ASSERT_EQ(asfs.ln_width, current.ln_width);
    ASSERT_EQ(asfs.ln_color, current.ln_color);
    ASSERT_EQ(asfs.mk_type, current.mk_type);
    ASSERT_EQ(asfs.mk_size, current.mk_size);
    ASSERT_EQ(asfs.mk_color, current.mk_color);
    ASSERT_EQ(asfs.tx_fp, current.tx_fp);
    ASSERT_EQ(asfs.tx_exp, current.tx_exp);
    ASSERT_EQ(asfs.tx_space, current.tx_space);
    ASSERT_EQ(asfs.tx_color, current.tx_color);
    ASSERT_EQ(asfs.fl_inter, current.fl_inter);
    ASSERT_EQ(asfs.fl_style, current.fl_style);
    ASSERT_EQ(asfs.fl_color, current.fl_color);
}

TEST_F(TestGKSSetStateList, charExpansionFactor)
{
    const Gfloat expan{0.5f};
    const Gint status = gsetcharexpan(expan);

    ASSERT_EQ(GERROR_NONE, status);
    Gfloat current{};
    ginqcharexpan(&current);
    ASSERT_EQ(expan, current);
}

TEST_F(TestGKSSetStateList, charHeight)
{
    const Gint status = gsetcharheight(0.5f);

    ASSERT_EQ(GERROR_NONE, status);
    Gfloat charHeight{-1.0f};
    ginqcharheight(&charHeight);
    ASSERT_EQ(0.5f, charHeight);
}

TEST_F(TestGKSSetStateList, charSpacing)
{
    const Gfloat space{0.5f};
    const Gint status = gsetcharspace(space);

    ASSERT_EQ(GERROR_NONE, status);
    Gfloat current{};
    ginqcharspace(&current);
    ASSERT_EQ(space, current);
}

TEST_F(TestGKSSetStateList, charUpVector)
{
    Gpoint up{1.0f, 0.0f};
    const Gint status = gsetcharup(&up);

    ASSERT_EQ(GERROR_NONE, status);
    Gpoint current{};
    ginqcharup(&current);
    ASSERT_EQ(up.x, current.x);
    ASSERT_EQ(up.y, current.y);
}

TEST_F(TestGKSSetStateList, fillAreaColorIndex)
{
    const Gint status = gsetfillcolorind(0);

    ASSERT_EQ(GERROR_NONE, status);
    ginqfillcolorind(&value);
    ASSERT_EQ(0, value);
}

TEST_F(TestGKSSetStateList, fillAreaIndex)
{
    const Gint status = gsetfillind(1);

    ASSERT_EQ(GERROR_NONE, status);
    ginqfillind(&value);
    ASSERT_EQ(1, value);
}

TEST_F(TestGKSSetStateList, fillAreaInteriorStyle)
{
    const Gint status = gsetfillstyle(GSOLID);

    ASSERT_EQ(GERROR_NONE, status);
    Gflinter current{};
    ginqfillstyle(&current);
    ASSERT_EQ(GSOLID, current);
}

TEST_F(TestGKSSetStateList, fillAreaInteriorStyleIndex)
{
    const Gint status = gsetfillstyleind(2);

    ASSERT_EQ(GERROR_NONE, status);
    ginqfillstyleind(&value);
    ASSERT_EQ(2, value);
}

TEST_F(TestGKSSetStateList, lineType)
{
    const Gint status = gsetlinetype(GLN_DASHED);

    ASSERT_EQ(GERROR_NONE, status);
    ginqlinetype(&value);
    ASSERT_EQ(GLN_DASHED, value);
}

TEST_F(TestGKSSetStateList, lineColorIndex)
{
    const Gint status = gsetlinecolorind(1);

    ASSERT_EQ(GERROR_NONE, status);
    ginqlinecolorind(&value);
    ASSERT_EQ(1, value);
}

TEST_F(TestGKSSetStateList, lineIndex)
{
    const Gint status = gsetlineind(1);

    ASSERT_EQ(GERROR_NONE, status);
    ginqlineind(&value);
    ASSERT_EQ(1, value);
}

TEST_F(TestGKSSetStateList, markerType)
{
    const Gint status = gsetmarkertype(GMK_DIAGONAL_CROSS);

    ASSERT_EQ(GERROR_NONE, status);
    ginqmarkertype(&value);
    ASSERT_EQ(GMK_DIAGONAL_CROSS, value);
}

TEST_F(TestGKSSetStateList, markerSize)
{
    const Gint status = gsetmarkersize(2.0f);

    ASSERT_EQ(GERROR_NONE, status);
    Gfloat size{-1.0f};
    ginqmarkersize(&size);
    ASSERT_EQ(2.0f, size);
}

TEST_F(TestGKSSetStateList, markerColorIndex)
{
    Gint index{1};
    const Gint status = gsetmarkercolorind(index);

    ASSERT_EQ(GERROR_NONE, status);
    ginqmarkercolorind(&value);
    ASSERT_EQ(index, value);
}

TEST_F(TestGKSSetStateList, markerIndex)
{
    Gint index{1};
    const Gint status = gsetmarkerind(index);

    ASSERT_EQ(GERROR_NONE, status);
    ginqmarkerind(&value);
    ASSERT_EQ(index, value);
}

TEST_F(TestGKSSetStateList, normalizationTransform)
{
    const Gint status = gselntran(1);

    ASSERT_EQ(GERROR_NONE, status);
    Gint transform{0};
    ginqcurrntrannum(&transform);
    ASSERT_EQ(1, transform);
}

TEST_F(TestGKSSetStateList, textAlignment)
{
    Gtxalign align{GAH_LEFT, GAV_BOTTOM};
    const Gint status = gsettextalign(&align);

    ASSERT_EQ(GERROR_NONE, status);
    Gtxalign current{};
    ginqtextalign(&current);
    ASSERT_EQ(align.hor, current.hor);
    ASSERT_EQ(align.ver, current.ver);
}

TEST_F(TestGKSSetStateList, textColorIndex)
{
    const Gint color{0};
    const Gint status = gsettextcolorind(color);

    ASSERT_EQ(GERROR_NONE, status);
    ginqtextcolorind(&value);
    ASSERT_EQ(color, value);
}

TEST_F(TestGKSSetStateList, textFontAndPrecision)
{
    Gtxfp font{2, GP_STROKE};
    const Gint status = gsettextfontprec(&font);

    ASSERT_EQ(GERROR_NONE, status);
    Gtxfp current{};
    ginqtextfontprec(&current);
    ASSERT_EQ(font.font, current.font);
    ASSERT_EQ(font.prec, current.prec);
}

TEST_F(TestGKSSetStateList, textIndex)
{
    const Gint index{1};
    const Gint status = gsettextind(index);

    ASSERT_EQ(GERROR_NONE, status);
    ginqtextind(&value);
    ASSERT_EQ(index, value);
}

namespace
{

class TestGKSSetStateListErrorHandling : public TestGKSOpen
{
public:
    ~TestGKSSetStateListErrorHandling() override = default;

protected:
    Gint value{-1};
};

} // namespace

TEST_F(TestGKSSetStateListErrorHandling, characterExpansionFactor)
{
    const Gint status = gsetcharexpan(0.0f);

    ASSERT_TRUE(expectError(status, GERROR_CHAR_EXPANSION_NOT_POSITIVE, GFN_SET_CHARACTER_EXPANSION_FACTOR));
    Gfloat value{};
    ASSERT_EQ(GERROR_NONE, ginqcharexpan(&value));
    ASSERT_EQ(1.0f, value);
}

TEST_F(TestGKSSetStateListErrorHandling, characterHeightZero)
{
    const Gint status = gsetcharheight(0.0f);

    ASSERT_TRUE(expectError(status, GERROR_CHAR_HEIGHT_NOT_POSITIVE, GFN_SET_CHARACTER_HEIGHT));
    Gfloat value{-1.0f};
    ginqcharheight(&value);
    ASSERT_EQ(0.01f, value);
}

TEST_F(TestGKSSetStateListErrorHandling, characterHeightNegative)
{
    const Gint status = gsetcharheight(-1.0f);

    ASSERT_TRUE(expectError(status, GERROR_CHAR_HEIGHT_NOT_POSITIVE, GFN_SET_CHARACTER_HEIGHT));
    Gfloat value{-1.0f};
    ginqcharheight(&value);
    ASSERT_EQ(0.01f, value);
}

TEST_F(TestGKSSetStateListErrorHandling, characterUpVector)
{
    Gpoint up{};
    const Gint status = gsetcharup(&up);

    ASSERT_TRUE(expectError(status, GERROR_CHAR_UP_LENGTH_ZERO, GFN_SET_CHARACTER_UP_VECTOR));
    Gpoint value{};
    ginqcharup(&value);
    ASSERT_EQ(0.0f, value.x);
    ASSERT_EQ(1.0f, value.y);
}

TEST_F(TestGKSSetStateListErrorHandling, fillAreaColor)
{
    const Gint status = gsetfillcolorind(-1);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_COLOR_INDEX, GFN_SET_FILL_AREA_COLOR_INDEX));
    Gint value{};
    ginqfillcolorind(&value);
    ASSERT_EQ(1, value);
}

TEST_F(TestGKSSetStateListErrorHandling, fillAreaIndex)
{
    const Gint status = gsetfillind(0);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_FILL_INDEX, GFN_SET_FILL_AREA_INDEX));
    Gint value{};
    ginqfillind(&value);
    ASSERT_EQ(1, value);
}

TEST_F(TestGKSSetStateListErrorHandling, fillAreaInteriorStyleIndex)
{
    const Gint status = gsetfillstyleind(0);

    ASSERT_TRUE(expectError(status, GERROR_FILL_STYLE_ZERO, GFN_SET_FILL_AREA_INTERIOR_STYLE));
    Gint value{};
    ginqfillstyleind(&value);
    ASSERT_EQ(1, value);
}

TEST_F(TestGKSSetStateListErrorHandling, lineColorTooSmall)
{
    const Gint status = gsetlinecolorind(-1);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_COLOR_INDEX, GFN_SET_POLYLINE_COLOR_INDEX));
    Gint value{};
    ginqlinecolorind(&value);
    ASSERT_EQ(1, value);
}

TEST_F(TestGKSSetStateListErrorHandling, lineColorTooLarge)
{
    const Gint status = gsetlinecolorind(2);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_COLOR_INDEX, GFN_SET_POLYLINE_COLOR_INDEX));
    Gint value{};
    ginqlinecolorind(&value);
    ASSERT_EQ(1, value);
}

TEST_F(TestGKSSetStateListErrorHandling, lineIndex)
{
    const Gint status = gsetlineind(0);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_LINE_INDEX, GFN_SET_POLYLINE_INDEX));
    Gint value{};
    ginqlineind(&value);
    ASSERT_EQ(1, value);
}

TEST_F(TestGKSSetStateListErrorHandling, lineType)
{
    const Gint status = gsetlinetype(0);

    ASSERT_TRUE(expectError(status, GERROR_LINE_TYPE_ZERO, GFN_SET_LINETYPE));
    Gint value{};
    ginqlinetype(&value);
    ASSERT_EQ(1, value);
}

TEST_F(TestGKSSetStateListErrorHandling, markerColorTooSmall)
{
    const Gint status = gsetmarkercolorind(-1);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_COLOR_INDEX, GFN_SET_POLYMARKER_COLOR_INDEX));
    Gint value{};
    ginqmarkercolorind(&value);
    ASSERT_EQ(1, value);
}

TEST_F(TestGKSSetStateListErrorHandling, markerColorTooLarge)
{
    const Gint status = gsetmarkercolorind(2);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_COLOR_INDEX, GFN_SET_POLYMARKER_COLOR_INDEX));
    Gint value{};
    ginqmarkercolorind(&value);
    ASSERT_EQ(1, value);
}

TEST_F(TestGKSSetStateListErrorHandling, markerIndex)
{
    const Gint status = gsetmarkerind(0);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_MARKER_INDEX, GFN_SET_POLYMARKER_INDEX));
    Gint value{};
    ginqmarkerind(&value);
    ASSERT_EQ(1, value);
}

TEST_F(TestGKSSetStateListErrorHandling, markerSize)
{
    const Gint status = gsetmarkersize(-1.0f);

    ASSERT_TRUE(expectError(status, GERROR_NEGATIVE_MARKER_SIZE, GFN_SET_MARKER_SIZE_SCALE_FACTOR));
    Gfloat value{};
    ginqmarkersize(&value);
    ASSERT_EQ(1.0f, value);
}

TEST_F(TestGKSSetStateListErrorHandling, markerType)
{
    const Gint status = gsetmarkertype(0);

    ASSERT_TRUE(expectError(status, GERROR_MARKER_TYPE_ZERO, GFN_SET_MARKER_TYPE));
    Gint value{};
    ginqmarkertype(&value);
    ASSERT_EQ(GMK_STAR, value);
}

TEST_F(TestGKSSetStateListErrorHandling, normalizationTransformTooSmall)
{
    const Gint status = gselntran(-1);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_TRAN_NUM, GFN_SELECT_NORMALIZATION_TRANSFORMATION));
    Gint value{};
    ginqcurrntrannum(&value);
    ASSERT_EQ(0, value);
}

TEST_F(TestGKSSetStateListErrorHandling, normalizationTransformTooLarge)
{
    Gint maxTranNum{};
    ASSERT_EQ(GERROR_NONE, ginqmaxntrannum(&maxTranNum));

    const Gint status = gselntran(maxTranNum + 1);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_TRAN_NUM, GFN_SELECT_NORMALIZATION_TRANSFORMATION));
    Gint value{};
    ginqcurrntrannum(&value);
    ASSERT_EQ(0, value);
}

TEST_F(TestGKSSetStateListErrorHandling, textColor)
{
    const Gint status = gsettextcolorind(-1);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_COLOR_INDEX, GFN_SET_TEXT_COLOR_INDEX));
    Gint value{};
    ginqtextcolorind(&value);
    ASSERT_EQ(1, value);
}

TEST_F(TestGKSSetStateListErrorHandling, textFontAndPrecision)
{
    Gtxfp fontPrec{0, GP_STROKE};
    const Gint status = gsettextfontprec(&fontPrec);

    ASSERT_TRUE(expectError(status, GERROR_TEXT_FONT_ZERO, GFN_SET_TEXT_FONT_AND_PRECISION));
    Gtxfp value{};
    ginqtextfontprec(&value);
    ASSERT_EQ(1, value.font);
}

TEST_F(TestGKSSetStateListErrorHandling, textIndex)
{
    const Gint status = gsettextind(0);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_TEXT_INDEX, GFN_SET_TEXT_INDEX));
    Gint value{};
    ginqtextind(&value);
    ASSERT_EQ(1, value);
}

TEST_F(TestGKSSetStateListErrorHandling, viewportInvalidIndex)
{
    Glimit viewport{0.0f, 1.0f, 0.0f, 1.0f};
    const Gint status = gsetviewport(0, &viewport);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_TRAN_NUM, GFN_SET_VIEWPORT));
}

TEST_F(TestGKSSetStateListErrorHandling, invalidRectangleXRange)
{
    Glimit viewport{1.0f, 0.0f, 0.0f, 1.0f};
    const Gint status = gsetviewport(1, &viewport);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_RECT, GFN_SET_VIEWPORT));
}

TEST_F(TestGKSSetStateListErrorHandling, invalidRectangleYRange)
{
    Glimit viewport{0.0f, 1.0f, 1.0f, 0.0f};
    const Gint status = gsetviewport(1, &viewport);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_RECT, GFN_SET_VIEWPORT));
}

TEST_F(TestGKSSetStateListErrorHandling, viewportXMinOutOfRange)
{
    Glimit viewport{-1.0f, 1.0f, 0.0f, 1.0f};

    const Gint status = gsetviewport(1, &viewport);

    ASSERT_TRUE(expectError(status, GERROR_VIEWPORT_NOT_IN_NDC, GFN_SET_VIEWPORT));
}

TEST_F(TestGKSSetStateListErrorHandling, viewportXMaxOutOfRange)
{
    Glimit viewport{0.0f, 2.0f, 0.0f, 1.0f};

    const Gint status = gsetviewport(1, &viewport);

    ASSERT_TRUE(expectError(status, GERROR_VIEWPORT_NOT_IN_NDC, GFN_SET_VIEWPORT));
}

TEST_F(TestGKSSetStateListErrorHandling, viewportYMinOutOfRange)
{
    Glimit viewport{0.0f, 1.0f, -1.0f, 1.0f};

    const Gint status = gsetviewport(1, &viewport);

    ASSERT_TRUE(expectError(status, GERROR_VIEWPORT_NOT_IN_NDC, GFN_SET_VIEWPORT));
}

TEST_F(TestGKSSetStateListErrorHandling, viewportYMaxOutOfRange)
{
    Glimit viewport{0.0f, 1.0f, 0.0f, 2.0f};

    const Gint status = gsetviewport(1, &viewport);

    ASSERT_TRUE(expectError(status, GERROR_VIEWPORT_NOT_IN_NDC, GFN_SET_VIEWPORT));
}

TEST_F(TestGKSSetStateListErrorHandling, windowInvalidIndex)
{
    Glimit window{0.0f, 1.0f, 0.0f, 1.0f};
    const Gint status = gsetwindow(0, &window);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_TRAN_NUM, GFN_SET_WINDOW));
}

TEST_F(TestGKSSetStateListErrorHandling, windowInvalidXRange)
{
    Glimit window{1.0f, 0.0f, 0.0f, 1.0f};
    const Gint status = gsetwindow(1, &window);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_RECT, GFN_SET_WINDOW));
}

TEST_F(TestGKSSetStateListErrorHandling, windowInvalidYRange)
{
    Glimit window{0.0f, 1.0f, 1.0f, 0.0f};
    const Gint status = gsetwindow(1, &window);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_RECT, GFN_SET_WINDOW));
}

namespace
{

class TestWorkstationDependentStateList : public TestWSOpen
{
};

} // namespace

TEST_F(TestWorkstationDependentStateList, connectionIdAndType)
{
    Gwsct ct{};
    Gint numCt{};
    const Gint status = ginqwsconntype(wsId, 0, &numCt, &ct);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(connId, ct.conn);
    ASSERT_EQ(wsType, ct.type);
}

TEST_F(TestWorkstationDependentStateList, colorIndices)
{
    Gint colors[16]{-1};
    Gint numIndices{};
    Gintlist indices{numOf(colors), colors};
    const Gint status = ginqcolorindices(wsId, numOf(colors), 0, &indices, &numIndices);

    ASSERT_EQ(GERROR_NONE, status);
    ASSERT_EQ(2, numIndices);
    ASSERT_EQ(0, colors[0]);
    ASSERT_EQ(1, colors[1]);
}

namespace
{

class TestSetWorkstationDependentState : public TestWSOpen
{
};

} // namespace

TEST_F(TestSetWorkstationDependentState, colorRepresentation)
{
    Gcobundl rep{0.5f, 0.5f, 0.5f};
    const Gint status = gsetcolorrep(wsId, 0, &rep);

    ASSERT_EQ(GERROR_NONE, status);
    Gcobundl current{};
    ginqcolorrep(wsId, 0, &current);
    ASSERT_EQ(rep.red, current.red);
    ASSERT_EQ(rep.green, current.green);
    ASSERT_EQ(rep.blue, current.blue);
}

namespace
{

class TestSetWorkstationDependentStateErrorHandling : public TestWSOpen
{
};

} // namespace

TEST_F(TestSetWorkstationDependentStateErrorHandling, getColorIndices)
{
    Gint colors[16]{};
    Gint numIndices{};
    Gintlist indices{};
    const Gint status = ginqcolorindices(wsId + 1, numOf(colors), 0, &indices, &numIndices);

    ASSERT_TRUE(expectError(status, GERROR_WS_NOT_OPEN, GFN_INQUIRE_COLOR_INDICES));
}

TEST_F(TestSetWorkstationDependentStateErrorHandling, getColorRepresentationWorkstationNotOpen)
{
    Gint index{1};
    Gcobundl value{};

    const Gint status = ginqcolorrep(wsId + 1, index, &value);

    ASSERT_TRUE(expectError(status, GERROR_WS_NOT_OPEN, GFN_INQUIRE_COLOR_REPRESENTATION));
}

TEST_F(TestSetWorkstationDependentStateErrorHandling, getColorRepresentationInvalidIndex)
{
    Gcobundl value{};

    const Gint status = ginqcolorrep(wsId, -1, &value);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_COLOR_INDEX, GFN_INQUIRE_COLOR_REPRESENTATION));
}

TEST_F(TestSetWorkstationDependentStateErrorHandling, getConnectionAndType)
{
    Gwsct ct{};
    Gint buffSize{};
    Gint numCts{};
    const Gint status = ginqwsconntype(wsId + 1, buffSize, &numCts, &ct);

    ASSERT_TRUE(expectError(status, GERROR_WS_NOT_OPEN, GFN_INQUIRE_WORKSTATION_CONNECTION_AND_TYPE));
}

TEST_F(TestSetWorkstationDependentStateErrorHandling, getWorkstationTransformationWSNotOpen)
{
    Gwsti transform{};
    const Gint status = ginqwstran(wsId + 1, &transform);

    ASSERT_TRUE(expectError(status, GERROR_WS_NOT_OPEN, GFN_INQUIRE_WORKSTATION_TRANSFORMATION));
}

TEST_F(TestSetWorkstationDependentStateErrorHandling, setColorRepresentationBadIndex)
{
    Gcobundl rep{0.5f, 0.5f, 0.5f};
    const Gint status = gsetcolorrep(wsId, -1, &rep);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_COLOR_INDEX, GFN_SET_COLOR_REPRESENTATION));
    Gcobundl current{};
    ginqcolorrep(wsId, 0, &current);
    ASSERT_EQ(0.0f, current.red);
    ASSERT_EQ(0.0f, current.green);
    ASSERT_EQ(0.0f, current.blue);
}

TEST_F(TestSetWorkstationDependentStateErrorHandling, setViewportInvalidXRange)
{
    Glimit viewport{2.0f, 1.0f, 0.0f, 1.0f};

    const Gint status = gsetwsviewport(wsId, &viewport);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_RECT, GFN_SET_WORKSTATION_VIEWPORT));
}

TEST_F(TestSetWorkstationDependentStateErrorHandling, setViewportInvalidYRange)
{
    Glimit viewport{0.0f, 1.0f, 2.0f, 1.0f};

    const Gint status = gsetwsviewport(wsId, &viewport);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_RECT, GFN_SET_WORKSTATION_VIEWPORT));
}

TEST_F(TestSetWorkstationDependentStateErrorHandling, setWindowInvalidXRange)
{
    Glimit window{2.0f, 1.0f, 0.0f, 1.0f};

    const Gint status = gsetwswindow(wsId, &window);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_RECT, GFN_SET_WORKSTATION_WINDOW));
}

TEST_F(TestSetWorkstationDependentStateErrorHandling, setWindowInvalidYRange)
{
    Glimit window{2.0f, 1.0f, 2.0f, 1.0f};

    const Gint status = gsetwswindow(wsId, &window);

    ASSERT_TRUE(expectError(status, GERROR_INVALID_RECT, GFN_SET_WORKSTATION_WINDOW));
}
