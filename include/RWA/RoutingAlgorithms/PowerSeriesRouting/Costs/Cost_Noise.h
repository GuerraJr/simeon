#ifndef COST_NOISE_H
#define COST_NOISE_H

#include "Cost.h"
#include "GeneralClasses/TransmissionBitrate.h"
#include "GeneralClasses/ModulationScheme.h"
#include <map>
#include <tuple>

namespace PSR
{

class cNoise : public Cost
{
public:
    cNoise(int NMin, int NMax, std::shared_ptr<Topology> T);
    arma::rowvec getCost(std::weak_ptr<Link> link, std::shared_ptr<Call> C);

private:
    struct CallProperties
    {
        std::weak_ptr<Link> link;
        TransmissionBitrate bitrate;
        ModulationScheme scheme;

        bool operator <(const CallProperties &other) const
        {
            return ((link.lock() < other.link.lock()) ||
                    ((link.lock() == other.link.lock()) && (bitrate < other.bitrate)) ||
                    ((link.lock() == other.link.lock()) && (bitrate == other.bitrate) &&
                     (scheme < other.scheme)));
        }
    };

    void createCache();
    std::map <CallProperties, arma::rowvec> cache;
};

}

#endif // COST_NOISE_H
