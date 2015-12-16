#include <boost/assert.hpp>
#include <limits>
#include <Structure/Node.h>
#include <Structure/Link.h>
#include <Devices/Amplifiers/PreAmplifier.h>
#include <Devices/Amplifiers/BoosterAmplifier.h>
#include <Devices/SSS.h>
#include <Devices/Splitter.h>

Node::NodeTypeBimap Node::NodeTypes;
Node::NodeArchBimap Node::NodeArchitectures;

Node::Node(int ID, NodeType T, NodeArchitecure A) : ID(ID), Type(T) ,
    Architecture(A) {

#define X(a,b) NodeTypes.insert(NodeTypeBimap::value_type(a,b));
    NODETYPE
#undef X
#undef NODETYPE

#define X(a,b) NodeArchitectures.insert(NodeArchBimap::value_type(a,b));
    NODEARCH
#undef X
#undef NODEARCH

    create_Devices();
    TotalNumRequestedRegenerators =
        MaxSimultUsedRegenerators =
            NumUsedRegenerators = 0;
}

Node::Node(const Node &node) : ID(node.ID) {
    Type = node.Type;
    Architecture = node.Architecture;
    NumRegenerators = node.NumRegenerators;
    NumUsedRegenerators = 0;
    TotalNumRequestedRegenerators = MaxSimultUsedRegenerators = 0;

    for (auto link : node.Links) {
        std::shared_ptr<Link> newlink = std::shared_ptr<Link>(new Link(*link));
        insert_Link(newlink->Destination, newlink);
    }

    for (auto device : node.Devices) {
        Devices.push_back(device->clone());
    }
}

bool Node::operator ==(const Node &N) const {
    return (ID == N.ID);
}

bool Node::operator <(const Node &N) const {
    return (ID < N.ID);
}

void Node::insert_Link(std::weak_ptr<Node> N, std::shared_ptr<Link> Link) {
    bool LinkExists = false;

    for (auto it : Neighbours) {
        if (it.lock() == N.lock()) {
            LinkExists = true;
            break;
        }
    }

    if (!LinkExists) {
        Neighbours.push_back(N);
        Links.push_back(Link);

        if (Architecture == BroadcastAndSelect) {
            BOOST_ASSERT_MSG((*Devices.front()).DevType == Device::SplitterDevice,
                             "In a B&S node, the first device is a spliiter.");
            static_cast<Splitter &>(*Devices.front()).set_NumPorts(Links.size());
        }
    }
}

Node::NodeArchitecure Node::get_NodeArch() {
    return Architecture;
}

Node::NodeType Node::get_NodeType() {
    return Type;
}

unsigned int Node::get_NumRegenerators() {
    if (Type == OpaqueNode) {
        return std::numeric_limits<unsigned int>::max();
    }

    return NumRegenerators;
}

unsigned int Node::get_NumAvailableRegenerators() {
    if (Type == OpaqueNode) {
        return std::numeric_limits<unsigned int>::max();
    }

    return NumRegenerators - NumUsedRegenerators;
}

void Node::create_Devices() {
    //Switching element - entrance
    switch (Architecture) {
        case BroadcastAndSelect:
            Devices.push_back(std::shared_ptr<Device>(new Splitter(Links.size())));
            break;

        case SwitchingSelect:
            Devices.push_back(std::shared_ptr<Device>(new SSS()));
            break;
    }

    //Switching element - exit
    Devices.push_back(std::shared_ptr<Device>(new SSS()));

    //Booster Amplifier
    Devices.push_back(std::shared_ptr<Device>(new BoosterAmplifier()));
}

Signal &Node::bypass(Signal &S) {
    for (auto it = Devices.begin(); it != Devices.end(); ++it) {
        S *= (*it)->get_Gain();
        S += (*it)->get_Noise();
    }

    return S;
}

Signal &Node::drop(Signal &S) {
    for (auto it : Devices) {
        S *= it->get_Gain();
        S += it->get_Noise();

        if ((it->DevType == Device::SplitterDevice) ||
                (it->DevType == Device::SSSDevice)) {
            break;
        }
    }

    return S;
}

Signal &Node::add(Signal &S) {
    auto it = Devices.begin();

    while (((*it)->DevType != Device::SplitterDevice) &&
            ((*it)->DevType != Device::SSSDevice)) {
        it++;
    }

    it++;

    for (; it != Devices.end(); ++it) {
        S *= (*it)->get_Gain();
        S += (*it)->get_Noise();
    }

    return S;
}

void Node::set_NumRegenerators(unsigned int NReg) {
    NumRegenerators = NReg;
    NumUsedRegenerators = 0;
}

bool Node::hasAsNeighbour(std::weak_ptr<Node> N) {
    for (auto it : Neighbours) {
        if (N.lock() == it.lock()) {
            return true;
        }
    }

    return false;
}

void Node::set_NodeType(NodeType T) {
    Type = T;
}

void Node::request_Regenerators(unsigned int NReg) {
    TotalNumRequestedRegenerators += NReg;

    BOOST_ASSERT_MSG((Type == OpaqueNode) ||
                     (NReg + NumUsedRegenerators <= NumRegenerators),
                     "Request to more regenerators than available.");

    NumUsedRegenerators += NReg;
}

void Node::free_Regenerators(unsigned int NReg) {
    BOOST_ASSERT_MSG(NumUsedRegenerators >= NReg,
                     "Freed more regenerators than available.");

    if (MaxSimultUsedRegenerators < NumUsedRegenerators) {
        MaxSimultUsedRegenerators = NumUsedRegenerators;
    }

    NumUsedRegenerators -= NReg;
}

unsigned int Node::get_NumMaxSimultUsedRegenerators() {
    return MaxSimultUsedRegenerators;
}

unsigned long long Node::get_TotalNumRequestedRegenerators() {
    return TotalNumRequestedRegenerators;
}
