#pragma once

#include <gks/gks.h>
#include <gks/gkserror.h>

#include <gtest/gtest.h>

#include <vector>

struct Error
{
    GError errNum;
    GFunction function;
};

extern std::vector<Error> g_recordedErrors;

inline enum Gopst getGksOpState()
{
    Gopst result = static_cast<Gopst>(-1);
    ginqopst(&result);
    return result;
}

template <typename T, size_t N>
Gint numOf(T (&ary)[N])
{
    return static_cast<Gint>(N);
}

inline testing::AssertionResult expectError(Gint status, GError errNum, GFunction function)
{
    if (status != errNum)
    {
        return testing::AssertionFailure() << "expected return value " << errNum << ", got " << status;
    }
    if (g_recordedErrors.size() != 1U)
    {
        return testing::AssertionFailure() << "expected one error, got " << g_recordedErrors.size();
    }
    if (g_recordedErrors[0].errNum != errNum)
    {
        return testing::AssertionFailure() << "expected error " << errNum << ", got " << g_recordedErrors[0].errNum;
    }
    if (g_recordedErrors[0].function != function)
    {
        return testing::AssertionFailure() << "expected function " << function << ", got " << g_recordedErrors[0].function;
    }
    return testing::AssertionSuccess();
}
