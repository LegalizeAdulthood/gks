#pragma once

#include <gtest/gtest.h>

class TestGKSOpen : public testing::Test
{
public:
    ~TestGKSOpen() override = default;

protected:
    void SetUp() override;
    void TearDown() override;
};
