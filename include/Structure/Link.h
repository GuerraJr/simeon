#ifndef LINK_H
#define LINK_H

#include <GeneralClasses/Signal.h>
#include <memory>
#include <vector>

class Node;
class Slot;
class Device;

class Link {
  public:
    constexpr static int NumSlots = 64;
    constexpr static long double AvgSpanLength = 80;

    Link(std::weak_ptr<Node> Origin, std::weak_ptr<Node> Destination, long double Length);

    std::weak_ptr<Node> Origin;
    std::weak_ptr<Node> Destination;
    long double Length;
    int numLineAmplifiers;

    Signal cross(Signal);

  private:
    void create_Slots();
    void create_Devices();
    std::vector<std::unique_ptr<Slot>> Slots;
    std::vector<std::unique_ptr<Device>> Devices;
};

#endif // LINK_H
