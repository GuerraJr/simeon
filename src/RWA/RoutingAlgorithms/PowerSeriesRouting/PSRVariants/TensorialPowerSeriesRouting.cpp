#include <RWA/RoutingAlgorithms/PowerSeriesRouting/PSRVariants/TensorialPowerSeriesRouting.h>
#include <RWA/RoutingAlgorithms/PowerSeriesRouting/Costs/Cost.h>

TensorialPowerSeriesRouting::TensorialPowerSeriesRouting
(std::shared_ptr<Topology> T) : PowerSeriesRouting(T, RoutingAlgorithm::tenPSR)
{

}

TensorialPowerSeriesRouting::TensorialPowerSeriesRouting
(std::shared_ptr<Topology> T, std::vector<std::shared_ptr<PSR::Cost>> Costs) :
    PowerSeriesRouting(T, Costs, RoutingAlgorithm::tenPSR)
{

}

double TensorialPowerSeriesRouting::get_Cost
(std::weak_ptr<Link> link, std::shared_ptr<Call> C)
{
    arma::mat cost_matrix = arma::ones(1);

    for (auto &cost : Costs)
        {
        cost_matrix = arma::kron(cost_matrix, cost->getCost(link, C));
        }

    if (!firstTimeRun)
        {
        coefficients.copy_size(cost_matrix);
        firstTimeRun = true;
        }

    return arma::accu(coefficients % cost_matrix);
}
