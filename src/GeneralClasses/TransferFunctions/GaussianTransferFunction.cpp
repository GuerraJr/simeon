#include "GeneralClasses/TransferFunctions/GaussianTransferFunction.h"
#include <GeneralClasses/PhysicalConstants.h>

GaussianTransferFunction::GaussianTransferFunction
(double freqMin, double freqMax, unsigned long int numSamples,
 unsigned int filterOrder, double scale) :
    TransferFunction(freqMin, freqMax, numSamples, scale)
{
    for (auto& val : frequencySamples)
        {
        val = std::exp2l( (-2) * pow(2 * (val - PhysicalConstants::freq) / BW_3dB,
                                     2 * filterOrder));
        }
}
