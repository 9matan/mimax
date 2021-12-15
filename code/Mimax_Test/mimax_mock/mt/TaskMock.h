#pragma once

#include "gmock/gmock.h"

#include "mimax/mt/Task.h"

namespace mimax_mock {
namespace mt {

class CTaskMock : public mimax::mt::ITask
{
public:
    ~CTaskMock() override = default;

    MOCK_METHOD(void, RunTask, (), (override));
    MOCK_METHOD(void, StopTask, (), (override));
};

using CTaskNiceMock = testing::NiceMock<CTaskMock>;

} // mt
} // mimax_mock