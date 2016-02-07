#ifndef CONTIGUITYFIT_H
#define CONTIGUITYFIT_H

#include "WavelengthAssignmentAlgorithm.h"

class BestFit : public WavelengthAssignmentAlgorithm
{
public:
    BestFit(std::shared_ptr<Topology> T);

    std::map<std::weak_ptr<Link>,
        std::vector<std::weak_ptr<Slot>>,
        std::owner_less<std::weak_ptr<Link>>>
        assignSlots(std::shared_ptr<Call> C, TransparentSegment Seg);
    void load() {}
    void save(std::string);
};

#endif // CONTIGUITYFIT_H
