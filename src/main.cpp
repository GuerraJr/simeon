#include "GeneralClasses.h"
#include "Devices.h"
#include "Structure.h"
#include "Calls.h"
#include "RWA.h"
#include "SimulationTypes.h"

#include <iostream>

int main(void) {
    std::shared_ptr<Topology> T = std::shared_ptr<Topology>(new Topology("NSFNet"));

    std::vector<ModulationScheme> Schemes;
    Schemes.push_back(ModulationScheme(4, Gain(6.8)));
    Schemes.push_back(ModulationScheme(16, Gain(10.5)));
    Schemes.push_back(ModulationScheme(64, Gain(14.8)));

    std::vector<TransmissionBitrate> Bitrates;
    Bitrates.push_back(TransmissionBitrate(10E9));
    Bitrates.push_back(TransmissionBitrate(40E9));
    Bitrates.push_back(TransmissionBitrate(100E9));
    Bitrates.push_back(TransmissionBitrate(160E9));
    Bitrates.push_back(TransmissionBitrate(400E9));

    std::shared_ptr<RoutingAlgorithm> SP(new ShortestPath(T));
    std::shared_ptr<WavelengthAssignmentAlgorithm> FF(new FirstFit(T));
    std::shared_ptr<RegeneratorAssignmentAlgorithm> FLR(new FirstLongestReach(T,
            Schemes));
    std::shared_ptr<RoutingWavelengthAssignment>
    RWA(new RoutingWavelengthAssignment(SP, FF, FLR, Schemes, T));

    std::shared_ptr<RegeneratorPlacementAlgorithm> MU(new MostUsed(T, RWA, 80, 1E6,
            Bitrates));
    MU->placeRegenerators(10, 20);

    for (long double load = 80; load <= 300; load += 10) {
        std::shared_ptr<CallGenerator> CG(new CallGenerator(T, load, Bitrates));
        NetworkSimulation Sim(CG, RWA, 1E5);
        std::cout << load << "\t" << Sim.get_CallBlockingProbability() << std::endl;
    }

    return 0;
}
