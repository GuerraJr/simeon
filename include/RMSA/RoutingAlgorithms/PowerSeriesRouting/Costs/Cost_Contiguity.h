#ifndef COST_CONTIGUITY_H
#define COST_CONTIGUITY_H

#include "Cost.h"

namespace ROUT
{
namespace PSR
{

/**
* @brief The cContiguity class has, as cost, the contiguity measure of the
* link. It only works on transparent networks. If sr is the number of
* possible allocations of the call C in the Link link, then this cost is
* 1.0 / (sr + 1).
*/
class cContiguity : public Cost
{
public:
    cContiguity(int NMin, int NMax, std::shared_ptr<Topology> T);
    arma::rowvec getCost(std::weak_ptr<Link> link, std::shared_ptr<Call> C);

private:
    void createCache();
    arma::mat cache;
};

}
}

#endif // COST_CONTIGUITY_H
