#ifndef DIJKSTRA_ROUTINGALGORITHM_H
#define DIJKSTRA_ROUTINGALGORITHM_H

#include "../RoutingAlgorithm.h"

namespace RMSA
{
namespace ROUT
{
class Dijkstra_RoutingAlgorithm : public RoutingAlgorithm
{
public:
    Dijkstra_RoutingAlgorithm(std::shared_ptr<Topology> T);
    std::vector<std::vector<std::weak_ptr<Link>>> route(std::shared_ptr<Call> C);
};
}
}

#endif // DIJKSTRA_ROUTINGALGORITHM_H
