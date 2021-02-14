#include "error.h"

#include <gks/gks.h>
#include <gks/gkserror.h>

#include <catch2/catch.hpp>

TEST_CASE("GKS closed")
{
    g_recordedErrors.clear();
    REQUIRE(getGksOpState() == GGKCL);

    SECTION("gksclose")
    {
        gclosegks();

        REQUIRE(g_recordedErrors.size() == 1u);
        REQUIRE(g_recordedErrors[0].errNum == GERROR_NOT_STATE_GKOP);
        REQUIRE(g_recordedErrors[0].function == GFN_CLOSEGKS);
    }
}
