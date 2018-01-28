#ifndef LENGTHOCCUPATIONROUTINGAVAILABILITY_H
#define LENGTHOCCUPATIONROUTINGAVAILABILITY_H

#include <RMSA/RoutingAlgorithms/RoutingCost.h>

namespace RMSA
{
namespace ROUT
{
/**
 * @brief The LengthOccupationRoutingAvailability class represents the Length and
 * Occupation Routing algorithm (availability variant). Its link cost is a
 * ponderation with the link length and its availability.
 */
class LengthOccupationRoutingAvailability : public RoutingCost
{
public:
    LengthOccupationRoutingAvailability(std::shared_ptr<Topology> T);

    double get_Cost(std::weak_ptr<Link> link, std::shared_ptr<Call> C);
    std::vector<std::vector<std::weak_ptr<Link>>> route(std::shared_ptr<Call> C);
    void load()
    {
        RoutingCost::load();
    }
    void save(std::string);
    void print();
};
}
}

#endif // LENGTHOCCUPATIONROUTINGAVAILABILITY_H
