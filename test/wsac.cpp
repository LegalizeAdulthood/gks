#include "error.h"

#include <gks/gks.h>
#include <gks/gkserror.h>

#include <catch2/catch.hpp>

TEST_CASE("workstation active")
{
    g_recordedErrors.clear();
    gopengks(stderr, 0);
    Gint wsId{};
    const Gchar *connId{"tek4105"};
    Gint wsType{GWSTYPE_TEK4105};
    gopenws(wsId, connId, wsType);
    gactivatews(wsId);
    REQUIRE(getGksOpState() == GWSAC);

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

    REQUIRE(getGksOpState() == GWSAC);

    gdeactivatews(wsId);
    gclosews(wsId);
    gclosegks();
}
