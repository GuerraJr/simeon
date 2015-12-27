#include "GeneralClasses.h"
#include "Devices.h"
#include "Structure.h"
#include "Calls.h"
#include "RWA.h"
#include "SimulationTypes.h"

#include "GeneralPurposeAlgorithms/NSGA-II/NSGA-II.h"

#include <iostream>

int main(void) {
    std::cout << "\t* * * SIMULATOR OF SLICE OPTICAL NETWORKS * * *"
              << std::endl;

    auto simulation = SimulationType::create();
    simulation->print();

    return 0;
}
