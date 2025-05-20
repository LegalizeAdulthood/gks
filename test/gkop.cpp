#include "error.h"

#include <gks/gks.h>
#include <gks/gkserror.h>

#include <catch.hpp>

TEST_CASE("GKS open")
{
    g_recordedErrors.clear();
    gopengks(stderr, 0);

    SECTION("gopengks")
    {
        gopengks(stderr, 0);

        requireError(GERROR_NOT_STATE_GKCL, GFN_OPEN_GKS);
    }

    SECTION("gclosews")
    {
        Gint wsId{0};
        gclosews(wsId);

        requireError(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_CLOSE_WORKSTATION);
    }

    SECTION("gactivatews")
    {
        Gint wsId{0};
        gactivatews(wsId);

        requireError(GERROR_NOT_STATE_WSOP_WSAC, GFN_ACTIVATE_WORKSTATION);
    }
    SECTION("gclearws")
    {
        Gint wsId{0};
        gclearws(wsId, GALWAYS);

        requireError(GERROR_NOT_STATE_WSOP_WSAC, GFN_CLEAR_WORKSTATION);
    }
    SECTION("gdeactivatews")
    {
        Gint wsId{0};
        gdeactivatews(wsId);

        requireError(GERROR_NOT_STATE_WSAC, GFN_DEACTIVATE_WORKSTATION);
    }
    SECTION("gupdatews")
    {
        Gint wsId{0};
        gupdatews(wsId, GPERFORM);

        requireError(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_UPDATE_WORKSTATION);
    }

    SECTION("gcellarray")
    {
        struct Grect rect =
        {
            {0.25f, 0.75f},
            {0.75f, 0.25f}
        };
        struct Gidim dim =
        {
            128, 128
        };
        Gint colors[] = { 0, 1 };
        gcellarray(&rect, &dim, colors);

        requireError(GERROR_NOT_STATE_WSAC_SGOP, GFN_CELL_ARRAY);
    }
    SECTION("gfillarea")
    {
        Gpoint points[] = {{0.0f, 0.0f}, {1.0f, 1.0f}};
        gfillarea(numOf(points), points);

        requireError(GERROR_NOT_STATE_WSAC_SGOP, GFN_FILL_AREA);
    }
    SECTION("ggdp")
    {
        ggdp(0, nullptr, 0, nullptr);

        requireError(GERROR_NOT_STATE_WSAC_SGOP, GFN_GDP);
    }
    SECTION("gpolyline")
    {
        Gpoint points[] = {{0.0f, 0.0f}, {1.0f, 1.0f}};
        gpolyline(numOf(points), points);

        requireError(GERROR_NOT_STATE_WSAC_SGOP, GFN_POLYLINE);
    }
    SECTION("gpolymarker")
    {
        Gpoint points[] = {{0.0f, 0.0f}, {1.0f, 1.0f}};
        gpolymarker(numOf(points), points);

        requireError(GERROR_NOT_STATE_WSAC_SGOP, GFN_POLYMARKER);
    }
    SECTION("gtext")
    {
        Gpoint pos{0.1f, 0.1f};
        gtext(&pos, "Hello, world!");

        requireError(GERROR_NOT_STATE_WSAC_SGOP, GFN_TEXT);
    }

    SECTION("inquire")
    {
        Gint wsId{0};
        Gint status{};

        SECTION("ginqcolorindices")
        {
            Gint index{1};
            Gcobundl value{};
            ginqcolorrep(wsId, index, &value, &status);

            REQUIRE(status == GERROR_NOT_STATE_WSOP_WSAC_SGOP);
        }
        SECTION("ginqcolorrep")
        {
            Gint index{1};
            Gcobundl value{};
            ginqcolorrep(wsId, index, &value, &status);

            REQUIRE(status == GERROR_NOT_STATE_WSOP_WSAC_SGOP);
        }
        SECTION("ginqtextextent")
        {
            Gpoint pos{};
            Gextent value{};
            ginqtextextent(wsId, &pos, "Hello, world!", &value, &status);

            REQUIRE(status == GERROR_NOT_STATE_WSOP_WSAC_SGOP);
        }
        SECTION("ginqwsconntype")
        {
            Gwsct ct{};
            Gint buffSize{};
            Gint numCts{};
            ginqwsconntype(wsId + 1, buffSize, &numCts, &ct, &status);

            REQUIRE(status == GERROR_NOT_STATE_WSOP_WSAC_SGOP);
        }
        SECTION("ginqwstran")
        {
            struct Gwsti transform{};
            ginqwstran(wsId, &transform, &status);

            REQUIRE(status == GERROR_NOT_STATE_WSOP_WSAC_SGOP);
        }
    }

    SECTION("gsetcolorrep")
    {
        Gint wsId{0};
        Gint index{1};
        Gcobundl value{};
        gsetcolorrep(wsId, index, &value);

        requireError(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_SET_COLOR_REPRESENTATION);
    }
    SECTION("gsetwswindow")
    {
        Gint wsId{0};
        Glimit window{0.0f, 1.0f, 0.0f, 1.0f};
        gsetwswindow(wsId, &window);

        requireError(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_SET_WORKSTATION_WINDOW);
    }
    SECTION("gsetwsviewport")
    {
        Gint wsId{0};
        Glimit viewport{};
        gsetwsviewport(wsId, &viewport);

        requireError(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_SET_WORKSTATION_VIEWPORT);
    }

    REQUIRE(getGksOpState() == GGKOP);

    gclosegks();
}
