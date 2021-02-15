#include "error.h"

#include <gks/gks.h>
#include <gks/gkserror.h>

#include <catch2/catch.hpp>

TEST_CASE("workstation open")
{
    g_recordedErrors.clear();
    gopengks(stderr, 0);
    Gint wsId{0};
    const Gchar *connId{"tek4105"};
    Gint wsType{GWSTYPE_TEK4105};
    gopenws(wsId, connId, wsType);

    SECTION("gopengks")
    {
        gopengks(stderr, 0);

        requireError(GERROR_NOT_STATE_GKCL, GFN_OPEN_GKS);
    }
    SECTION("gclosews")
    {
        gclosegks();

        requireError(GERROR_NOT_STATE_GKOP, GFN_CLOSE_GKS);
    }
    SECTION("gdeactivatews")
    {
        Gint wsId{0};
        gdeactivatews(wsId);

        requireError(GERROR_NOT_STATE_WSAC, GFN_DEACTIVATE_WORKSTATION);
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

    gclosews(wsId);
    gclosegks();
}
