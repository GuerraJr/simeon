#include <boost/assert.hpp>
#include <boost/assign.hpp>
#include <limits>
#include <Structure/Node.h>
#include <Structure/Link.h>
#include <Devices/Amplifiers/PreAmplifier.h>
#include <Devices/Amplifiers/BoosterAmplifier.h>
#include <Devices/Regenerator.h>
#include <Devices/SSS.h>
#include <Devices/Splitter.h>

Node::NodeArchitecture Node::Default_Arch;

Node::NodeTypeBimap Node::NodeTypes =
    boost::assign::list_of<Node::NodeTypeBimap::relation>
#define X(a,b) (a,b)
    NODETYPE
#undef X
#undef NODETYPE
    ;

Node::NodeArchNameBimap Node::NodeArchitecturesNames =
    boost::assign::list_of<Node::NodeArchNameBimap::relation>
#define X(a,b,c) (a,b)
    NODEARCH
#undef X
    ;

Node::NodeArchNicknameBimap Node::NodeArchitecturesNicknames =
    boost::assign::list_of<Node::NodeArchNicknameBimap::relation>
#define X(a,b,c) (a,c)
    NODEARCH
#undef X
#undef NODEARCH
    ;

Node::Node(int ID, NodeType T, NodeArchitecture A) : ID(ID), Type(T) ,
    Architecture(A)
{
    create_Devices();
    TotalNumRequestedRegenerators =
        MaxSimultUsedRegenerators =
            NumUsedRegenerators = 0;
}

Node::Node(const Node &node) : ID(node.ID)
{
    Type = node.Type;
    Architecture = node.Architecture;
    NumUsedRegenerators = 0;
    TotalNumRequestedRegenerators = MaxSimultUsedRegenerators = 0;

    for (auto &link : node.Links)
        {
        std::shared_ptr<Link> newlink = std::shared_ptr<Link>(new Link(*link));
        insert_Link(newlink->Destination, newlink);
        }

    for (auto &device : node.Devices)
        {
        Devices.push_back(device->clone());
        }

    for (unsigned i = 0; i < node.Regenerators.size(); i++)
        {
        Regenerators.push_back(std::shared_ptr<Device>(new Regenerator()));
        }

}

bool Node::operator ==(const Node &N) const
{
    return (ID == N.ID);
}

bool Node::operator <(const Node &N) const
{
    return (ID < N.ID);
}

void Node::insert_Link(std::weak_ptr<Node> N, std::shared_ptr<Link> Link)
{
    bool LinkExists = false;

    for (auto &it : Neighbours)
        {
        if (it.lock() == N.lock())
            {
            LinkExists = true;
            break;
            }
        }

    if (!LinkExists)
        {
        Neighbours.push_back(N);
        Links.push_back(Link);
        }
}

Node::NodeArchitecture Node::get_NodeArch()
{
    return Architecture;
}

Node::NodeType Node::get_NodeType()
{
    return Type;
}

unsigned int Node::get_NumRegenerators()
{
    if (Type == OpaqueNode)
        {
        return std::numeric_limits<unsigned int>::max();
        }

    return Regenerators.size();
}

unsigned int Node::get_NumAvailableRegenerators()
{
    if (Type == OpaqueNode)
        {
        return std::numeric_limits<unsigned int>::max();
        }

    return Regenerators.size() - NumUsedRegenerators;
}

void Node::create_Devices()
{
    //Switching element - entrance
    switch (Architecture)
        {
        case BroadcastAndSelect:
            Devices.push_back(std::shared_ptr<Device>(new Splitter(this)));
            break;

        case SwitchingSelect:
            Devices.push_back(std::shared_ptr<Device>(new SSS(this)));
            break;
        }

    //Switching element - exit
    Devices.push_back(std::shared_ptr<Device>(new SSS(this)));

    //Booster Amplifier
    Devices.push_back(std::shared_ptr<Device>(new BoosterAmplifier()));
}

Signal &Node::bypass(Signal &S)
{
    for (auto &it : Devices)
        {
        S *= it->get_Gain();
        S += it->get_Noise();
        S *= it->get_TransferFunction(S.numSlots);
        }

    return S;
}

Signal &Node::drop(Signal &S)
{
    for (auto &it : Devices)
        {
        S *= it->get_Gain();
        S += it->get_Noise();
        S *= it->get_TransferFunction(S.numSlots);

        if ((it->DevType == Device::SplitterDevice) ||
                (it->DevType == Device::SSSDevice))
            {
            break;
            }
        }

    return S;
}

Signal &Node::add(Signal &S)
{
    auto it = Devices.begin();

    while (((*it)->DevType != Device::SplitterDevice) &&
            ((*it)->DevType != Device::SSSDevice))
        {
        it++;
        }

    it++;

    for (; it != Devices.end(); ++it)
        {
        S *= (*it)->get_Gain();
        S += (*it)->get_Noise();
        S *= (*it)->get_TransferFunction(S.numSlots); // Talvez devessemos remover, parece estar considerando a mesma coisa 2 vezes
        }

    return S;
}

void Node::set_NumRegenerators(unsigned int NReg)
{
    Regenerators.clear();

    for (unsigned i = 0; i < NReg; i++)
        {
        Regenerators.push_back(std::shared_ptr<Device>(new Regenerator()));
        }

    NumUsedRegenerators = 0;
}

bool Node::hasAsNeighbour(std::weak_ptr<Node> N)
{
    for (auto &it : Neighbours)
        {
        if (N.lock() == it.lock())
            {
            return true;
            }
        }

    return false;
}

void Node::set_NodeType(NodeType T)
{
    Type = T;
}

void Node::request_Regenerators(unsigned int NReg)
{
    TotalNumRequestedRegenerators += NReg;

    BOOST_ASSERT_MSG((Type == OpaqueNode) ||
                     (NReg + NumUsedRegenerators <= Regenerators.size()),
                     "Request to more regenerators than available.");

    NumUsedRegenerators += NReg;
}

void Node::free_Regenerators(unsigned int NReg)
{
    BOOST_ASSERT_MSG(NumUsedRegenerators >= NReg,
                     "Freed more regenerators than available.");

    if (MaxSimultUsedRegenerators < NumUsedRegenerators)
        {
        MaxSimultUsedRegenerators = NumUsedRegenerators;
        }

    NumUsedRegenerators -= NReg;
}

unsigned int Node::get_NumMaxSimultUsedRegenerators()
{
    return MaxSimultUsedRegenerators;
}

unsigned long long Node::get_TotalNumRequestedRegenerators()
{
    return TotalNumRequestedRegenerators;
}

double Node::get_CapEx()
{
    double CapEx = 0;

    for (auto reg : Regenerators)
        {
        CapEx += reg->get_CapEx();
        }

    for (auto device : Devices)
        {
        //In the two architectures, each device in Devices is actually a
        //representation of each one of the N (Num. of Neighb.) device.
        CapEx += (Neighbours.size()) * device->get_CapEx();
        }

    return CapEx;
}

double Node::get_OpEx()
{
    double OpEx = 0;

    for (auto reg : Regenerators)
        {
        OpEx += reg->get_OpEx();
        }

    for (auto device : Devices)
        {
        //In the two architectures, each device in Devices is actually a
        //representation of each one of the N (Num. of Neighb.) device.
        OpEx += (Neighbours.size()) * device->get_OpEx();
        }

    return OpEx;
}
