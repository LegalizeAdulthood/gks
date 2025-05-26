#pragma once

#include <gks/gks.h>

#include <gtest/gtest.h>

#include <string>

class TestWSOpen : public testing::Test
{
public:
    ~TestWSOpen() override = default;

protected:
    void SetUp() override;
    void TearDown() override;

    Gint wsId{0};
    std::string connId{"tek4105"};
    Gint wsType{GWSTYPE_TEK4105};
};
