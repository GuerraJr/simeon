#ifndef SIMULATIONTYPE_H
#define SIMULATIONTYPE_H

#include <fstream>
#include <string>
#include <boost/bimap.hpp>
#include <Structure/Topology.h>

#include <RWA/RoutingAlgorithms/RoutingAlgorithm.h>
#include <RWA/WavelengthAssignmentAlgorithms/WavelengthAssignmentAlgorithm.h>
#include <RWA/RegeneratorPlacementAlgorithms/RegeneratorPlacementAlgorithm.h>
#include <RWA/RegeneratorAssignmentAlgorithms/RegeneratorAssignmentAlgorithm.h>

class SimulationType {
  public:
#define NETWORK_TYPE \
    X(TransparentNetwork, "Transparent Network") \
    X(TranslucentNetwork, "Translucent Network") //X Macros

#define SIMULATION_TYPE \
    X(networkload, "Network Load Variation", "networkload") \
    X(psroptimization, "Power Series Routing PSO Optimization", "psroptimization") \
    X(statisticaltrend, "Statistical Trend Analysis", "statisticaltrend")

#define X(a,b) a,
    enum Network_Type {
        NETWORK_TYPE
    };
#undef X

#define X(a,b,c) a,
    enum Simulation_Type {
        SIMULATION_TYPE
    };
#undef X

    static std::shared_ptr<SimulationType> create();

    typedef boost::bimap<Network_Type, std::string> NetworkTypeBimap;
    static NetworkTypeBimap NetworkTypes;
    typedef boost::bimap<Simulation_Type, std::string> SimulationTypeNameBimap;
    static SimulationTypeNameBimap SimulationTypeNames;
    typedef boost::bimap<Simulation_Type, std::string> SimulationTypeNicknameBimap;
    static SimulationTypeNameBimap SimulationTypeNicknames;

    virtual void run() = 0;
    virtual void load() = 0;
    virtual void print() = 0;
    virtual void save(std::string) = 0;
    virtual void load_file(std::string) = 0;
    virtual void help() = 0;

    static Network_Type Type;
    Topology::DefaultTopologies Chosen_Topology;
    std::shared_ptr<Topology> T;
};

#endif // SIMULATIONTYPE_H
