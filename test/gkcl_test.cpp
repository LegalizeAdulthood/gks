#include "error.h"

#include <gks/gks.h>
#include <gks/gkserror.h>

#include <gtest/gtest.h>

using namespace testing;

namespace
{

AssertionResult expectGksOpen(Gint status, GFunction fn)
{
    return expectError(status, GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, fn);
}

class TestGKSClosed : public Test
{
public:
    ~TestGKSClosed() override = default;

protected:
    void SetUp() override;
    void TearDown() override;
};

void TestGKSClosed::SetUp()
{
    Test::SetUp();
    g_recordedErrors.clear();
    ASSERT_EQ(GGKCL, getGksOpState());
}

void TestGKSClosed::TearDown()
{
    ASSERT_EQ(GGKCL, getGksOpState());

    Test::TearDown();
}

} // namespace

TEST_F(TestGKSClosed, closeGKS)
{
    const Gint status = gclosegks();

    ASSERT_TRUE(expectError(status, GERROR_NOT_STATE_GKOP, GFN_CLOSE_GKS));
}

TEST_F(TestGKSClosed, inquireAspectSourceFlag)
{
    Gasfs value{};

    const Gint status = ginqasf(&value);

    ASSERT_TRUE(expectError(status, GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_INQUIRE_ASPECT_SOURCE_FLAGS));
}

TEST_F(TestGKSClosed, inquireAvailableWorkstationTypes)
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
}

TEST_F(TestGKSClosed, inquireCharacterExpansion)
{
    Gfloat value{};

    const Gint status = ginqcharexpan(&value);
}

TEST_F(TestGKSClosed, inquireCharacterBase)
{
    Gpoint value{};

    const Gint status = ginqcharbase(&value);
}

TEST_F(TestGKSClosed, inquireCharacterHeight)
{
    Gfloat value{};

    const Gint status = ginqcharheight(&value);
}

TEST_F(TestGKSClosed, inquireCharacterSpacing)
{
    Gfloat value{};

    const Gint status = ginqcharspace(&value);
}

TEST_F(TestGKSClosed, inquireCharacterWidth)
{
    Gfloat value{};

    const Gint status = ginqcharwidth(&value);
}

TEST_F(TestGKSClosed, inquireCharacterUpVector)
{
    Gpoint value{};

    const Gint status = ginqcharup(&value);
}

TEST_F(TestGKSClosed, inquireClipRectangle)
{
    Gcliprect clipping{};

    const Gint status = ginqclip(&clipping);
}

TEST_F(TestGKSClosed, inquireCurrentTransform)
{
    Gint transform{-1};

    const Gint status = ginqcurrntrannum(&transform);
}

TEST_F(TestGKSClosed, inquierFillColorIndex)
{
    Gint value{};

    const Gint status = ginqfillcolorind(&value);
}

TEST_F(TestGKSClosed, inquireFillIndex)
{
    Gint value{};

    const Gint status = ginqfillind(&value);
}

TEST_F(TestGKSClosed, inquireFillStyle)
{
    Gflinter value{};

    const Gint status = ginqfillstyle(&value);
}

TEST_F(TestGKSClosed, inquireFillStyleIndex)
{
    Gint value{};

    const Gint status = ginqfillstyleind(&value);
}

TEST_F(TestGKSClosed, inquireGKSLevel)
{
    Glevel level{};

    const Gint status = ginqlevelgks(&level);
}

TEST_F(TestGKSClosed, inquireLineColorIndex)
{
    Gint value{};

    const Gint status = ginqlinecolorind(&value);
}

TEST_F(TestGKSClosed, inquireLineIndex)
{
    Gint value{};

    const Gint status = ginqlineind(&value);
}

TEST_F(TestGKSClosed, inquireLineType)
{
    Gint value{};

    const Gint status = ginqlinetype(&value);
}

TEST_F(TestGKSClosed, inuireMarkerColorIndex)
{
    Gint value{};

    const Gint status = ginqmarkercolorind(&value);
}

TEST_F(TestGKSClosed, inquireMarkerIndex)
{
    Gint value{};

    const Gint status = ginqmarkerind(&value);
}

TEST_F(TestGKSClosed, inquireMarkerSize)
{
    Gfloat value{};

    const Gint status = ginqmarkersize(&value);
}

TEST_F(TestGKSClosed, inquireMarkerType)
{
    Gint value{};

    const Gint status = ginqmarkertype(&value);
}

TEST_F(TestGKSClosed, inquireMaximumTransformNum)
{
    Gint value{};

    const Gint status = ginqmaxntrannum(&value);
}

TEST_F(TestGKSClosed, inquireNumTransform)
{
    Gtran value{};

    const Gint status = ginqntran(0, &value);
}

TEST_F(TestGKSClosed, inquireOpenWorkstation)
{
    std::vector<Gint> ids(10);
    Gintlist idList{static_cast<Gint>(ids.size()), ids.data()};
    Gint start{0};
    Gint numIds{0};

    const Gint status = ginqopenws(idList.number, start, &idList, &numIds);
}

TEST_F(TestGKSClosed, inquirePatternSize)
{
    Gpoint value{};

    const Gint status = ginqpatheight(&value);
}

TEST_F(TestGKSClosed, inquirePatternRefPoint)
{
    Gpoint refPt{};

    const Gint status = ginqpatrefpt(&refPt);
}

TEST_F(TestGKSClosed, inquireTextAlignment)
{
    Gtxalign value{};

    const Gint status = ginqtextalign(&value);
}

TEST_F(TestGKSClosed, inquireTextColorIndex)
{
    Gint value{};

    const Gint status = ginqtextcolorind(&value);
}

TEST_F(TestGKSClosed, inqurieTextFontPrecision)
{
    Gtxfp value{};

    const Gint status = ginqtextfontprec(&value);
}

TEST_F(TestGKSClosed, inquireTextIndex)
{
    Gint value{};

    const Gint status = ginqtextind(&value);
}

TEST_F(TestGKSClosed, inquireColorFacilities)
{
    Gcofac facil{};

    const Gint status = ginqcolorfacil(GWSTYPE_TEK4105, 0, NULL, &facil);
}

TEST_F(TestGKSClosed, inquireDisplaySize)
{
    Gdspsize value{};

    const Gint status = ginqdisplaysize(GWSTYPE_TEK4105, &value);
}

TEST_F(TestGKSClosed, inuireFillFacilities)
{
    Gflinter interiorStyles[4]{};
    Gint hatchStyles[10]{};
    Gflfac facil{};
    facil.interiors = interiorStyles;
    facil.hatches = hatchStyles;
    Gint numAvailHatches{};

    const Gint status = ginqfillfacil(GWSTYPE_TEK4105, numOf(hatchStyles), &numAvailHatches, &facil);
}

TEST_F(TestGKSClosed, inquireLineFacilities)
{
    Gint lineTypes[10]{};
    Gint numLineTypes{};
    Glnfac value{};

    const Gint status = ginqlinefacil(GWSTYPE_TEK4105, numOf(lineTypes), &numLineTypes, &value);
}

TEST_F(TestGKSClosed, inquireMarkerFacilities)
{
    Gint lineTypes[10]{};
    Gint numLineTypes{};
    Glnfac value{};

    const Gint status = ginqlinefacil(GWSTYPE_TEK4105, numOf(lineTypes), &numLineTypes, &value);
}

TEST_F(TestGKSClosed, inquireMaxWorkstationTables)
{
    Gwstables tables{-1, -1, -1, -1, -1, -1};

    const Gint status = ginqmaxwssttables(GWSTYPE_TEK4105, &tables);
}

TEST_F(TestGKSClosed, inquireWorkstationMaxNum)
{
    Gwsmax value{};

    const Gint status = ginqwsmaxnum(&value);
}


TEST_F(TestGKSClosed, inquireSelectNormalizationTransform)
{
    const Gint status = gselntran(0);

    ASSERT_TRUE(expectGksOpen(status, GFN_SELECT_NORMALIZATION_TRANSFORMATION));
}

TEST_F(TestGKSClosed, setAspectSourceFlags)
{
    Gasfs value{};

    const Gint status = gsetasf(&value);

    ASSERT_TRUE(expectGksOpen(status, GFN_SET_ASPECT_SOURCE_FLAGS));
}

TEST_F(TestGKSClosed, setCharacterExpansion)
{
    const Gint status = gsetcharexpan(1.0f);

    ASSERT_TRUE(expectGksOpen(status, GFN_SET_CHARACTER_EXPANSION_FACTOR));
}

TEST_F(TestGKSClosed, setCharacterHeight)
{
    const Gint status = gsetcharheight(0.02f);

    ASSERT_TRUE(expectGksOpen(status, GFN_SET_CHARACTER_HEIGHT));
}

TEST_F(TestGKSClosed, setCharacterSpacing)
{
    const Gint status = gsetcharspace(1.0f);

    ASSERT_TRUE(expectGksOpen(status, GFN_SET_CHARACTER_SPACING));
}

TEST_F(TestGKSClosed, setCharacterUpVector)
{
    Gpoint up{0.0f, 1.0f};

    const Gint status = gsetcharup(&up);

    ASSERT_TRUE(expectGksOpen(status, GFN_SET_CHARACTER_UP_VECTOR));
}

TEST_F(TestGKSClosed, setClippingIndicator)
{
    const Gint status = gsetclip(GCLIP);

    ASSERT_TRUE(expectGksOpen(status, GFN_SET_CLIPPING_INDICATOR));
}

TEST_F(TestGKSClosed, setFillAreaColorIndex)
{
    const Gint status = gsetfillcolorind(0);

    ASSERT_TRUE(expectGksOpen(status, GFN_SET_FILL_AREA_COLOR_INDEX));
}

TEST_F(TestGKSClosed, setFillIndex)
{
    const Gint status = gsetfillind(1);

    ASSERT_TRUE(expectGksOpen(status, GFN_SET_FILL_AREA_INDEX));
}

TEST_F(TestGKSClosed, setFillStyle)
{
    const Gint status = gsetfillstyle(GHOLLOW);

    ASSERT_TRUE(expectGksOpen(status, GFN_SET_FILL_AREA_INTERIOR_STYLE));
}

TEST_F(TestGKSClosed, setFillStyleIndex)
{
    const Gint status = gsetfillstyleind(1);

    ASSERT_TRUE(expectGksOpen(status, GFN_SET_FILL_AREA_INTERIOR_STYLE));
}

TEST_F(TestGKSClosed, setLineColorIndex)
{
    const Gint status = gsetlinecolorind(0);

    ASSERT_TRUE(expectGksOpen(status, GFN_SET_POLYLINE_COLOR_INDEX));
}

TEST_F(TestGKSClosed, setLineIndex)
{
    const Gint status = gsetlineind(1);

    ASSERT_TRUE(expectGksOpen(status, GFN_SET_POLYLINE_INDEX));
}

TEST_F(TestGKSClosed, setLineType)
{
    const Gint status = gsetlinetype(1);

    ASSERT_TRUE(expectGksOpen(status, GFN_SET_LINETYPE));
}

TEST_F(TestGKSClosed, setMarkerColorIndex)
{
    const Gint status = gsetmarkercolorind(0);

    ASSERT_TRUE(expectGksOpen(status, GFN_SET_POLYMARKER_COLOR_INDEX));
}

TEST_F(TestGKSClosed, setMarkerIndex)
{
    const Gint status = gsetmarkerind(1);

    ASSERT_TRUE(expectGksOpen(status, GFN_SET_POLYMARKER_INDEX));
}

TEST_F(TestGKSClosed, setMarkerSize)
{
    const Gint status = gsetmarkersize(1.0f);

    ASSERT_TRUE(expectGksOpen(status, GFN_SET_MARKER_SIZE_SCALE_FACTOR));
}

TEST_F(TestGKSClosed, setMarkerType)
{
    const Gint status = gsetmarkertype(GMK_DIAGONAL_CROSS);

    ASSERT_TRUE(expectGksOpen(status, GFN_SET_MARKER_TYPE));
}

TEST_F(TestGKSClosed, setTextAlignment)
{
    const Gint status = gsettextalign(nullptr);

    ASSERT_TRUE(expectGksOpen(status, GFN_SET_TEXT_ALIGNMENT));
}

TEST_F(TestGKSClosed, setTExtColorIndex)
{
    const Gint status = gsettextcolorind(0);

    ASSERT_TRUE(expectGksOpen(status, GFN_SET_TEXT_COLOR_INDEX));
}

TEST_F(TestGKSClosed, setTextFontPrecision)
{
    Gtxfp value{1, GP_STRING};

    const Gint status = gsettextfontprec(&value);

    ASSERT_TRUE(expectGksOpen(status, GFN_SET_TEXT_FONT_AND_PRECISION));
}

TEST_F(TestGKSClosed, setTextIndex)
{
    const Gint status = gsettextind(1);

    ASSERT_TRUE(expectGksOpen(status, GFN_SET_TEXT_INDEX));
}

TEST_F(TestGKSClosed, setViewport)
{
    Glimit viewport{0.0f, 1.0f, 0.0f, 1.0f};

    const Gint status = gsetviewport(1, &viewport);

    ASSERT_TRUE(expectGksOpen(status, GFN_SET_VIEWPORT));
}

TEST_F(TestGKSClosed, setWindow)
{
    Glimit window{0.0f, 1.0f, 0.0f, 1.0f};

    const Gint status = gsetwindow(1, &window);

    ASSERT_TRUE(expectGksOpen(status, GFN_SET_WINDOW));
}

TEST_F(TestGKSClosed, openWorkstation)
{
    Gint wsId{0};
    const Gchar *connId{"tek4105"};
    Gint wsType{GWSTYPE_TEK4105};

    const Gint status = gopenws(wsId, connId, wsType);

    ASSERT_TRUE(expectGksOpen(status, GFN_OPEN_WORKSTATION));
}

TEST_F(TestGKSClosed, closeWorkstation)
{
    Gint wsId{0};

    const Gint status = gclosews(wsId);

    ASSERT_TRUE(expectError(status, GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_CLOSE_WORKSTATION));
}

TEST_F(TestGKSClosed, setWorkstationViewport)
{
    const Gint status = gsetwsviewport(0, nullptr);

    ASSERT_TRUE(expectError(status, GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_SET_WORKSTATION_VIEWPORT));
}

TEST_F(TestGKSClosed, setWorkstationWindow)
{
    Glimit value{0.0f, 1.0f, 0.0f, 1.0f};

    const Gint status = gsetwswindow(0, &value);

    ASSERT_TRUE(expectError(status, GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_SET_WORKSTATION_WINDOW));
}

TEST_F(TestGKSClosed, activateWorkstation)
{
    Gint wsId{0};

    const Gint status = gactivatews(wsId);

    ASSERT_TRUE(expectError(status, GERROR_NOT_STATE_WSOP_WSAC, GFN_ACTIVATE_WORKSTATION));
}

TEST_F(TestGKSClosed, clearWorkstation)
{
    Gint wsId{0};

    const Gint status = gclearws(wsId, GALWAYS);

    ASSERT_TRUE(expectError(status, GERROR_NOT_STATE_WSOP_WSAC, GFN_CLEAR_WORKSTATION));
}

TEST_F(TestGKSClosed, deactivateWorkstation)
{
    Gint wsId{0};

    const Gint status = gdeactivatews(wsId);

    ASSERT_TRUE(expectError(status, GERROR_NOT_STATE_WSAC, GFN_DEACTIVATE_WORKSTATION));
}

TEST_F(TestGKSClosed, escape)
{
    const Gint status = gescape(0, nullptr, 0, nullptr, nullptr);

    ASSERT_TRUE(expectGksOpen(status, GFN_ESCAPE));
}

TEST_F(TestGKSClosed, updateWorkstation)
{
    Gint wsId{0};

    const Gint status = gupdatews(wsId, GPERFORM);

    ASSERT_TRUE(expectError(status, GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_UPDATE_WORKSTATION));
}

TEST_F(TestGKSClosed, inquireColorIndices)
{
    const Gint wsId{0};
    Gint values[1];
    Gintlist indices{sizeof(values), values};
    Gint numIndices{};

    const Gint status = ginqcolorindices(wsId, sizeof(values), 0, &indices, &numIndices);

    ASSERT_TRUE(expectError(status, GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_INQUIRE_COLOR_INDICES));
}

TEST_F(TestGKSClosed, inquireColorRepresentation)
{
    const Gint wsId{0};
    Gint index{1};
    Gcobundl value{};

    const Gint status = ginqcolorrep(wsId, index, &value);

    ASSERT_TRUE(expectError(status, GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_INQUIRE_COLOR_REPRESENTATION));
}

TEST_F(TestGKSClosed, inquireTextExtent)
{
    const Gint wsId{0};
    Gpoint pos{};
    Gextent value{};

    const Gint status = ginqtextextent(wsId, &pos, "Hello, world!", &value);

    ASSERT_TRUE(expectError(status, GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_INQUIRE_TEXT_EXTENT));
}

TEST_F(TestGKSClosed, inquireWorkstationConnectionType)
{
    const Gint wsId{0};
    Gwsct ct{};
    Gint buffSize{};
    Gint numCts{};

    const Gint status = ginqwsconntype(wsId + 1, buffSize, &numCts, &ct);

    ASSERT_TRUE(expectError(status, GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_INQUIRE_WORKSTATION_CONNECTION_AND_TYPE));
}

TEST_F(TestGKSClosed, inquireWorkstationTransform)
{
    const Gint wsId{0};
    Gwsti transform{};

    const Gint status = ginqwstran(wsId, &transform);

    ASSERT_TRUE(expectError(status, GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_INQUIRE_WORKSTATION_TRANSFORMATION));
}

TEST_F(TestGKSClosed, setColorRepresentation)
{
    const Gint wsId{0};
    Gint index{1};
    Gcobundl color{1.0f, 1.0f, 1.0f};

    const Gint status = gsetcolorrep(wsId, index, &color);

    ASSERT_TRUE(expectError(status, GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_SET_COLOR_REPRESENTATION));
}

TEST_F(TestGKSClosed, cellArray)
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

TEST_F(TestGKSClosed, fillArea)
{
    const Gint status = gfillarea(0, nullptr);

    ASSERT_TRUE(expectError(status, GERROR_NOT_STATE_WSAC_SGOP, GFN_FILL_AREA));
}

TEST_F(TestGKSClosed, generalizedDrawingPrimitive)
{
    const Gint status = ggdp(0, nullptr, 0, nullptr);

    ASSERT_TRUE(expectError(status, GERROR_NOT_STATE_WSAC_SGOP, GFN_GDP));
}

TEST_F(TestGKSClosed, polyline)
{
    const Gint status = gpolyline(0, nullptr);

    ASSERT_TRUE(expectError(status, GERROR_NOT_STATE_WSAC_SGOP, GFN_POLYLINE));
}

TEST_F(TestGKSClosed, polymarker)
{
    const Gint status = gpolymarker(0, nullptr);

    ASSERT_TRUE(expectError(status, GERROR_NOT_STATE_WSAC_SGOP, GFN_POLYMARKER));
}

TEST_F(TestGKSClosed, text)
{
    const Gint status = gtext(nullptr, nullptr);

    ASSERT_TRUE(expectError(status, GERROR_NOT_STATE_WSAC_SGOP, GFN_TEXT));
}
