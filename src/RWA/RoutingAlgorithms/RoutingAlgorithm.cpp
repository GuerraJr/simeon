#include <RWA/RoutingAlgorithms/RoutingAlgorithm.h>
#include <boost/assign.hpp>
#include <iostream>

RoutingAlgorithm::RoutAlgNameBimap RoutingAlgorithm::RoutingAlgorithmNames =
    boost::assign::list_of<RoutingAlgorithm::RoutAlgNameBimap::relation>
#define X(a,b,c) (a,b)
    ROUTING_ALGORITHM
#undef X
    ;

RoutingAlgorithm::RoutAlgNicknameBimap
RoutingAlgorithm::RoutingAlgorithmNicknames =
    boost::assign::list_of<RoutingAlgorithm::RoutAlgNicknameBimap::relation>
#define X(a,b,c) (a,c)
    ROUTING_ALGORITHM
#undef X
#undef ROUTING_ALGORITHM
    ;


RoutingAlgorithm::RoutingAlgorithm(std::shared_ptr<Topology> T) : T(T) {

}

RoutingAlgorithm::RoutingAlgorithms RoutingAlgorithm::load() {
    std::cout << std::endl << "-> Choose a routing algorithm." << std::endl;

    do {
        for (auto routing : RoutingAlgorithmNames.left) {
            std::cout << "(" << routing.first << ")\t" << routing.second << std::endl;
        }

        int Routing_Alg;
        std::cin >> Routing_Alg;

        if (std::cin.fail() || RoutingAlgorithmNames.left.count
                ((RoutingAlgorithms) Routing_Alg) == 0) {
            std::cin.clear();
            std::cin.ignore();

            std::cerr << "Invalid routing algorithm." << std::endl;
            std::cout << std::endl << "-> Choose a routing algorithm." << std::endl;
        } else {
            return (RoutingAlgorithms) Routing_Alg;
        }
    } while (1);

    return (RoutingAlgorithms) - 1;
}
