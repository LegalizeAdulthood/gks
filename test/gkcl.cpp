#include "error.h"

#include <gks/gks.h>
#include <gks/gkserror.h>

#include <catch2/catch.hpp>

TEST_CASE("GKS closed", "[errors]")
{
    g_recordedErrors.clear();
    REQUIRE(getGksOpState() == GGKCL);

    SECTION("gclosegks")
    {
        gclosegks();

        requireError(GERROR_NOT_STATE_GKOP, GFN_CLOSE_GKS);
    }
    SECTION("gopenws")
    {
        Gint wsId{1};
        const Gchar *connId{"tek4105"};
        Gint wsType{};
        gopenws(wsId, connId, wsType);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_OPEN_WORKSTATION);
    }
    SECTION("gclosews")
    {
        Gint wsId{1};
        gclosews(wsId);

        requireError(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_CLOSE_WORKSTATION);
    }
    SECTION("ginqasf")
    {
        Gasfs value{};
        Gint status{};
        ginqasf(&value, &status);

        REQUIRE(status == GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP);
    }
    SECTION("ginqavailwstypes")
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
        Gint status{};
        ginqavailwstypes(numWsTypes, start, &wsTypes, &numAvailTypes, &status);

        REQUIRE(status == GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP);
    }
    SECTION("ginqcharexpan")
    {
        Gfloat value{};
        Gint status{};
        ginqcharexpan(&value, &status);

        REQUIRE(status == GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP);
    }
    SECTION("ginqcharbase")
    {
        Gpoint value{};
        Gint status{};
        ginqcharbase(&value, &status);

        REQUIRE(status == GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP);
    }
    SECTION("ginqcharheight")
    {
        Gfloat value{};
        Gint status{};
        ginqcharheight(&value, &status);

        REQUIRE(status == GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP);
    }
    SECTION("ginqcharspace")
    {
        Gfloat value{};
        Gint status{};
        ginqcharspace(&value, &status);

        REQUIRE(status == GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP);
    }
    SECTION("ginqcharwidth")
    {
        Gfloat value{};
        Gint status{};
        ginqcharwidth(&value, &status);

        REQUIRE(status == GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP);
    }
    SECTION("ginqcharup")
    {
        Gpoint value{};
        Gint status{};
        ginqcharup(&value, &status);

        REQUIRE(status == GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP);
    }
    SECTION("gactivatews")
    {
        Gint wsId{1};
        gactivatews(wsId);

        requireError(GERROR_NOT_STATE_WSOP_WSAC, GFN_ACTIVATE_WORKSTATION);
    }
    SECTION("gdeactivatews")
    {
        Gint wsId{1};
        gdeactivatews(wsId);

        requireError(GERROR_NOT_STATE_WSAC, GFN_DEACTIVATE_WORKSTATION);
    }
    SECTION("gclearws")
    {
        Gint wsId{1};
        gclearws(wsId, GALWAYS);

        requireError(GERROR_NOT_STATE_WSOP_WSAC, GFN_CLEAR_WORKSTATION);
    }
    SECTION("gupdatews")
    {
        Gint wsId{1};
        gupdatews(wsId, GPERFORM);

        requireError(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_UPDATE_WORKSTATION);
    }
    SECTION("gescape")
    {
        gescape(0, nullptr, 0, nullptr, nullptr);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_ESCAPE);
    }
    SECTION("gpolyline")
    {
        gpolyline(0, nullptr);

        requireError(GERROR_NOT_STATE_WSAC_SGOP, GFN_POLYLINE);
    }
    SECTION("gpolymarker")
    {
        gpolymarker(0, nullptr);

        requireError(GERROR_NOT_STATE_WSAC_SGOP, GFN_POLYMARKER);
    }
    SECTION("gtext")
    {
        gtext(nullptr, nullptr);

        requireError(GERROR_NOT_STATE_WSAC_SGOP, GFN_TEXT);
    }
    SECTION("gfillarea")
    {
        gfillarea(0, nullptr);

        requireError(GERROR_NOT_STATE_WSAC_SGOP, GFN_FILL_AREA);
    }
    SECTION("ggdp")
    {
        ggdp(0, nullptr, 0, nullptr);

        requireError(GERROR_NOT_STATE_WSAC_SGOP, GFN_GDP);
    }
    SECTION("gsetlinetype")
    {
        gsetlinetype(0);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_LINETYPE);
    }
    SECTION("gsetlinecolorind")
    {
        gsetlinecolorind(0);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_POLYLINE_COLOR_INDEX);
    }
    SECTION("gsetmarkertype")
    {
        gsetmarkertype(0);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_MARKER_TYPE);
    }
    SECTION("gsetmarkercolorind")
    {
        gsetmarkercolorind(0);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_POLYMARKER_COLOR_INDEX);
    }
    SECTION("gsettextcolorind")
    {
        gsettextcolorind(0);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_TEXT_COLOR_INDEX);
    }
    SECTION("gsetcharheight")
    {
        gsetcharheight(0.0f);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_CHARACTER_HEIGHT);
    }
    SECTION("gsetcharheight")
    {
        gsetcharup(nullptr);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_CHARACTER_UP_VECTOR);
    }
    SECTION("gsettextalign")
    {
        gsettextalign(nullptr);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_TEXT_ALIGNMENT);
    }
    SECTION("gsetfillstyle")
    {
        gsetfillstyle(GHOLLOW);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_FILL_AREA_INTERIOR_STYLE);
    }
    SECTION("gsetfillcolorind")
    {
        gsetfillcolorind(0);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_FILL_AREA_COLOR_INDEX);
    }
    SECTION("gsetcolorrep")
    {
        gsetcolorrep(0, 0, nullptr);

        requireError(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_SET_COLOR_REPRESENTATION);
    }
    SECTION("gsetwindow")
    {
        gsetwindow(0, nullptr);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_WINDOW);
    }
    SECTION("gsetviewport")
    {
        gsetviewport(0, nullptr);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SET_VIEWPORT);
    }
    SECTION("gselntran")
    {
        gselntran(0);

        requireError(GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP, GFN_SELECT_NORMALIZATION_TRANSFORMATION);
    }
    SECTION("gsetwswindow")
    {
        gsetwswindow(0, nullptr);

        requireError(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_SET_WORKSTATION_WINDOW);
    }
    SECTION("gsetwsviewport")
    {
        gsetwsviewport(0, nullptr);

        requireError(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_SET_WORKSTATION_VIEWPORT);
    }
    SECTION("ginqlevelgks")
    {
        Glevel level{};
        Gint status{-1};
        ginqlevelgks(&level, &status);

        REQUIRE(status == GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP);
    }
    SECTION("ginqwsmaxnum")
    {
        Gwsmax value{};
        Gint status{};
        ginqwsmaxnum(&value, &status);

        REQUIRE(status == GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP);
    }
    SECTION("ginqmaxntrannum")
    {
        Gint value{};
        Gint status{};
        ginqmaxntrannum(&value, &status);

        REQUIRE(status == GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP);
    }
    SECTION("ginqlineind")
    {
        Gint value{};
        Gint status{};
        ginqlineind(&value, &status);

        REQUIRE(status == GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP);
    }
    SECTION("ginqmarkerind")
    {
        Gint value{};
        Gint status{};
        ginqmarkerind(&value, &status);

        REQUIRE(status == GERROR_NOT_STATE_GKOP_WSOP_WSAC_SGOP);
    }

    REQUIRE(getGksOpState() == GGKCL);
}
