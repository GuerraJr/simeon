#include <RWA/RoutingAlgorithms/PowerSeriesRouting/PowerSeriesRouting.h>
#include <iostream>
#include <algorithm>
#include <boost/program_options.hpp>
#include <boost/assign.hpp>
#include <RWA/RoutingAlgorithms/PowerSeriesRouting/Costs.h>
#include <RWA/RoutingAlgorithms/PowerSeriesRouting/PSRVariants.h>

arma::mat PowerSeriesRouting::defaultcoefficients;
bool PowerSeriesRouting::hasLoaded = false;
std::vector<std::shared_ptr<PSR::Cost>> PowerSeriesRouting::defaultcosts;

PowerSeriesRouting::VariantNameBimap PowerSeriesRouting::VariantNames =
    boost::assign::list_of<PowerSeriesRouting::VariantNameBimap::relation>
#define X(a,b,c) (a,b)
    PSRVARIANTS
#undef X
    ;

PowerSeriesRouting::VariantNicknameBimap
PowerSeriesRouting::VariantNicknames =
    boost::assign::list_of<PowerSeriesRouting::VariantNicknameBimap::relation>
#define X(a,b,c) (a,b)
    PSRVARIANTS
#undef X
    ;


PowerSeriesRouting::PowerSeriesRouting
(std::shared_ptr<Topology> T, Variants Variant) :
    DijkstraRoutingAlgorithm(T, PSR), PSRVariant(Variant)
{
    firstTimeRun = false;
}

PowerSeriesRouting::PowerSeriesRouting(std::shared_ptr<Topology> T,
                                       std::vector<std::shared_ptr<PSR::Cost>> Costs,
                                       Variants Variant) :
    DijkstraRoutingAlgorithm(T, RoutingAlgorithms::PSR), PSRVariant(Variant)
{

    firstTimeRun = false;
    hasLoaded = true;

    NMin = Costs.front()->get_NMin();
    NMax = Costs.front()->get_NMax();

    //Copies the costs types, but not the costs themselves.
    //It's therefore topology-independent.
    for (auto &cost : Costs)
        {
        this->Costs.push_back(PSR::Cost::createCost(cost->Type, NMin, NMax, T));
        }
}

void PowerSeriesRouting::load()
{
    if (hasLoaded)
        {
        coefficients = defaultcoefficients;
        NMin = defaultcosts.front()->get_NMin();
        NMax = defaultcosts.front()->get_NMax();

        for (auto &cost : defaultcosts)
            {
            Costs.push_back(PSR::Cost::createCost(cost->Type, NMin, NMax, T));
            }

        return;
        }

    std::cout << std::endl << "-> Define the PSR Coefficients filepath." <<
              std::endl;

    do
        {
        std::string Filename;

        std::cin >> Filename;

        if (std::cin.fail() || !initCoefficients(Filename))
            {
            std::cin.clear();
            std::cin.ignore();

            std::cerr << "Invalid coefficients." << std::endl;
            std::cout << std::endl << "-> Define the PSR Coefficients filepath."
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

bool PowerSeriesRouting::initCoefficients(std::vector<double> X)
{
    coefficients = arma::mat(1, X.size());
    int it = 0;

    for (auto &x : X)
        {
        coefficients(0, it++) = x;
        }

    return true;
}

bool PowerSeriesRouting::initCoefficients(std::string Filename)
{
    using namespace boost::program_options;

        {
        std::ifstream File(Filename);

        if (!File.is_open())
            {
            std::cerr << "Could not find file." << std::endl;
            return false;
            }
        }

    Costs.clear();

    options_description PSRDescription("PSR");
    PSRDescription.add_options()
    ("PSR.minexponent", value<int>(), "Minimum Exponent")
    ("PSR.maxexponent", value<int>(), "Maximum Exponent")
    ("PSR.variant", value<std::string>(), "PSR Variant")
    ("PSR.costs", value<std::string>(), "Chosen Costs")
    ("PSR.coefficients", value<std::string>(), "Coefficients");

    variables_map VariablesMap;

    store(parse_config_file<char>(Filename.c_str(), PSRDescription, true),
          VariablesMap);

    //Reads from configuration file.
    NMin = VariablesMap.find("PSR.minexponent")->second.as<int>();
    NMax = VariablesMap.find("PSR.maxexponent")->second.as<int>();
    PSRVariant = VariantNames.right.at(
                     VariablesMap.find("PSR.variant")->second.as<std::string>());

    std::clog << "Reading a PSR with min. exponent " << NMin
              << " and max. exponent " << NMax << "." << std::endl;

    if (NMax < NMin)
        {
        std::cerr << "The max. exponent must be greater than the min. exponent"
                  << std::endl;
        return false;
        }

    //Reads the PSR costs
        {
        std::istringstream PSRCosts(
            VariablesMap.find("PSR.costs")->second.as<std::string>());
        std::string currentcost;

        while (PSRCosts >> currentcost)
            {
            if (PSR::Cost::CostsNicknames.right.count(currentcost) == 0)
                {
                std::cerr << "Invalid PSR cost \"" << currentcost << "\"." << std::endl;
                return false;
                }

            Costs.push_back(
                PSR::Cost::createCost(
                    PSR::Cost::CostsNicknames.right.at(currentcost), NMin, NMax, T)
            );
            defaultcosts.push_back(PSR::Cost::createCost(
                                       PSR::Cost::CostsNicknames.right.at(currentcost), NMin, NMax, T)
                                  );
            }

        if (Costs.empty())
            {
            std::cerr << "No costs defined." << std::endl;
            return false;
            }

        std::clog << "Its costs are:" << std::endl;

        for (auto &cost : Costs)
            {
            std::clog << "  - " << PSR::Cost::CostsNames.left.at(cost->Type) << std::endl;
            }
        }

    //Reads the PSR Coefficients
        {
        std::istringstream PSRCoeffs(
            VariablesMap.find("PSR.coefficients")->second.as<std::string>());
        std::vector<double> read_coefficients;
        double currentcoef;

        while (PSRCoeffs >> currentcoef)
            {
            read_coefficients.push_back(currentcoef);
            }

        coefficients = arma::mat(1, read_coefficients.size());
        defaultcoefficients = arma::mat(1, read_coefficients.size());
        int it = 0;

        for (auto &x : read_coefficients)
            {
            coefficients(0, it) = defaultcoefficients(0, it) = x;
            it++;
            }
        }

    return true;
}

std::vector<std::shared_ptr<PSR::Cost>> PowerSeriesRouting::get_Costs()
{
    return Costs;
}

int PowerSeriesRouting::get_NMin() const
{
    return NMin;
}

int PowerSeriesRouting::get_NMax() const
{
    return NMax;
}

void PowerSeriesRouting::save(std::string SimConfigFileName)
{
    RoutingAlgorithm::save(SimConfigFileName);
}

std::shared_ptr<PowerSeriesRouting> PowerSeriesRouting::createPSR(
    std::shared_ptr<Topology> T, std::vector<std::shared_ptr<PSR::Cost> > Costs,
    Variants Variant)
{
    std::shared_ptr<PowerSeriesRouting> PSR;

    switch (Variant)
        {
#define X(a,b,c) case a: PSR = std::make_shared<c>(T, Costs); break;
            PSRVARIANTS
#undef X
#undef PSRVARIANTS
        }

    return PSR;
}
