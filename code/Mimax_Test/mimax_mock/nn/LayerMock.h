#pragma once

#include "gmock/gmock.h"

#include "mimax/nn/Layer.h"

namespace mimax_mock {
namespace nn {

class CLayerMock : public mimax::nn::ILayer
{
public:
    ~CLayerMock() override = default;

    MOCK_METHOD(void, Activate, (mimax::common::CMatrix const& input, mimax::common::CMatrix& output), (override));
};

using CLayerNiceMock = testing::NiceMock<CLayerMock>;

} // nn
} // mimax_mock