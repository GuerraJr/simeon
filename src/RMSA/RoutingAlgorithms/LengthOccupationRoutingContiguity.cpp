#include <RMSA/RoutingAlgorithms/LengthOccupationRoutingContiguity.h>
#include <Structure/Link.h>
#include <Structure/Topology.h>

using namespace RMSA::ROUT;

LengthOccupationRoutingContiguity::LengthOccupationRoutingContiguity(
    std::shared_ptr<Topology> T) : DijkstraRoutingAlgorithm(T, LORc)
{

}

double LengthOccupationRoutingContiguity::get_Cost(
    std::weak_ptr<Link> link,
    std::shared_ptr<Call> C)
{
    return 1 +
           (link.lock()->Length / T->get_LengthLongestLink()) +
           (1.0 / (link.lock()->get_Contiguity(C) + 1));
}

void LengthOccupationRoutingContiguity::save(std::string SimConfigFileName)
{
    RoutingAlgorithm::save(SimConfigFileName);
}
