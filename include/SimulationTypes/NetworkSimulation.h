#ifndef NETWORKSIMULATION_H
#define NETWORKSIMULATION_H

#include <memory>
#include <Calls/CallGenerator.h>
#include <RWA/RoutingWavelengthAssignment.h>

class NetworkSimulation {
  public:
    NetworkSimulation(std::shared_ptr<CallGenerator> Generator,
                      std::shared_ptr<RoutingWavelengthAssignment> RWA,
                      long unsigned NumMaxCalls);

    void run();

    std::shared_ptr<CallGenerator> Generator;
    std::shared_ptr<RoutingWavelengthAssignment> RWA;

    long unsigned NumMaxCalls;
    long unsigned NumCalls;
    long unsigned NumBlockedCalls;

  private:
    bool implement_call(Event evt);
    void drop_call(Event evt);
};

#endif // NETWORKSIMULATION_H
