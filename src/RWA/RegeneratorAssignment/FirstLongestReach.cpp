#include <RWA/RegeneratorAssignment/FirstLongestReach.h>
#include <Structure/Node.h>
#include <Structure/Link.h>

FirstLongestReach::FirstLongestReach(std::shared_ptr<Topology> T,
                                     std::vector<std::shared_ptr<ModulationScheme>> ModulationSchemes) :
    RegeneratorAssignment(T, ModulationSchemes) {

}

std::vector<TransparentSegment> FirstLongestReach::assignRegenerators(Call C,
        std::vector<std::weak_ptr<Link> > Links) {

    unsigned int NeededRegenerators = get_NumNeededRegenerators(C);
    std::vector<TransparentSegment> TransparentSegments;
    std::vector<std::weak_ptr<Node>> Nodes;

    for (auto link : Links) {
        Nodes.push_back(link.lock()->Origin);
    }

    Nodes.push_back(Links.back().lock()->Destination);

    auto r = Nodes.begin();

    for (auto s = Nodes.begin(); s != Nodes.end(); ++s) {
        for (auto x = s + 1; s != Nodes.end(); ++x) {
            if (((*x).lock()->get_NumAvailableRegenerators() >= NeededRegenerators) ||
                    ((*x).lock() == C.Destination.lock())) {
                if (isThereSpectrumAndOSNR(C, Links, *s, *x)) {
                    if ((*x).lock() == C.Destination.lock()) {
                        TransparentSegments.push_back(createTransparentSegment(C, Links,
                                                      *r, *x, NeededRegenerators));
                        return TransparentSegments;
                    } else {
                        r = x;
                    }
                } else {
                    if (r != s) {
                        TransparentSegments.push_back(createTransparentSegment(C, Links,
                                                      *s, *r, NeededRegenerators));
                        s = r;
                        x = r;
                    } else {
                        TransparentSegments.clear();
                        return TransparentSegments;
                    }
                }
            }
        }
    }

    TransparentSegments.clear();
    return TransparentSegments;
}
