#include <Structure/Topology.h>
#include <gtest/gtest.h>

#include <boost/assign.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <Structure/Link.h>

Topology::DefaultTopNamesBimap Topology::DefaultTopologiesNames =
    boost::assign::list_of<Topology::DefaultTopNamesBimap::relation>
#define X(a,b,c) (a,b)
    DEFAULT_TOPOLOGIES
#undef X
    ;

Topology::DefaultTopPathsBimap Topology::DefaultTopologiesPaths =
    boost::assign::list_of<Topology::DefaultTopPathsBimap::relation>
#define X(a,b,c) (a,c)
    DEFAULT_TOPOLOGIES
#undef X
#undef DEFAULT_TOPOLOGIES
    ;

Topology::Topology() : PowerRatioThreshold(0.6, Gain::Linear)
{
    Nodes.clear();
    Links.clear();
    AvgSpanLength = LongestLink = -1;
}

Topology::Topology(const Topology &topology) : PowerRatioThreshold(0.6, Gain::Linear)
{
    Nodes.clear();
    Links.clear();
    LongestLink = -1;
    AvgSpanLength = topology.AvgSpanLength;

    for (auto &node : topology.Nodes)
        {
        add_Node(node->ID,
                 node->get_NodeType(),
                 node->get_NodeArch(),
                 node->get_NumRegenerators());
        }

    for (auto &link : topology.Links)
        {
        std::weak_ptr<Node> orig = Nodes.front();
        std::weak_ptr<Node> dest = Nodes.front();

        for (auto node = Nodes.begin(); node != Nodes.end(); ++node)
            {
            if (*(*node) == *(link.second->Origin.lock()))
                {
                orig = *node;
                }

            if (*(*node) == *(link.second->Destination.lock()))
                {
                dest = *node;
                }
            }

        add_Link(orig, dest, link.second->Length);
        }
}

Topology::Topology(std::string TopologyFileName) : PowerRatioThreshold(0.6, Gain::Linear)
{
    using namespace boost::program_options;

    Nodes.clear();
    Links.clear();
    AvgSpanLength = LongestLink = -1;

    options_description TopologyDescription("Topology");
    TopologyDescription.add_options()
    ("nodes.node", value<std::vector<std::string>>()->multitoken(),
     "Node Description")
    ("links.->", value<std::vector<std::string>>()->multitoken(),
     "Unidirectional Link Description");

    variables_map VariablesMap;

    store(parse_config_file<char>(TopologyFileName.c_str(), TopologyDescription,
                                  true), VariablesMap);

    //Reads nodes from configuration file.
    std::vector<std::string> NodesList =
        VariablesMap.find("nodes.node")->second.as<std::vector<std::string>>();

    for (auto &node : NodesList)
        {
        int NodeId, NumReg;
        std::string StrType, StrArch;

        std::istringstream NodeParameters(node);
        NodeParameters >> NodeId >> StrType >> StrArch >> NumReg;

        Node::NodeType Type = Node::NodeTypes.right.at(StrType);
        Node::NodeArchitecture Arch = Node::NodeArchitecturesNicknames.
                                      right.at(StrArch);

        add_Node(NodeId, Type, Arch, NumReg);
        }

    //Reads links from configuration file.
    std::vector<std::string> LinksList =
        VariablesMap.find("links.->")->second.as<std::vector<std::string>>();

    for (auto &link : LinksList)
        {
        int OriginID, DestinationID;
        double length;
        std::weak_ptr<Node> Origin, Destination;

        std::istringstream LinkParameters(link);
        LinkParameters >> OriginID >> DestinationID >> length;
        EXPECT_NE(OriginID, DestinationID) <<
                                           "Link can't have the same Origin and Destination.";

        int NodesFound = 0;

        for (auto &node : Nodes)
            {
            if (node->ID == OriginID)
                {
                Origin = node;
                NodesFound++;
                }

            if (node->ID == DestinationID)
                {
                Destination = node;
                NodesFound++;
                }
            }

        EXPECT_EQ(NodesFound, 2) << "Link with invalid origin and/or destination.";

        add_Link(Origin, Destination, length);
        }
}

std::weak_ptr<Node> Topology::add_Node(int NodeID, Node::NodeType Type,
                                       Node::NodeArchitecture Arch, int NumReg)
{
    if (NodeID == -1)
        {
        NodeID = Nodes.size() + 1;
        }

    Nodes.push_back(std::make_shared<Node>(NodeID, Type, Arch));
    Nodes.back()->set_NumRegenerators(NumReg);
    return (std::weak_ptr<Node>) Nodes.back();
}

std::weak_ptr<Link> Topology::add_Link(std::weak_ptr<Node> Origin,
                                       std::weak_ptr<Node> Destination, double Length)
{
    std::shared_ptr<Link> link = std::make_shared<Link>(Origin, Destination, Length);

    Links.emplace(std::make_pair(Origin.lock()->ID, Destination.lock()->ID), link);
    Origin.lock()->insert_Link(Destination, link);
    LongestLink = -1;
    if (AvgSpanLength != -1)
        {
        link->set_AvgSpanLength(AvgSpanLength);
        }

    return link;
}

void Topology::save(std::string TopologyFileName)
{
    std::ofstream TopologyFile(TopologyFileName,
                               std::ofstream::out | std::ofstream::app);

    EXPECT_TRUE(TopologyFile.is_open()) << "Output file is not open";

    TopologyFile << "  [nodes]" << std::endl << std::endl;
    TopologyFile << "# node = ID TYPE ARCHITECTURE NUMREG" << std::endl;


    for (auto &it : Nodes)
        {
        TopologyFile << "  node = " << it->ID
                     << " " << Node::NodeTypes.left.at(it->get_NodeType())
                     << " " << Node::NodeArchitecturesNicknames.left.at(it->get_NodeArch())
                     << " " << it->get_NumRegenerators() << std::endl;
        }

    TopologyFile << std::endl;

    TopologyFile << "  [links]" << std::endl << std::endl;
    TopologyFile << "# -> = ORIGIN DESTINATION LENGTH" << std::endl;

    for (auto it : Links)
        {
        TopologyFile << "  -> = " << it.second->Origin.lock().get()->ID << " " <<
                     it.second->Destination.lock().get()->ID << " " <<
                     it.second->Length << std::endl;
        }

}

double Topology::get_LengthLongestLink()
{
    if (LongestLink == -1)
        {
        for (auto &link : Links)
            {
            if (LongestLink < link.second->Length)
                {
                LongestLink = link.second->Length;
                }
            }
        }

    return LongestLink;
}

std::shared_ptr<Topology>
Topology::create_DefaultTopology(DefaultTopologies Top)
{
    return std::shared_ptr<Topology>(new Topology(
                                         Topology::DefaultTopologiesPaths.left.at(Top)));
}

double Topology::get_CapEx()
{
    double CapEx = 0;

    for (auto node : Nodes)
        {
        CapEx += node->get_CapEx();
        }

    for (auto link : Links)
        {
        CapEx += link.second->get_CapEx();
        }

    return CapEx;
}

double Topology::get_OpEx()
{
    double OpEx = 0;

    for (auto node : Nodes)
        {
        OpEx += node->get_OpEx();
        }

    for (auto link : Links)
        {
        OpEx += link.second->get_OpEx();
        }

    return OpEx;
}

unsigned long Topology::get_NumRegenerators()
{
    unsigned long NReg = 0;

    for (auto &node : Nodes)
        {
        NReg += node->get_NumRegenerators();
        }

    return NReg;
}

unsigned long Topology::get_NumTranslucentNodes()
{
    unsigned long numTranslucentNodes = 0;

    for (auto &node : Nodes)
        {
        numTranslucentNodes += (node->get_NumRegenerators() != 0);
        }

    return numTranslucentNodes;
}

void Topology::set_avgSpanLength(double avgSpanLength)
{
    for (auto &link : Links)
        {
        link.second->set_AvgSpanLength(avgSpanLength);
        }
    AvgSpanLength = avgSpanLength;
}

Gain Topology::get_PowerRatioThreshold()
{
    return PowerRatioThreshold;
}

void Topology::set_PowerRatioThreshold(Gain PRThreshold)
{
    PowerRatioThreshold = PRThreshold;
}

void Topology::print()
{
    std::cout << "-> Topology = " << std::endl;

    for (auto &it : Nodes)
        {
        std::cout << "\t (" << it->ID
                  << ")\t" << Node::NodeArchitecturesNames.left.at(it->get_NodeArch())
                  << "\t" << it->get_NumRegenerators() << " Regenerators" << std::endl;
        }

    std::cout << std::endl;
}
