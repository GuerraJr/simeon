#ifndef SIMULATION_PSROPTIMIZATION_H
#define SIMULATION_PSROPTIMIZATION_H

#include <SimulationTypes/SimulationType.h>
#include <GeneralPurposeAlgorithms/PSO.h>
#include <RWA/RoutingAlgorithms/PowerSeriesRouting/PowerSeriesRouting.h>
#include <RWA/RoutingAlgorithms/RoutingAlgorithm.h>
#include <RWA/WavelengthAssignmentAlgorithms/WavelengthAssignmentAlgorithm.h>
#include <RWA/RegeneratorPlacementAlgorithms/RegeneratorPlacementAlgorithm.h>
#include <RWA/RegeneratorAssignmentAlgorithms/RegeneratorAssignmentAlgorithm.h>

class Simulation_PSROptimization : public SimulationType {
  public:
    Simulation_PSROptimization();

    void help();
    void run();
    void load();
    void save(std::string);
    void load_file(std::string);
    void print();

  private:
    bool hasLoaded;
    bool hasRun;

    static WavelengthAssignmentAlgorithm::WavelengthAssignmentAlgorithms
    WavAssign_Algorithm;
    static RegeneratorPlacementAlgorithm::RegeneratorPlacementAlgorithms
    RegPlacement_Algorithm;
    static RegeneratorAssignmentAlgorithm::RegeneratorAssignmentAlgorithms
    RegAssignment_Algorithm;

    static double NumCalls;
    static double OptimizationLoad;

    int NMin;
    int NMax;
    static std::vector<std::shared_ptr<PSR::Cost>> Costs;

    std::string FileName;

    static constexpr unsigned int P = 50;
    static constexpr unsigned int G = 500;
    static constexpr double XMin = -1;
    static constexpr double XMax = 1;
    static constexpr double VMin = -1;
    static constexpr double VMax = 1;

    void create_Simulation();

    struct Compare {
        bool operator()(double a, double b) {
            return a < b;
        }
    };

    struct Fitness {
        static std::shared_ptr<Topology> T;
        double operator()(std::shared_ptr<PSO::PSO_Particle<double>>);
    };

    std::shared_ptr<PSO::ParticleSwarmOptimization<double, Fitness, Compare>>
            PSO_Optim;

    void printCoefficients(std::string file, bool override = true);
};

#endif // SIMULATION_PSROPTIMIZATION_H
