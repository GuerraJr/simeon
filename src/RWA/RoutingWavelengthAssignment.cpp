#include <RWA/RoutingWavelengthAssignment.h>
#include <algorithm>
#include <GeneralClasses/ModulationScheme.h>
#include <RWA/Route.h>
#include <RWA/Routing/RoutingAlgorithm.h>
#include <RWA/WavelengthAssignment/WavelengthAssignmentAlgorithm.h>
#include <RWA/RegeneratorPlacement/RegeneratorPlacement.h>
#include <RWA/RegeneratorAssignment/RegeneratorAssignment.h>
#include <Structure/Topology.h>

RoutingWavelengthAssignment::RoutingWavelengthAssignment(
    std::shared_ptr<RoutingAlgorithm> R_Alg,
    std::shared_ptr<WavelengthAssignmentAlgorithm> WA_Alg,
    std::shared_ptr<RegeneratorAssignment> RA_Alg,
    std::vector<ModulationScheme> Schemes,
    std::shared_ptr<Topology> T) :
    R_Alg(R_Alg), WA_Alg(WA_Alg), RA_Alg(RA_Alg), Schemes(Schemes), T(T) {

}

RoutingWavelengthAssignment::RoutingWavelengthAssignment(
    std::shared_ptr<RoutingAlgorithm>  R_Alg,
    std::shared_ptr<WavelengthAssignmentAlgorithm> WA_Alg,
    std::vector<ModulationScheme> Schemes,
    std::shared_ptr<Topology> T) :
    R_Alg(R_Alg), WA_Alg(WA_Alg), Schemes(Schemes), T(T) {

    RA_Alg = nullptr;

}

std::shared_ptr<Route> RoutingWavelengthAssignment::routeCall(
    std::shared_ptr<Call> C) {

    std::vector<std::weak_ptr<Link>> Links;
    std::vector<TransparentSegment> Segments;
    std::map<std::weak_ptr<Link>, std::vector<std::weak_ptr<Slot>>,
        std::owner_less<std::weak_ptr<Link>>> Slots;

    Links = R_Alg->route(C);

    if (Links.empty()) {
        C->Status = Call::Blocked;
    }

    if (RA_Alg == nullptr) {
        std::sort(Schemes.rbegin(), Schemes.rend());

        for (auto scheme : Schemes) {
            TransparentSegment Segment(Links, scheme, 0);
            Signal S;

            if (Segment.bypass(S).get_OSNR() > scheme.get_ThresholdOSNR(C->Bitrate)) {
                Segments.push_back(Segment);
                auto SegmentSlots = WA_Alg->assignSlots(C, Segment);

                if (SegmentSlots.empty()) {
                    if (scheme == Schemes.back()) {
                        C->Status = Call::Blocked;
                    }

                    continue;
                }

                Slots.insert(SegmentSlots.begin(), SegmentSlots.end());
                break;
            }
        }
    } else {
        Segments = RA_Alg->assignRegenerators(C, Links);

        if (Segments.empty()) {
            C->Status = Call::Blocked;
        }

        for (auto segment : Segments) {
            auto SegmentSlots = WA_Alg->assignSlots(C, segment);

            if (SegmentSlots.empty()) {
                C->Status = Call::Blocked;
                Slots.clear();
                break;
            }

            Slots.insert(SegmentSlots.begin(), SegmentSlots.end());
        }
    }

    if (C->Status == Call::Not_Evaluated) {
        C->Status = Call::Implemented;
    }

    return std::shared_ptr<Route>(new Route(Segments, Slots));
}
