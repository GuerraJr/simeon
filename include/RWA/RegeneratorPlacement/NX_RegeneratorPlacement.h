#ifndef NX_REGENERATORPLACEMENT_H
#define NX_REGENERATORPLACEMENT_H

#include <RWA/RegeneratorPlacement/RegeneratorPlacement.h>

class NX_RegeneratorPlacement : public RegeneratorPlacement {
  public:
    NX_RegeneratorPlacement(std::shared_ptr<Topology> T);

    /**
     * @brief placeRegenerators inserts the regenerators in the network.
     * @param N is the number of translucent nodes.
     * @param X is the number of regenerators per translucent node.
     */
    virtual void placeRegenerators(int N, int X) = 0;
};

#endif // NX_REGENERATORPLACEMENT_H
