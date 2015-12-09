#ifndef WAVELENGTHASSIGNMENTALGORITHM_H
#define WAVELENGTHASSIGNMENTALGORITHM_H

#include <map>
#include <Calls/Call.h>
#include <Structure/Slot.h>
#include <Structure/Topology.h>
#include <RWA/TransparentSegment.h>

class WavelengthAssignmentAlgorithm {
  public:
    WavelengthAssignmentAlgorithm(std::shared_ptr<Topology> T, std::vector<ModulationScheme> Schemes);

    virtual std::map<std::weak_ptr<Link>, std::vector<std::weak_ptr<Slot>>>
    assignSlots(Call C, TransparentSegment Seg) = 0;

    std::shared_ptr<Topology> T;
    std::vector<ModulationScheme> Schemes;
};

#endif // WAVELENGTHASSIGNMENTALGORITHM_H
