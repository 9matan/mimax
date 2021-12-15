#include "gtest/gtest.h"

#include "mimax/mt/TasksRunner.h"

#include "mimax_mock/mt/TaskMock.h"

namespace mimax_test {
namespace mt {
namespace tasks_manager {

using mimax::mt::CTasksRunner;
using namespace std;

class CTaskMock : public mimax_mock::mt::CTaskMock
{
public:
    CTaskMock()
        : m_isStopRequested(false)
        , m_isTaskCompletedProperly(false)
    {
        ON_CALL(*this, RunTask())
            .WillByDefault([this]()
                {
                    auto const startTime = chrono::high_resolution_clock::now();
                    auto const endTime = startTime + 1000ms;
                    while (!m_isStopRequested)
                    {
                        if (chrono::high_resolution_clock::now() >= endTime)
                        {
                            bool const isTimeOut = true;
                            EXPECT_FALSE(isTimeOut);
                            return;
                        }
                    }
                    m_isTaskCompletedProperly = true;
                });

        ON_CALL(*this, StopTask())
            .WillByDefault([this]()
                {
                    m_isStopRequested = true;
                });
    }

    void ExpectRunTaskIsCalledOnce()
    {
        EXPECT_CALL(*this, RunTask())
            .Times(1);
    }

    void ExpectStopTaskIsCalledOnce()
    {
        EXPECT_CALL(*this, StopTask())
            .Times(1);
    }

    bool IsTaskCompletedProperly() const { return m_isTaskCompletedProperly; }

private:
    bool m_isStopRequested;
    bool m_isTaskCompletedProperly;
};

GTEST_TEST(MtCTasksRunner, RunTasksAndWaitExpectRunTaskIsCalled)
{
    testing::NiceMock<CTaskMock> taskMock;
    taskMock.ExpectRunTaskIsCalledOnce();
    CTasksRunner TasksRunner;

    TasksRunner.RunTasksAndWait({ &taskMock }, 0ms);
}

GTEST_TEST(MtCTasksRunner, RunTasksAndWaitExpectStopTaskIsCalled)
{
    testing::NiceMock<CTaskMock> taskMock;
    taskMock.ExpectStopTaskIsCalledOnce();
    CTasksRunner TasksRunner;

    TasksRunner.RunTasksAndWait({ &taskMock }, 0ms);
}

GTEST_TEST(MtCTasksRunner, RunTasksAndWaitExpectTaskIsCompletedProperly)
{
    testing::NiceMock<CTaskMock> taskMock;
    CTasksRunner TasksRunner;

    TasksRunner.RunTasksAndWait({ &taskMock }, 0ms);

    EXPECT_TRUE(taskMock.IsTaskCompletedProperly());
}

GTEST_TEST(MtCTasksRunner, RunTasksAndWaitRun2TasksExpectNoHang)
{
    testing::NiceMock<CTaskMock> taskMock1;
    testing::NiceMock<CTaskMock> taskMock2;
    CTasksRunner TasksRunner;

    TasksRunner.RunTasksAndWait({ &taskMock1, &taskMock2 }, 0ms);
}

} // tasks_manager
} // mt
} // mimax_test