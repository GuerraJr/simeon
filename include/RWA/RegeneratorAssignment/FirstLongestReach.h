#ifndef FIRSTLONGESTREACH_H
#define FIRSTLONGESTREACH_H

#include <RWA/RegeneratorAssignment/RegeneratorAssignment.h>
#include <GeneralClasses/ModulationScheme.h>

class FirstLongestReach : public RegeneratorAssignment {
  public:
    FirstLongestReach(std::shared_ptr<Topology> T,
                      std::vector<ModulationScheme> ModulationSchemes);

    std::vector<TransparentSegment> assignRegenerators(
        std::shared_ptr<Call> C,
        std::vector<std::weak_ptr<Link> > Links);
};

#endif // FIRSTLONGESTREACH_H
