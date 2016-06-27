#include "SimulationTypes.h"
#include <gtest/gtest.h>

namespace Simulations
{
bool parallelism_enabled = true;
}

bool considerAseNoise = false;
bool considerFilterImperfection = false;

#ifdef RUN_TESTS
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    if (RUN_ALL_TESTS())
        {
        return -1;
        }
#else
int main()
{
#endif

    std::cout << "\t* * * SIMULATOR OF SLICE OPTICAL NETWORKS * * *"
              << std::endl;

    auto simulation = Simulations::SimulationType::create();
    simulation->print();
    simulation->run();

    return 0;
}
