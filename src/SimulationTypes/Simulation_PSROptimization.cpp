#include <SimulationTypes/Simulation_PSROptimization.h>
#include <GeneralPurposeAlgorithms/PSO.h>
#include <iomanip>
#include <RMSA/RoutingAlgorithms/Costs/PowerSeriesRouting/Costs.h>
#include <SimulationTypes/NetworkSimulation.h>
#include <Calls.h>
#include <RMSA.h>
#include <Structure/Link.h>
#include <sstream>
#include <boost/assert.hpp>
#include <boost/assign.hpp>
#include <boost/program_options.hpp>
#include <map>

using namespace RMSA;
using namespace Simulations;
using namespace ROUT::PSR;
using namespace SA;
using namespace RA;
using namespace RP;
using namespace PSO;

double Simulation_PSROptimization::NumCalls;
double Simulation_PSROptimization::OptimizationLoad;
std::vector<std::shared_ptr<Cost>> Simulation_PSROptimization::Costs;
std::shared_ptr<Topology> Simulation_PSROptimization::Fitness::T;
PowerSeriesRouting::Variants Simulation_PSROptimization::Fitness::Variant;
ROUT::RoutingAlgorithm::RoutingAlgorithms
Simulation_PSROptimization::Routing_Algorithm;
SA::SpectrumAssignmentAlgorithm::SpectrumAssignmentAlgorithms
Simulation_PSROptimization::WavAssign_Algorithm;
RegeneratorPlacementAlgorithm::RegeneratorPlacementAlgorithms
Simulation_PSROptimization::RegPlacement_Algorithm;
RegeneratorAssignmentAlgorithm::RegeneratorAssignmentAlgorithms
Simulation_PSROptimization::RegAssignment_Algorithm;

Simulation_PSROptimization::Simulation_PSROptimization() :
    SimulationType(Simulation_Type::psroptimization)
{
    hasLoaded = hasRun = false;
}

void Simulation_PSROptimization::help()
{
    std::cout << "\t\tPSR OPTIMIZATION SIMULATION" << std::endl << std::endl <<
              "This simulation runs the Particle Swarm Optimization algorithm"
              " to find a set of coefficients that minimize the call blocking"
              " probability of the network. Warning: this usually takes"
              " some time." << std::endl;
}

void Simulation_PSROptimization::load()
{
    //Generic readings.
    SimulationType::load();

    std::cout << std::endl << "-> Choose a network type." << std::endl;

    do
        {
        for (auto &nettype : NetworkTypes.left)
            {
            std::cout << "(" << nettype.first << ")\t" << nettype.second << std::endl;
            }

        int Net_Type;
        std::cin >> Net_Type;

        if (std::cin.fail() || NetworkTypes.left.count((Network_Type) Net_Type) == 0)
            {
            std::cin.clear();
            std::cin.ignore();

            std::cerr << "Invalid Network Type." << std::endl;
            std::cout << std::endl << "-> Choose a network type." << std::endl;
            }
        else
            {
            Type = (Network_Type) Net_Type;
            break;
            }
        }
    while (1);

    std::cout << std::endl << "-> Choose a PSR Variant." << std::endl;
    do
        {
        for (auto &variant : PowerSeriesRouting::VariantNames.left)
            {
            std::cout << "(" << variant.first << ")\t" << variant.second << std::endl;
            }

        int PSR_Variant;
        std::cin >> PSR_Variant;

        if (std::cin.fail() ||
                PowerSeriesRouting::VariantNames.left.count
                ((PowerSeriesRouting::Variants) PSR_Variant) == 0)
            {
            std::cin.clear();
            std::cin.ignore();

            std::cerr << "Invalid PSR Variant." << std::endl;
            std::cout << std::endl << "-> Choose a PSR Variant." << std::endl;
            }
        else
            {
            Variant = (PowerSeriesRouting::Variants) PSR_Variant;
            break;
            }
        }
    while(1);

    Node::load();

    Link::load(T);

    //RMSA Algorithms
        {
        //Routing Algorithm
        Routing_Algorithm = ROUT::RoutingAlgorithm::define_RoutingAlgorithm();

        //Wavelength Assignment Algorithm
        WavAssign_Algorithm =
            SA::SpectrumAssignmentAlgorithm::define_SpectrumAssignmentAlgorithm();

        if (Type == TranslucentNetwork)
            {
            //Regenerator Placement Algorithm
            RegPlacement_Algorithm =
                RegeneratorPlacementAlgorithm::define_RegeneratorPlacementAlgorithm();

            //Regenerator Assignment Algorithm
            RegAssignment_Algorithm =
                RegeneratorAssignmentAlgorithm::define_RegeneratorAssignmentAlgorithm();
            }
        }

    switch(Variant)
        {
        case PowerSeriesRouting::Variant_LocalPSR:
        case PowerSeriesRouting::Variant_MatricialPSR:
        case PowerSeriesRouting::Variant_TensorialPSR:

            std::cout << std::endl << "-> Define the minimum PSR Exponent." << std::endl;

            do
                {
                int nmin;
                std::cin >> nmin;

                if (std::cin.fail())
                    {
                    std::cin.clear();
                    std::cin.ignore();

                    std::cerr << "Invalid minimum exponent." << std::endl;
                    std::cout << std::endl << "-> Define the minimum PSR Exponent."
                              << std::endl;
                    }
                else
                    {
                    NMin = nmin;
                    break;
                    }
                }
            while (1);

            std::cout << std::endl << "-> Define the maximum PSR Exponent." << std::endl;

            do
                {
                int nmax;
                std::cin >> nmax;

                if (std::cin.fail() || nmax < NMin)
                    {
                    std::cin.clear();
                    std::cin.ignore();

                    std::cerr << "Invalid maximum exponent." << std::endl;
                    std::cout << std::endl << "-> Define the maximum PSR Exponent." << std::endl;
                    }
                else
                    {
                    NMax = nmax;
                    break;
                    }
                }
            while (1);
            break;

        case PowerSeriesRouting::Variant_AWR:
            NMax = NMin = 1;
            break;
        }

    std::cout << std::endl << "-> Choose the PSR Costs." << std::endl;

    do
        {
        std::vector<Cost::PossibleCosts> chosenCosts;

        do
            {
            int numPossibleCosts = 0;

            for (auto &cost : Cost::CostsNames.left)
                {
                if (std::find(chosenCosts.begin(), chosenCosts.end(),
                              cost.first) != chosenCosts.end())
                    {
                    continue;
                    } //Verifies whether the cost has already been chosen.

                std::cout << "(" << cost.first << ")\t" << cost.second << std::endl;
                numPossibleCosts++;
                }

            if (numPossibleCosts == 0)
                {
                break;
                }

            int Cost;
            std::cin >> Cost;

            if (std::cin.fail() ||
                    Cost::CostsNames.left.count((Cost::PossibleCosts) Cost) == 0)
                {
                std::cin.clear();
                std::cin.ignore();

                if (Cost == -1 && !chosenCosts.empty())
                    {
                    break;
                    }

                std::cerr << "Invalid Cost." << std::endl;
                }
            else if (std::find(chosenCosts.begin(), chosenCosts.end(),
                               (Cost::PossibleCosts) Cost) == chosenCosts.end())
                {
                chosenCosts.push_back((Cost::PossibleCosts) Cost);
                Costs.push_back(Cost::createCost((Cost::PossibleCosts) Cost, NMin, NMax, T));
                } //Verifies that the cost hasn't been chosen.

            std::cout << std::endl << "-> Choose the PSR Costs. (-1 to exit)" << std::endl;
            }
        while (1);

        }
    while (0);   //Dummy do-while. Only to encapsulate reading.

    std::cout << std::endl << "-> Define the number of calls." << std::endl;

    do
        {
        std::cin >> NumCalls;

        if (std::cin.fail() || NumCalls < 0)
            {
            std::cin.clear();
            std::cin.ignore();

            std::cerr << "Invalid number of calls." << std::endl;
            std::cout << std::endl << "-> Define the number of calls." << std::endl;
            }
        else
            {
            break;
            }
        }
    while (1);

    std::cout << std::endl << "-> Define the optimization network load."
              << std::endl;

    do
        {
        std::cin >> OptimizationLoad;

        if (std::cin.fail() || OptimizationLoad < 0)
            {
            std::cin.clear();
            std::cin.ignore();

            std::cerr << "Invalid network load." << std::endl;
            std::cout << std::endl << "-> Define the optimization network load." <<
                      std::endl;
            }
        else
            {
            break;
            }
        }
    while (1);

    std::cout << std::endl << "-> Define the file where to store the coefficients."
              << std::endl;

    do
        {
        std::cin >> CoefficientsFilename;

        if (std::cin.fail())
            {
            std::cin.clear();
            std::cin.ignore();

            std::cerr << "Invalid filename." << std::endl;
            std::cout << std::endl << "-> Define the file where to store the coefficients."
                      << std::endl;
            }
        else
            {
            break;
            }
        }
    while (1);

    std::cout << std::endl << "-> Define the file where to log the optimization."
              << std::endl;

    do
        {
        std::cin >> LogFilename;

        if (std::cin.fail() || LogFilename == CoefficientsFilename)
            {
            std::cin.clear();
            std::cin.ignore();

            std::cerr << "Invalid filename." << std::endl;
            std::cout << std::endl << "-> Define the file where to log the optimization."
                      << std::endl;
            }
        else
            {
            break;
            }
        }
    while (1);


    hasLoaded = true;
}

void Simulation_PSROptimization::create_Simulation()
{

}

void Simulation_PSROptimization::save(std::string SimConfigFileName)
{
    SimulationType::save(SimConfigFileName);

    std::ofstream SimConfigFile(SimConfigFileName,
                                std::ofstream::out | std::ofstream::app);

    BOOST_ASSERT_MSG(SimConfigFile.is_open(), "Output file is not open");

    SimConfigFile << "  NetworkType = " << NetworkTypesNicknames.left.at(
                      Type) << std::endl;

    SimConfigFile.close();

    Link::save(SimConfigFileName, T);

    SimConfigFile.open(SimConfigFileName,
                       std::ofstream::out | std::ofstream::app);
    BOOST_ASSERT_MSG(SimConfigFile.is_open(), "Output file is not open");

    SimConfigFile << std::endl << "  [algorithms]" << std::endl << std::endl;
    SimConfigFile << "  WavelengthAssignmentAlgorithm = " <<
                  SA::SpectrumAssignmentAlgorithm::SpectrumAssignmentAlgorithmNicknames.left.at(
                      WavAssign_Algorithm) << std::endl;
    if(Type == TranslucentNetwork)
        {
        SimConfigFile << "  RegeneratorPlacementAlgorithm = " <<
                      RegeneratorPlacementAlgorithm::RegeneratorPlacementNicknames.left.at(
                          RegPlacement_Algorithm) << std::endl;
        SimConfigFile << "  RegeneratorAssignmentAlgorithm = " <<
                      RegeneratorAssignmentAlgorithm::RegeneratorAssignmentNicknames.left.at(
                          RegAssignment_Algorithm) << std::endl;
        }

    SimConfigFile << std::endl << "  [sim_info]" << std::endl << std::endl;
    SimConfigFile << "  MinimumPSRExponent = " << NMin << std::endl;
    SimConfigFile << "  MaximumPSRExponent = " << NMax << std::endl;
    SimConfigFile << "  PSRCosts =";
    for(auto &cost : Costs)
        {
        SimConfigFile << " " << Cost::CostsNicknames.left.at(cost->Type);
        }
    SimConfigFile << std::endl;
    SimConfigFile << "  PSRVariant =" << PowerSeriesRouting::VariantNicknames.
                  left.at(Variant) << std::endl;
    SimConfigFile << "  NumCalls = " << NumCalls << std::endl;
    SimConfigFile << "  NetworkLoad = " << OptimizationLoad << std::endl;

    SimConfigFile << std::endl;
    T->save(SimConfigFileName);
}

void Simulation_PSROptimization::load_file(std::string ConfigFileName)
{
    using namespace boost::program_options;

    options_description ConfigDesctription("Configurations Data");
    ConfigDesctription.add_options()
    ("general.SimulationType", value<std::string>()->required(), "Simulation Type")
    ("general.NetworkType", value<std::string>()->required(), "Network Type")
    ("general.AvgSpanLength", value<long double>()->required(),
     "Distance Between Inline Amps.")
    ("algorithms.WavelengthAssignmentAlgorithm", value<std::string>()->required(),
     "Wavelength Assignment Algorithm")
    ("algorithms.RegeneratorPlacementAlgorithm", value<std::string>(),
     "Regenerator Placement Algorithm")
    ("algorithms.RegeneratorAssignmentAlgorithm", value<std::string>(),
     "Regenerator Assignment Algorithm")
    ("sim_info.MinimumPSRExponent", value<int>()->required(),
     "Minimum PSR Exponent")
    ("sim_info.MaximumPSRExponent", value<int>()->required(),
     "Maximum PSR Exponent")
    ("sim_info.PSRCosts", value<std::vector<std::string>>()->multitoken(),
     "PSR Costs")
    ("sim_info.PSRVariant", value<std::string>()->required(), "PSR Variant")
    ("sim_info.NumCalls", value<long double>()->required(), "Number of Calls")
    ("sim_info.NetworkLoad", value<long double>()->required(), "Network Load");

    variables_map VariablesMap;

    std::ifstream ConfigFile(ConfigFileName, std::ifstream::in);
    BOOST_ASSERT_MSG(ConfigFile.is_open(), "Input file is not open");

    store(parse_config_file<char>(ConfigFile, ConfigDesctription, true),
          VariablesMap);
    ConfigFile.close();
    notify(VariablesMap);

    T = std::shared_ptr<Topology>(new Topology(ConfigFileName));
    Type = NetworkTypesNicknames.right.at(
               VariablesMap["general.NetworkType"].as<std::string>());
    Link::DefaultAvgSpanLength =
        VariablesMap["general.AvgSpanLength"].as<long double>();
    T->set_avgSpanLength(VariablesMap["general.AvgSpanLength"].as<long double>());

    WavAssign_Algorithm =
        SA::SpectrumAssignmentAlgorithm::SpectrumAssignmentAlgorithmNicknames.right.at(
            VariablesMap["algorithms.WavelengthAssignmentAlgorithm"].as<std::string>());
    if(Type == Network_Type::TranslucentNetwork)
        {
        RegPlacement_Algorithm =
            RegeneratorPlacementAlgorithm::RegeneratorPlacementNicknames.right.at(
                VariablesMap["algorithms.RegeneratorPlacementAlgorithm"].as<std::string>());
        RegAssignment_Algorithm =
            RegeneratorAssignmentAlgorithm::RegeneratorAssignmentNicknames.right.at(
                VariablesMap["algorithms.RegeneratorAssignmentAlgorithm"].as<std::string>());
        }

    NMin = VariablesMap["sim_info.MinimumPSRExponent"].as<int>();
    NMax = VariablesMap["sim_info.MaximumPSRExponent"].as<int>();

    std::vector<std::string> CostsList =
        VariablesMap.find("sim_info.PSRCosts")->second.as<std::vector<std::string>>();
    for(auto &costs : CostsList)
        {
        std::istringstream PSR_Costs(costs);
        std::string Aux;
        while(PSR_Costs.tellg() != -1) // Reading the last cost twice
            {
            PSR_Costs >> Aux;
            Costs.push_back(Cost::createCost((Cost::PossibleCosts)
                                             Cost::CostsNicknames.right.at(Aux), NMin, NMax, T));
            }
        }

    Variant = PowerSeriesRouting::VariantNicknames.right.at(
                  VariablesMap["sim_info.PSRVariant"].as<std::string>());
    NumCalls = VariablesMap["sim_info.NumCalls"].as<long double>();
    OptimizationLoad = VariablesMap["sim_info.NetworkLoad"].as<long double>();

    std::cout << std::endl << "-> Define the file where to store the coefficients."
              << std::endl;

    do
        {
        std::cin >> CoefficientsFilename;

        if (std::cin.fail())
            {
            std::cin.clear();
            std::cin.ignore();

            std::cerr << "Invalid filename." << std::endl;
            std::cout << std::endl << "-> Define the file where to store the coefficients."
                      << std::endl;
            }
        else
            {
            break;
            }
        }
    while (1);

    hasLoaded = true;
}

void Simulation_PSROptimization::print()
{
    if (!hasLoaded)
        {
        load();
        }

    std::cout << std::endl <<
              "  A Power Series Routing PSO Optimization Simulation is about to start with the following parameters: "
              << std::endl;
    std::cout << "-> Metrics =" << std::endl;
    for(auto &metric : Metrics)
        {
        std::cout << "\t-> " << SimulationType::MetricTypes.left.at(
                      metric) << std::endl;
        }
    if(considerFilterImperfection)
        {
        std::cout << "-> Tx Filter Order = " << SpectralDensity::TxFilterOrder <<
                  std::endl;
        std::cout << "-> Gaussian Filter Order = " << SpectralDensity::GaussianOrder <<
                  std::endl;
        }
    std::cout << "-> Network Type = " << NetworkTypesNicknames.left.at(
                  Type) << std::endl;
    std::cout << "-> Distance Between Inline Amplifiers = " << T->AvgSpanLength <<
              std::endl;
    std::cout << "-> Routing Algorithm = " <<
              ROUT::RoutingAlgorithm::RoutingAlgorithmNames.left.at(Routing_Algorithm)
              << std::endl;
    std::cout << "-> Wavelength Assignment Algorithm = " <<
              SA::SpectrumAssignmentAlgorithm::SpectrumAssignmentAlgorithmNames.left.at(
                  WavAssign_Algorithm)
              << std::endl;
    if(Type == TranslucentNetwork)
        {
        std::cout << "-> Regenerator Placement Algorithm = " <<
                  RegeneratorPlacementAlgorithm::RegeneratorPlacementNames.left.at(
                      RegPlacement_Algorithm) << std::endl;
        std::cout << "-> Regenerator Assignment Algorithm = " <<
                  RegeneratorAssignmentAlgorithm::RegeneratorAssignmentNames.left.at(
                      RegAssignment_Algorithm) << std::endl;
        }
    std::cout << "-> Minimum PSR Exponent = " << NMin << std::endl;
    std::cout << "-> Maximum PSR Exponent = " << NMax << std::endl;

    std::cout << "-> PSR Costs =" << std::endl;
    for(auto &cost : Costs)
        {
        std::cout << "\t-> " << Cost::CostsNames.left.at(cost->Type) <<
                  std::endl;
        }

    std::cout << "-> PSR Variant = " << PowerSeriesRouting::VariantNames.left.at(
                  Variant)
              << std::endl;
    std::cout << "-> Number of Calls = " << NumCalls << std::endl;
    std::cout << "-> Network Load = " << OptimizationLoad << std::endl;
}

void Simulation_PSROptimization::run()
{
    if (!hasLoaded)
        {
        load();
        }

    runPSR();

    // Saving Sim. Configurations
    std::string ConfigFileName = "SimConfigFile.ini"; // Name of the file
    save(ConfigFileName);
}

double Simulation_PSROptimization::Fitness::operator()(
    std::shared_ptr<PSO_Particle<double>> particle)
{

    //Creates a copy of the topology.
    auto TopologyCopy = std::make_shared<Topology>(*T);

    //Creates the RMSA Algorithms
    auto R_Alg = ROUT::RoutingAlgorithm::create_RoutingAlgorithm(Routing_Algorithm,
                 ROUT::RoutingCost::matPSR, TopologyCopy, false); //first with Matricial PSR. Changes later.
    auto RCost = PowerSeriesRouting::createPSR(TopologyCopy, Costs, Variant);

    std::shared_ptr<SA::SpectrumAssignmentAlgorithm> WA_Alg =
        SA::SpectrumAssignmentAlgorithm::create_SpectrumAssignmentAlgorithm(
            Simulation_PSROptimization::WavAssign_Algorithm, TopologyCopy);
    std::shared_ptr<RegeneratorAssignmentAlgorithm> RA_Alg;

    if (Simulation_PSROptimization::Type == TranslucentNetwork)
        {
        RA_Alg = RegeneratorAssignmentAlgorithm::create_RegeneratorAssignmentAlgorithm(
                     Simulation_PSROptimization::RegAssignment_Algorithm, TopologyCopy);
        }
    else
        {
        RA_Alg = nullptr;
        }

    //Initializes routing cost with the particle.
    RCost->initCoefficients(particle.get()->X);
    //Sets the routing algorithm with the PSR cost.
    R_Alg->RCost = RCost;

    //Creates the Call Generator and the RMSA Object
    auto Generator = std::make_shared<CallGenerator>(TopologyCopy,
                     Simulation_PSROptimization::OptimizationLoad,
                     TransmissionBitrate::DefaultBitrates);
    auto RMSA = std::make_shared<RoutingWavelengthAssignment> (R_Alg, WA_Alg,
                RA_Alg, ModulationScheme::DefaultSchemes, TopologyCopy);

    return
        NetworkSimulation(Generator, RMSA, Simulation_PSROptimization::NumCalls).
        get_CallBlockingProbability();
}

void Simulation_PSROptimization::printCoefficients(std::string file,
        bool override)
{
    std::ofstream OutFile;

    if (override)
        {
        OutFile.open(file);
        }
    else
        {
        OutFile.open(file, std::ofstream::out | std::ofstream::app);
        }

    OutFile << "  [PSR]" << std::endl << std::endl;
    OutFile << "minexponent = " << NMin << std::endl;
    OutFile << "maxexponent = " << NMax << std::endl;
    OutFile << "variant = " << PowerSeriesRouting::VariantNicknames.left.at
            (Variant) << std::endl;
    OutFile << "costs =";
        {
        for (auto &cost : Costs)
            {
            OutFile << " " << Cost::CostsNicknames.left.at(cost->Type);
            }
        OutFile << std::endl;
        }
    OutFile << "bestfit = " << BestParticle->bestFit << std::endl;
    OutFile << "coefficients =";
        {
        for (auto &coef : BestParticle->P)
            {
            OutFile << " " << std::setprecision(15) << coef;
            }
        OutFile << std::endl;
        }
}

void Simulation_PSROptimization::runPSR()
{
    if (!hasRun)
        {
        Fitness::T = T;
        Fitness::Variant = Variant;

        switch (Variant)
            {
            case PowerSeriesRouting::Variant_MatricialPSR:
                PSO_Optim =
                    std::shared_ptr<ParticleSwarmOptimization<double, Fitness, Compare>>
                    (new ParticleSwarmOptimization<double, Fitness, Compare>
                     (P, G, std::pow(NMax - NMin + 1, Costs.size()), XMin, XMax, VMin, VMax));
                break;

            case PowerSeriesRouting::Variant_TensorialPSR:
                PSO_Optim =
                    std::shared_ptr<ParticleSwarmOptimization<double, Fitness, Compare>>
                    (new ParticleSwarmOptimization<double, Fitness, Compare>
                     (P, G, Costs.size() * (NMax - NMin + 1), XMin, XMax, VMin, VMax));
                break;

            case PowerSeriesRouting::Variant_LocalPSR:
                PSO_Optim =
                    std::shared_ptr<ParticleSwarmOptimization<double, Fitness, Compare>>
                    (new ParticleSwarmOptimization<double, Fitness, Compare>
                     (P, G, std::pow
                      (NMax - NMin + 1, Costs.size()) * T->Nodes.size() * (T->Nodes.size() - 1),
                      XMin, XMax, VMin, VMax));
                break;

            case PowerSeriesRouting::Variant_AWR:
                PSO_Optim =
                    std::shared_ptr<ParticleSwarmOptimization<double, Fitness, Compare>>
                    (new ParticleSwarmOptimization<double, Fitness, Compare>
                     (P, G, Costs.size() - 1, 0, std::acos(-1), VMin, VMax));
                break;
            }
        }

    std::cout << std::endl << "* * RESULTS * *" << std::endl;

    std::ofstream logFile(LogFilename);

    for (unsigned i = 1; i <= G; i++)
        {
        if (!hasRun && (!BestParticle || BestParticle->bestFit != 0.0))
            {
            PSO_Optim->run_generation();
            }

        BestParticle = PSO_Optim->BestParticle;

        std::cout << "GENERATION\tCALL BLOCKING PROBABILITY" << std::endl;
        std::cout << i << "\t\t" << BestParticle->bestFit << std::endl;
        logFile << i << "\t\t" << BestParticle->bestFit << std::endl;
        printCoefficients(CoefficientsFilename);
        }

    hasRun = true;
}
