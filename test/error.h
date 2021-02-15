#pragma once

#include <gks/gks.h>
#include <gks/gkserror.h>

#include <catch2/catch.hpp>

#include <vector>

struct Error
{
    Gint errNum;
    Gint function;
};

extern std::vector<Error> g_recordedErrors;

inline enum Gopst getGksOpState()
{
    Gopst result = static_cast<Gopst>(-1);
    ginqopst(&result);
    return result;
}

inline void requireError(GError error, GFunction fn)
{
    REQUIRE(g_recordedErrors.size() == 1u);
    REQUIRE(g_recordedErrors[0].errNum == error);
    REQUIRE(g_recordedErrors[0].function == fn);
}

template <typename T, size_t N>
Gint numOf(T (&ary)[N])
{
    return static_cast<Gint>(N);
}
