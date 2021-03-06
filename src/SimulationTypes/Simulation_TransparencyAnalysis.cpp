#include <SimulationTypes/Simulation_TransparencyAnalysis.h>
#include <RMSA/RoutingAlgorithms/Costs/ShortestPath.h>
#include <RMSA/RoutingAlgorithms/Algorithms/Dijkstra_RoutingAlgorithm.h>
#include <RMSA/SpectrumAssignmentAlgorithms/FirstFit.h>
#include <RMSA/RoutingWavelengthAssignment.h>
#include <RMSA/Route.h>
#include <Structure/Link.h>
#include <Calls/Call.h>
#include <GeneralClasses/Signal.h>
#include <algorithm>
#include <boost/assign.hpp>
#include <boost/program_options.hpp>
#include <map>

using namespace RMSA;
using namespace Simulations;
using namespace ROUT;
using namespace SA;

Simulation_TransparencyAnalysis::Simulation_TransparencyAnalysis() :
    SimulationType(Simulation_Type::transparency)
{
    hasLoaded = hasRun = false;
    minModulationScheme = std::make_shared<ModulationScheme>(*
                          (ModulationScheme::DefaultSchemes.begin()));
    maxBitrate = std::make_shared<TransmissionBitrate>(*std::max_element(
                     TransmissionBitrate::DefaultBitrates.begin(),
                     TransmissionBitrate::DefaultBitrates.end()));
}

void Simulation_TransparencyAnalysis::help()
{
    std::cout << "\t\tTRANSPARENCY ANALYSIS" << std::endl << std::endl <<
              "This simulation varies the average distance between inline amplifiers "
              " and the input OSNR, and checks whether it's still possible to "
              " establish a connection in the route with longest length found by"
              " the Shortest Path routing algorithm, using the greatest possible "
              " bitrate and any modulation scheme. If it's possible to establish "
              " such connection, then the network is said to be \"transparent\"."
              << std::endl;
}

void Simulation_TransparencyAnalysis::run()
{
    if (!hasLoaded)
        {
        load();
        }

    for (double avgSpan = minAvgLinkSpan; avgSpan <= maxAvgLinkSpan;
            avgSpan += stepAvgLinkSpan)
        {
        for (double inOSNR = minOSNR; inOSNR <= maxOSNR; inOSNR += stepOSNR)
            {
            Signal::InputOSNR = Gain(inOSNR, Gain::dB);

            std::shared_ptr<Topology> TopCopy(new Topology(*T));
            TopCopy->set_avgSpanLength(avgSpan);
            std::weak_ptr<Node> origin, destination;

            for (auto node : TopCopy->Nodes)
                {
                if (node->ID == Orig_Origin->ID)
                    {
                    origin = node;
                    }

                if (node->ID == Orig_Destination->ID)
                    {
                    destination = node;
                    }
                }

            auto SP = RMSA::ROUT::RoutingAlgorithm::create_RoutingAlgorithm(
                          RoutingAlgorithm::dijkstra, RoutingCost::SP, TopCopy);
            std::shared_ptr<SA::FirstFit> FF(new SA::FirstFit(TopCopy));

            RoutingWavelengthAssignment RMSA(SP, FF, ModulationScheme::DefaultSchemes,
                                             TopCopy);
            std::shared_ptr<Call> C(new Call(origin, destination, *maxBitrate));

            if (!RMSA.routeCall(C)->Slots.empty())
                {
                TransparentPoints.push_back(InLineDistance_OSNR_Point(avgSpan, inOSNR));
                }
            else
                {
                OpaquePoints.push_back(InLineDistance_OSNR_Point(avgSpan, inOSNR));
                }
            }
        }

    std::ofstream TransparentFile((FileName + "_TR").c_str());
    std::ofstream OpaqueFile((FileName + "_OP").c_str());

    std::cout << std::endl << "* * RESULTS * *" << std::endl;

    if (!TransparentPoints.empty())
        {
        std::cout << std::endl << "Transparent Points: " << std::endl;

        for (auto &pair : TransparentPoints)
            {
            std::cout << "[" << pair.first << "km, " << pair.second << "dB] ";
            TransparentFile << pair.first << "\t" << pair.second << std::endl;
            }
        }

    if (!OpaquePoints.empty())
        {
        std::cout << std::endl << "Opaque Points: " << std::endl;

        for (auto &pair : OpaquePoints)
            {
            std::cout << "[" << pair.first << "km, " << pair.second << "dB] ";
            OpaqueFile << pair.first << "\t" << pair.second << std::endl;
            }
        }

    hasRun = true;

    // Saving Sim. Configurations
    std::string ConfigFileName = "SimConfigFile.ini"; // Name of the file
    save(ConfigFileName);
}

void Simulation_TransparencyAnalysis::load()
{
    //Generic readings.
    SimulationType::load();

    std::cout << std::endl <<
              "-> Define the minimum average distance between in-line amplifiers" <<
              std::endl;

    do
        {
        std::cin >> minAvgLinkSpan;

        if (std::cin.fail() || minAvgLinkSpan < 0)
            {
            std::cin.clear();
            std::cin.ignore();

            std::cerr << "Invalid distance." << std::endl;
            std::cout << std::endl <<
                      "-> Define the minimum average distance between in-line amplifiers" <<
                      std::endl;
            }
        else
            {
            break;
            }
        }
    while (1);

    std::cout << std::endl <<
              "-> Define the maximum average distance between in-line amplifiers"
              << std::endl;

    do
        {
        std::cin >> maxAvgLinkSpan;

        if (std::cin.fail() || maxAvgLinkSpan < minAvgLinkSpan)
            {
            std::cin.clear();
            std::cin.ignore();

            std::cerr << "Invalid distance." << std::endl;
            std::cout << std::endl <<
                      "-> Define the maximum average distance between in-line amplifiers"
                      << std::endl;
            }
        else
            {
            break;
            }
        }
    while (1);

    std::cout << std::endl <<
              "-> Define the average distance between in-line amplifiers step."
              << std::endl;

    do
        {
        std::cin >> stepAvgLinkSpan;

        if (std::cin.fail() || stepAvgLinkSpan < 0)
            {
            std::cin.clear();
            std::cin.ignore();

            std::cerr << "Invalid distance." << std::endl;
            std::cout << std::endl <<
                      "-> Define the average distance between in-line amplifiers step."
                      << std::endl;
            }
        else
            {
            break;
            }
        }
    while (1);

    std::cout << std::endl << "-> Define the minimum input OSNR" << std::endl;

    do
        {
        std::cin >> minOSNR;

        if (std::cin.fail())
            {
            std::cin.clear();
            std::cin.ignore();

            std::cerr << "Invalid OSNR." << std::endl;
            std::cout << "-> Define the minimum input OSNR" << std::endl;
            }
        else
            {
            break;
            }
        }
    while (1);

    std::cout << std::endl << "-> Define the maximum input OSNR" << std::endl;

    do
        {
        std::cin >> maxOSNR;

        if (std::cin.fail() || maxOSNR < minOSNR)
            {
            std::cin.clear();
            std::cin.ignore();

            std::cerr << "Invalid OSNR." << std::endl;
            std::cout << "-> Define the maximum input OSNR" << std::endl;
            }
        else
            {
            break;
            }
        }
    while (1);

    std::cout << std::endl << "-> Define the input OSNR step." << std::endl;

    do
        {
        std::cin >> stepOSNR;

        if (std::cin.fail() || stepOSNR < 0)
            {
            std::cin.clear();
            std::cin.ignore();

            std::cerr << "Invalid OSNR." << std::endl;
            std::cout << "-> Define the input OSNR step." << std::endl;
            }
        else
            {
            break;
            }
        }
    while (1);

    std::cout << std::endl << "-> Define the file where to store the results."
              << std::endl;
    do
        {
        std::cin >> FileName;

        if (std::cin.fail())
            {
            std::cin.clear();
            std::cin.ignore();

            std::cerr << "Invalid filename." << std::endl;
            std::cout << std::endl << "-> Define the file where to store the results."
                      << std::endl;
            }
        else
            {
            break;
            }
        }
    while (1);

    find_OriginDestination();

    hasLoaded = true;
}

void Simulation_TransparencyAnalysis::load_file(std::string ConfigFileName)
{
    using namespace boost::program_options;

    options_description ConfigDesctription("Configurations Data");
    ConfigDesctription.add_options()("general.SimulationType",
                                     value<std::string>()->required(), "Simulation Type")
    ("distance.minAvgLinkSpan", value<long double>()->required(),
     "Min. Avg. Dist. between in-line Amps.")
    ("distance.maxAvgLinkSpan", value<long double>()->required(),
     "Max. Avg. Dist. between in-line Amps.")
    ("distance.stepAvgLinkSpan", value<long double>()->required(),
     "Avg. Dist. between in-line Amps. Step")
    ("osnr.minOSNR", value<long double>()->required(), "Min. OSNR")
    ("osnr.maxOSNR", value<long double>()->required(), "Max. OSNR")
    ("osnr.stepOSNR", value<long double>()->required(), "OSNR Step");

    variables_map VariablesMap;

    std::ifstream ConfigFile(ConfigFileName, std::ifstream::in);
#ifdef RUN_ASSERTIONS
    if (!ConfigFile.is_open())
        {
        std::cerr << "Input file is not open" << std::endl;
        abort();
        }
#endif
    store(parse_config_file<char>(ConfigFile, ConfigDesctription, true),
          VariablesMap);
    ConfigFile.close();
    notify(VariablesMap);

    T = std::shared_ptr<Topology>(new Topology(ConfigFileName));
    minAvgLinkSpan = VariablesMap["distance.minAvgLinkSpan"].as<long double>();
    maxAvgLinkSpan = VariablesMap["distance.maxAvgLinkSpan"].as<long double>();
    stepAvgLinkSpan = VariablesMap["distance.stepAvgLinkSpan"].as<long double>();
    minOSNR = VariablesMap["osnr.minOSNR"].as<long double>();
    maxOSNR = VariablesMap["osnr.maxOSNR"].as<long double>();
    stepOSNR = VariablesMap["osnr.stepOSNR"].as<long double>();

    find_OriginDestination();

    hasLoaded = true;
}

void Simulation_TransparencyAnalysis::save(std::string SimConfigFileName)
{
    SimulationType::save(SimConfigFileName);

    std::ofstream SimConfigFile(SimConfigFileName,
                                std::ofstream::out | std::ofstream::app);

#ifdef RUN_ASSERTIONS
    if (!SimConfigFile.is_open())
        {
        std::cerr << "Output file is not open" << std::endl;
        abort();
        }
#endif

    SimConfigFile << std::endl << "  [distance]" << std::endl << std::endl;
    SimConfigFile << "  minAvgLinkSpan = " << minAvgLinkSpan << std::endl;
    SimConfigFile << "  maxAvgLinkSpan = " << maxAvgLinkSpan << std::endl;
    SimConfigFile << "  stepAvgLinkSpan = " << stepAvgLinkSpan << std::endl;

    SimConfigFile << std::endl << "  [osnr]" << std::endl << std::endl;
    SimConfigFile << "  minOSNR = " << minAvgLinkSpan << std::endl;
    SimConfigFile << "  maxOSNR = " << maxAvgLinkSpan << std::endl;
    SimConfigFile << "  stepOSNR = " << stepAvgLinkSpan << std::endl;

    SimConfigFile << std::endl;
    T->save(SimConfigFileName);
}

void Simulation_TransparencyAnalysis::print()
{
    if (!hasLoaded)
        {
        load();
        }

    std::cout << std::endl <<
              "  A Transparency Analysis Simulation is about to start with the following parameters: "
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
    std::cout << "-> Minimum Average Span Length = " << minAvgLinkSpan << std::endl;
    std::cout << "-> Maximum Average Span Length = " << maxAvgLinkSpan << std::endl;
    std::cout << "-> Minimum OSNR = " << minAvgLinkSpan << std::endl;
    std::cout << "-> Maximum OSNR = " << maxAvgLinkSpan << std::endl;

    T->print();
}

void Simulation_TransparencyAnalysis::find_OriginDestination()
{
    auto R_Alg = RoutingAlgorithm::create_RoutingAlgorithm(
                     RoutingAlgorithm::dijkstra, RoutingCost::SP, T);

    double maxLength = -1;

    for (auto orig : T->Nodes)
        {
        for (auto dest : T->Nodes)
            {
            if (orig == dest)
                {
                continue;
                }

            std::shared_ptr<Call> DummyCall(new Call(orig, dest, 0));
            auto route = R_Alg->route(DummyCall).front();
            double length = 0;

            for (auto link : route)
                {
                length += link.lock()->Length;
                }

            if (maxLength < length)
                {
                maxLength = length;
                Orig_Origin = orig;
                Orig_Destination = dest;
                }
            }
        }
}
