#include "error.h"

#include <gks/gks.h>
#include <gks/gkserror.h>

#include <catch2/catch.hpp>

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
        Gint wsId{1};
        gclosews(wsId);

        requireError(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_CLOSE_WORKSTATION);
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
    SECTION("gsetcolorrep")
    {
        gsetcolorrep(0, 0, nullptr);

        requireError(GERROR_NOT_STATE_WSOP_WSAC_SGOP, GFN_SET_COLOR_REPRESENTATION);
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

    REQUIRE(getGksOpState() == GGKOP);

    gclosegks();
}
