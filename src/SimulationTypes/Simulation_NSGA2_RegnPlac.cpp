#include <SimulationTypes/Simulation_NSGA2_RegnPlac.h>
#include <GeneralClasses/RandomGenerator.h>
#include <SimulationTypes/NetworkSimulation.h>
#include <GeneralClasses/TransmissionBitrate.h>
#include <GeneralClasses/ModulationScheme.h>
#include <Structure/Link.h>
#include <Calls/CallGenerator.h>
#include <RWA/RoutingWavelengthAssignment.h>
#include <algorithm>
#include <cmath>

unsigned int Simulation_NSGA2_RegnPlac::RegnMax;

Simulation_NSGA2_RegnPlac::Simulation_NSGA2_RegnPlac() : hasLoaded(false) {
    auto maxBitrate = std::max_element(TransmissionBitrate::DefaultBitrates.begin(),
                                       TransmissionBitrate::DefaultBitrates.end());
    auto maxScheme = std::max_element(ModulationScheme::DefaultSchemes.begin(),
                                      ModulationScheme::DefaultSchemes.end());
    int MaxNumSlots = maxScheme->get_NumSlots(*maxBitrate);
    RegnMax = std::floor(Link::NumSlots / MaxNumSlots) * std::ceil(
                  maxBitrate->get_Bitrate() / RegeneratorAssignmentAlgorithm::RegeneratorBitrate);
}

Simulation_NSGA2_RegnPlac::Param_CapEx::Param_CapEx(
    std::vector<int> gene, Simulation_NSGA2_RegnPlac &Sim) :
    NSGA2_Parameter(gene), Sim(Sim) {

}

double Simulation_NSGA2_RegnPlac::Param_CapEx::evaluate() {
    if (!isEvaluated) {
        Topology T(*Sim.T);

        for (unsigned int i = 0; i < gene.size(); i++) {
            if (gene[i] != 0) {
                T.Nodes[i]->set_NodeType(Node::TranslucentNode);
            } else {
                T.Nodes[i]->set_NodeType(Node::TransparentNode);
            }

            T.Nodes[i]->set_NumRegenerators(gene[i]);
        }

        CapEx = T.get_CapEx();
        isEvaluated = true;
    }

    return CapEx;
}

Simulation_NSGA2_RegnPlac::Param_OpEx::Param_OpEx(
    std::vector<int> gene, Simulation_NSGA2_RegnPlac &Sim) :
    NSGA2_Parameter(gene), Sim(Sim) {

}

double Simulation_NSGA2_RegnPlac::Param_OpEx::evaluate() {
    if (!isEvaluated) {
        Topology T(*Sim.T);

        for (unsigned int i = 0; i < gene.size(); i++) {
            if (gene[i] != 0) {
                T.Nodes[i]->set_NodeType(Node::TranslucentNode);
            } else {
                T.Nodes[i]->set_NodeType(Node::TransparentNode);
            }

            T.Nodes[i]->set_NumRegenerators(gene[i]);
        }

        OpEx = T.get_OpEx();
        isEvaluated = true;
    }

    return OpEx;
}

Simulation_NSGA2_RegnPlac::Param_BlockProb::Param_BlockProb(
    std::vector<int> gene, Simulation_NSGA2_RegnPlac &Sim) :
    NSGA2_Parameter(gene), Sim(Sim) {

}

double Simulation_NSGA2_RegnPlac::Param_BlockProb::evaluate() {
    if (!isEvaluated) {
        std::shared_ptr<Topology> T(new Topology(*Sim.T));

        for (unsigned int i = 0; i < gene.size(); i++) {
            if (gene[i] != 0) {
                T->Nodes[i]->set_NodeType(Node::TranslucentNode);
            } else {
                T->Nodes[i]->set_NodeType(Node::TransparentNode);
            }

            T->Nodes[i]->set_NumRegenerators(gene[i]);
        }

        std::shared_ptr<RoutingAlgorithm> R_Alg =
            RoutingAlgorithm::create_RoutingAlgorithm(Sim.Routing_Algorithm, T);
        std::shared_ptr<WavelengthAssignmentAlgorithm> WA_Alg =
            WavelengthAssignmentAlgorithm::create_WavelengthAssignmentAlgorithm(
                Sim.WavAssign_Algorithm, T);
        std::shared_ptr<RegeneratorAssignmentAlgorithm> RA_Alg =
            RegeneratorAssignmentAlgorithm::create_RegeneratorAssignmentAlgorithm(
                Sim.RegAssignment_Algorithm, T);

        //Creates the Call Generator and the RWA Object
        std::shared_ptr<CallGenerator> Generator(new CallGenerator(T,
                Sim.NetworkLoad));
        std::shared_ptr<RoutingWavelengthAssignment> RWA(
            new RoutingWavelengthAssignment(
                R_Alg, WA_Alg, RA_Alg, ModulationScheme::DefaultSchemes, T));

        BlockProb = NetworkSimulation(Generator, RWA, Sim.NumCalls)
                    .get_CallBlockingProbability();
        isEvaluated = true;
    }

    return BlockProb;
}

void Simulation_NSGA2_RegnPlac::Individual::createIndividual() {
    std::vector<int> IndivGene;

    for (unsigned int gene = 0; gene < Sim.T->Nodes.size(); gene++) {
        IndivGene.push_back(createGene(gene));
    }

    setGene(IndivGene);
}

int Simulation_NSGA2_RegnPlac::Individual::createGene(unsigned int) {
    std::uniform_real_distribution<double> isTranslucent(0, 1);

    if (isTranslucent(random_generator) < 0.5) {
        return std::uniform_int_distribution<unsigned int>
               (0, Simulation_NSGA2_RegnPlac::RegnMax)(random_generator);
    } else {
        return 0;
    }
}

std::shared_ptr<NSGA2_Individual>
Simulation_NSGA2_RegnPlac::Individual::clone() {
    std::shared_ptr<Individual> indiv(new Individual(Sim));
    indiv->setGene(Gene);
    return indiv;
}

void Simulation_NSGA2_RegnPlac::Individual::setGene(
    std::vector<int> gene) {
    Gene = gene;
    Parameters.clear();
    Parameters.push_back(std::shared_ptr<NSGA2_Parameter>(
                             new Param_CapEx(Gene, Sim)));
    Parameters.push_back(std::shared_ptr<NSGA2_Parameter>(
                             new Param_OpEx(Gene, Sim)));
    Parameters.push_back(std::shared_ptr<NSGA2_Parameter>(
                             new Param_BlockProb(Gene, Sim)));
}

Simulation_NSGA2_RegnPlac::Sim_NSGA2::Sim_NSGA2(Simulation_NSGA2_RegnPlac &Sim)
    : NSGA2(), Sim(Sim) {

}

void Simulation_NSGA2_RegnPlac::Sim_NSGA2::createInitialGeneration() {
    std::shared_ptr<NSGA2_Generation> gen(new NSGA2_Generation());

    for (unsigned i = 0; i < NSGA2::numIndiv; i++) {
        std::shared_ptr<Individual> indiv(new Individual(Sim));
        indiv->createIndividual();

        gen->operator +=(indiv);
    }

    evolution.push_back(gen);
}

void Simulation_NSGA2_RegnPlac::help() {
    std::cout << "\t\tMORP-3O REGENERATOR PLACEMENT" << std::endl << std::endl <<
              "This simulation uses the NSGA-2 genetic algorithm to find a"
              " suitable set of transparent and translucent nodes, in order to"
              " fit simultaneously call blocking probability, CapEx and OpEx"
              " constraints." << std::endl;
}

void Simulation_NSGA2_RegnPlac::save(std::string) {

}

void Simulation_NSGA2_RegnPlac::load_file(std::string) {

}

void Simulation_NSGA2_RegnPlac::print() {
    if (!hasLoaded) {
        load();
    }

    Sim_NSGA2 Optimization(*this);

    std::cout << std::endl << "* * RESULTS * *" << std::endl;
    std::cout << "FIRST PARETO FRONT OF I-TH GENERATION" << std::endl;

    while (Optimization.generation < NSGA2::numGen) {
        Optimization.run_Generation();
        std::cout << std::endl << "GENERATION " <<  Optimization.generation
                  << std::endl;
        Optimization.evolution.at(Optimization.generation - 1)->print();
    }
}

void Simulation_NSGA2_RegnPlac::load() {
    //Generic readings.
    SimulationType::load();

    Node::load();

    Link::load(T);

    //RWA Algorithms
    {
        //Routing Algorithm
        Routing_Algorithm = RoutingAlgorithm::define_RoutingAlgorithm();

        //Wavelength Assignment Algorithm
        WavAssign_Algorithm =
            WavelengthAssignmentAlgorithm::define_WavelengthAssignmentAlgorithm();

        //Regenerator Assignment Algorithm
        RegAssignment_Algorithm =
            RegeneratorAssignmentAlgorithm::define_RegeneratorAssignmentAlgorithm();
    }

    std::cout << std::endl << "-> Define the number of calls." << std::endl;

    do {
        std::cin >> NumCalls;

        if (std::cin.fail() || NumCalls < 0) {
            std::cin.clear();
            std::cin.ignore();

            std::cerr << "Invalid number of calls." << std::endl;
            std::cout << std::endl << "-> Define the number of calls." << std::endl;
        } else {
            break;
        }
    } while (1);

    std::cout << std::endl << "-> Define the network load." << std::endl;

    do {
        std::cin >> NetworkLoad;

        if (std::cin.fail() || NetworkLoad < 0) {
            std::cin.clear();
            std::cin.ignore();

            std::cerr << "Invalid network load." << std::endl;
            std::cout << std::endl << "-> Define the network load." << std::endl;
        } else {
            break;
        }
    } while (1);

    hasLoaded = true;
}

void Simulation_NSGA2_RegnPlac::run() {
    if (!hasLoaded) {
        load();
    }

    Sim_NSGA2 Optimization(*this);

    while (Optimization.generation < NSGA2::numGen) {
        Optimization.run_Generation();
    }
}
