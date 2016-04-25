#ifndef LINKSPECTRALDENSITY_H
#define LINKSPECTRALDENSITY_H

#include <GeneralClasses/SpectralDensity.h>
#include <GeneralClasses/Power.h>
#include <Structure/Slot.h>
#include <vector>
#include <memory>
#include <map>

class LinkSpectralDensity
{
public:
    LinkSpectralDensity(std::vector<std::shared_ptr<Slot>>);

    std::vector<std::shared_ptr<Slot>> LinkSlots;
    LinkSpectralDensity &operator *=(TF::TransferFunction &); // TO DO

    Power get_SpectralPower(); // TO DO
    Power get_TransmitedPower(double);


    std::vector<std::shared_ptr<SpectralDensity>> S;    

    static unsigned long numFrequencySamples;
};

#endif // LINKSPECTRALDENSITY_H
