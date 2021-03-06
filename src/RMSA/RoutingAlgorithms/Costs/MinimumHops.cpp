#include <RMSA/RoutingAlgorithms/Costs/MinimumHops.h>
#include <Structure/Link.h>

using namespace RMSA::ROUT;

MinimumHops::MinimumHops(std::shared_ptr<Topology> T) :
    RoutingCost(T, MH)
{

}

double MinimumHops::get_Cost(
    std::weak_ptr<Link> link,
    std::shared_ptr<Call>)
{
    if (!link.lock()->is_LinkActive())
        {
        return std::numeric_limits<double>::max();
        }

    return 1;
}

void MinimumHops::save(std::string SimConfigFileName)
{
    RoutingCost::save(SimConfigFileName);
}

void MinimumHops::print()
{
    RoutingCost::print();
}
