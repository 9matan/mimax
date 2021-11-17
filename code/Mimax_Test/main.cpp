#include "mimax_test/common/Test_MimaxCommon.h"
#include "mimax_test/minimax/Test_Minimax.h"

int main()
{
    mimax_test::common::RunTests();
    mimax_test::minimax::RunTests();

    return 0;
}