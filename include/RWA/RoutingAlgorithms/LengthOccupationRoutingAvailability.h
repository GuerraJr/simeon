#ifndef LENGTHOCCUPATIONROUTINGAVAILABILITY_H
#define LENGTHOCCUPATIONROUTINGAVAILABILITY_H

#include <RWA/RoutingAlgorithms/DijkstraRoutingAlgorithm.h>

class LengthOccupationRoutingAvailability : public DijkstraRoutingAlgorithm {
  public:
    LengthOccupationRoutingAvailability(std::shared_ptr<Topology> T);

    double get_Cost(std::weak_ptr<Link> link, std::shared_ptr<Call> C);
    void load() {}
};

#endif // LENGTHOCCUPATIONROUTINGAVAILABILITY_H
