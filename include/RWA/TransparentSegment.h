#ifndef TRANSPARENTSEGMENT_H
#define TRANSPARENTSEGMENT_H

#include <memory>
#include <vector>
#include <map>
#include <GeneralClasses/Signal.h>
#include <GeneralClasses/ModulationScheme.h>

class Link;
class Node;
class Slot;

class TransparentSegment {
  public:
    TransparentSegment(std::vector<std::weak_ptr<Link>> Links,
                       ModulationScheme ModScheme,
                       unsigned int NumRegUsed = 0);
    TransparentSegment(const TransparentSegment &segment);

    std::vector<std::weak_ptr<Node>> Nodes;
    std::vector<std::weak_ptr<Link>> Links;
    ModulationScheme ModScheme;
    /**
     * @brief NumRegUsed is the number of regenerators used on the last node
     * of the transparent segment.
     */
    unsigned int NumRegUsed;

    Signal bypass(Signal S);
    unsigned int get_MaxContigSlots();
};

#endif // TRANSPARENTSEGMENT_H
