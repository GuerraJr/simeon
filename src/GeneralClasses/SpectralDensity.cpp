#include "GeneralClasses/SpectralDensity.h"
#include <GeneralClasses/PhysicalConstants.h>
#include <GeneralClasses/Signal.h>
#include <Structure/Slot.h>

SpectralDensity::SpectralDensity
(double freqMin, double freqMax, unsigned int numSamples) : densityScaling(1),
    freqMin(freqMin), freqMax(freqMax)
{
    std::pair<double, double> freqValues = std::make_pair(freqMin, freqMax);

    if(specDensityMap.count(freqValues) == 0)
        {
        arma::rowvec thisSpecDensity = arma::linspace(freqMin, freqMax, numSamples).t();

        thisSpecDensity.transform(
            [] (double val)
            {
            return (std::exp2l(-pow(2 * (val - PhysicalConstants::freq) / SBW_3dB,
                                    2 * GaussianOrder)));
            }
        );

        specDensityMap.emplace(freqValues, thisSpecDensity);
        }
}

SpectralDensity& SpectralDensity::operator *=(TransferFunction &H)
{
    densityScaling *= H.scale;
    if (!H.isImpulseTransferFunction)
        {
        std::pair<double, double> freqValues = std::make_pair(freqMin, freqMax);

        specDensityMap[freqValues] %= H.frequencySamples;
        }

    return *this;
}
