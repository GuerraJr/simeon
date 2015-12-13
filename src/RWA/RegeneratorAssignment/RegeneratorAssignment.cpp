#include <RWA/RegeneratorAssignment/RegeneratorAssignment.h>
#include <cmath>
#include <algorithm>
#include <boost/assert.hpp>
#include <Structure/Link.h>

RegeneratorAssignment::RegeneratorAssignment(std::shared_ptr<Topology> T,
        std::vector<ModulationScheme> Schemes) :
    T(T), ModulationSchemes(Schemes) {

}

unsigned int RegeneratorAssignment::get_NumNeededRegenerators(
    std::shared_ptr<Call> C) {
    return ceil(C->Bitrate.get_Bitrate() / RegeneratorBitrate);
}

bool RegeneratorAssignment::isThereSpectrumAndOSNR(std::shared_ptr<Call> C,
        std::vector<std::weak_ptr<Link> > Links,
        std::weak_ptr<Node> start,
        std::weak_ptr<Node> end) {

    std::vector<std::weak_ptr<Link>> SegmentLinks = segmentLinks(Links, start, end);

    Signal Sig;
    TransparentSegment Segment(SegmentLinks, ModulationSchemes.front(), 0);
    Sig = Segment.bypass(Sig);

    bool isThereScheme = false;

    for (auto scheme : ModulationSchemes) {
        isThereScheme |= isThereSpectrumAndOSNR(C, Links, start, end, scheme);

        if (isThereScheme) {
            return true;
        }
    }

    return false;
}

bool RegeneratorAssignment::isThereSpectrumAndOSNR(std::shared_ptr<Call> C,
        std::vector<std::weak_ptr<Link> > Links,
        std::weak_ptr<Node> start,
        std::weak_ptr<Node> end,
        ModulationScheme scheme) {

    std::vector<std::weak_ptr<Link>> SegmentLinks = segmentLinks(Links, start, end);
    TransparentSegment Segment(SegmentLinks, scheme, 0);
    Signal Sig;
    Sig = Segment.bypass(Sig);

    return ((Sig.get_OSNR() > scheme.get_ThresholdOSNR(C->Bitrate)) &&
            (Segment.get_MaxContigSlots() > scheme.get_NumSlots(C->Bitrate)));
}

ModulationScheme RegeneratorAssignment::getMostEfficientScheme(
    std::shared_ptr<Call> C,
    std::vector<std::weak_ptr<Link>> SegmentLinks) {

    TransparentSegment Segment(SegmentLinks, ModulationSchemes.front(), 0);
    Signal S;
    S = Segment.bypass(S);

    std::sort(ModulationSchemes.rbegin(), ModulationSchemes.rend());

    for (auto scheme : ModulationSchemes) {
        //There's OSNR
        if (S.get_OSNR() > scheme.get_ThresholdOSNR(C->Bitrate)) {
            //There's spectrum
            if (Segment.get_MaxContigSlots() > scheme.get_NumSlots(C->Bitrate)) {
                return scheme;
            }
        }
    }

    BOOST_ASSERT_MSG(false, "No Scheme can implement Call in Transparent Segment.");
    return ModulationSchemes.back();
}

TransparentSegment RegeneratorAssignment::createTransparentSegment(
    std::shared_ptr<Call> C,
    std::vector<std::weak_ptr<Link> > Links,
    std::weak_ptr<Node> start,
    std::weak_ptr<Node> end,
    unsigned int NumRegUsed) {

    std::vector<std::weak_ptr<Link>> SegmentLinks = segmentLinks(Links, start, end);
    ModulationScheme Scheme = getMostEfficientScheme(C, SegmentLinks);

    return TransparentSegment(SegmentLinks, Scheme, NumRegUsed);
}

std::vector<std::weak_ptr<Link>> RegeneratorAssignment::segmentLinks(
                                  std::vector<std::weak_ptr<Link> > Links,
                                  std::weak_ptr<Node> start,
std::weak_ptr<Node> end) {

    std::vector<std::weak_ptr<Link>> SegmentLinks;
    bool foundNode = false;

    for (auto link : Links) {
        if (link.lock()->Origin.lock() == start.lock()) {
            SegmentLinks.push_back(link);
            foundNode = true;
            continue;
        } else if (foundNode && (link.lock()->Origin.lock() == end.lock())) {
            break;
        } else if (foundNode) {
            SegmentLinks.push_back(link);
            continue;
        }
    }

    return SegmentLinks;
}
