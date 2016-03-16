#include "GeneralClasses/SpectralDensity.h"
#include <GeneralClasses/PhysicalConstants.h>
#include <GeneralClasses/Signal.h>
#include <Structure/Slot.h>

std::map<std::pair<double, double>, arma::rowvec>
SpectralDensity::specDensityMap;

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
            return (pow(std::exp(-pow(2 * std::log(2) * (val - PhysicalConstants::freq) / SBW_3dB,
                                    2 * GaussianOrder)), 2)); // Elevei ao quadrado
            }
        );

        specDensityMap.emplace(freqValues, thisSpecDensity);
        }
    specDensity = specDensityMap[freqValues];
}

SpectralDensity& SpectralDensity::operator *=(TransferFunction &H)
{
    densityScaling *= H.scale;
    if (!H.isImpulseTransferFunction)
        {
        specDensity %= H.frequencySamples;
        }

    return *this;
}
