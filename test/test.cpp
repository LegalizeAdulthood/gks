#include <gks/gks.h>

#include <catch2/catch.hpp>

#include <cstdio>

TEST_CASE("Open and close", "[gks]")
{
    gopengks(stderr, 0L);
    gclosegks();
}
