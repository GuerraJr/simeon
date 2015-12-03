#ifndef NODE_H
#define NODE_H

#include <memory>
#include <vector>
#include <GeneralClasses/Signal.h>

class Link;
class Device;

class Node {
  public:
    /**
    * @brief The Node_Type enum is used to set the type of node.
    *
    * Use TransparentNode if the node is transparent, with no regenerators, TranslucentNode if the node is
    * translucent, with a limited amount of regenerators, or OpaqueNode if the node is opaque, that is,
    * has a infinity amount of regenerators.
    */
    enum Node_Type {
        TransparentNode, TranslucentNode, OpaqueNode
    };
    /**
     * @brief The Node_Architecure enum is used to set the type of node.
     *
     * There are two architectures, Broadcast And Select, that uses splitters and SSS, and Switching Select,
     * that only uses SSS devices.
     */
    enum Node_Architecure {
        BroadcastAndSelect, SwitchingSelect
    };

    Node(int ID, Node_Type T = TransparentNode,
         Node_Architecure A = SwitchingSelect, int NumRegenerators = 0);

    bool operator==(Node *);

    const int ID;
    std::vector<Node *> Neighbours;
    std::vector<std::shared_ptr<Link>> Links;
    std::vector<std::unique_ptr<Device>> Devices;

    Node_Architecure get_NodeArch();
    void insert_Link(Node *N, std::shared_ptr<Link> Link);

    Signal cross(Signal);
    Signal enter(Signal);
    Signal exit(Signal);
  private:
    Node_Type Type;
    Node_Architecure Architecture;
    void create_Devices();
    int NumRegenerators;
};

#endif // NODE_H
