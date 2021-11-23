#pragma once

#include "gmock/gmock.h"

#include "mimax/nn/Activation.h"

namespace mimax_mock {
namespace nn {

class CActivationMock : public mimax::nn::IActivation
{
public:
    ~CActivationMock() override = default;

    MOCK_METHOD(void, Activate, (mimax::common::CMatrix& matrix), (override));
};

using CActivationNiceMock = testing::NiceMock<CActivationMock>;

} // nn
} // mimax_mock