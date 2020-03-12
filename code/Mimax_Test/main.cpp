#include "Minimax/MinimaxBFSAlgorithm.h"
#include "Minimax/MinimaxDFSAlgorithm.h"

int main()
{
    mimax::CMinimaxBFSAlgorithm<int, int> bfsAlgo(nullptr, {});
    mimax::CMinimaxDFSAlgorithm<int, int> dfsAlgo(nullptr, {});

    return 0;
}