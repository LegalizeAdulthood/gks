#pragma once

#include <gks/gks.h>

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
