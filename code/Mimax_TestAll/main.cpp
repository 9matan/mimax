#include "mimax_test/common/Test_MimaxCommon_Main.h"
#include "mimax_test/minimax/Test_Minimax_Main.h"

int main()
{
    mimax_test::common::RunTests();
    mimax_test::minimax::RunTests();

    return 0;
}