#ifndef ROUTINGALGORITHM_H
#define ROUTINGALGORITHM_H

#include <boost/bimap.hpp>
#include <memory>
#include <vector>

class Topology;
class Call;
class Link;

namespace RMSA
{
//! Contains the routing algorithms
namespace ROUT
{
/**
 * @brief The RoutingAlgorithm class searches for a route, given a Call.
 */
class RoutingAlgorithm
{
public:
#define ROUTING_TYPE \
    X(dijkstra_alg, "Dijkstra Shortest Path Algorithm") \
    X(yen_alg, "Yen K-Shortest Path Algorithm")

#define ROUTING_ALGORITHM \
    X(AWR, "Adaptative Weighing Routing", "AWR", PSR::AdaptativeWeighingRouting) \
    X(LORa, "Length and Occupation Routing - Availability", "LORa", LengthOccupationRoutingAvailability) \
    X(LORc, "Length and Occupation Routing - Contiguity", "LORc", LengthOccupationRoutingContiguity) \
    X(localPSR, "Local Power Series Routing", "localPSR", PSR::LocalPowerSeriesRouting) \
    X(matPSR, "Matricial Power Series Routing", "matPSR", PSR::MatricialPowerSeriesRouting) \
    X(tenPSR, "Tensorial Power Series Routing", "tenPSR", PSR::TensorialPowerSeriesRouting) \
    X(MH, "Minimum Hops", "MH", MinimumHops) \
    X(SP, "Shortest Path", "SP", ShortestPath) //Enum value, Name, Nickname, Class name.

#define X(a,b) a,
    enum RoutingType
    {
        ROUTING_TYPE
    };
#undef X

#define X(a,b,c,d) a,
    enum RoutingAlgorithms
    {
        ROUTING_ALGORITHM
    };
#undef X

    typedef boost::bimap<RoutingAlgorithms, std::string> RoutAlgNameBimap;
    static RoutAlgNameBimap RoutingAlgorithmNames;
    typedef boost::bimap<RoutingAlgorithms, std::string> RoutAlgNicknameBimap;
    static RoutAlgNicknameBimap RoutingAlgorithmNicknames;

    RoutingAlgorithms RoutAlgType;

    RoutingAlgorithm(std::shared_ptr<Topology> T, RoutingAlgorithms RoutAlgType);

    static RoutingAlgorithms define_RoutingAlgorithm();
    static std::shared_ptr<RoutingAlgorithm> create_RoutingAlgorithm(
        RoutingAlgorithms, std::shared_ptr<Topology>);
    virtual void load() = 0;
    virtual void save(std::string) = 0;

    std::vector<std::vector<std::weak_ptr<Link>>> route(std::shared_ptr<Call> C);
    virtual double get_Cost(std::weak_ptr<Link> link, std::shared_ptr<Call> C) = 0;

    std::shared_ptr<Topology> T;

    /**
     * @brief kShortestPaths is the number of shortest paths that will be searched
     * by the KSP Algorithms, like Yen on Eppstein,  if one of them is choosed.
     */
    static constexpr int kShortestPaths = 3;

private:
    std::vector<std::vector<std::weak_ptr<Link>>> dijkstra(std::shared_ptr<Call> C);
    std::vector<std::vector<std::weak_ptr<Link>>> yen(std::shared_ptr<Call> C);

    double get_RoutingCost(std::vector<std::weak_ptr<Link>>, std::shared_ptr<Call>);
};
}
}

#endif // ROUTINGALGORITHM_H
