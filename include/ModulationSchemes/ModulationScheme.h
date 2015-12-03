#ifndef MODULATIONSCHEME_H
#define MODULATIONSCHEME_H

#include <GeneralClasses/TransmissionBitrate.h>
#include <GeneralClasses/Gain.h>

/**
 * @brief The ModulationScheme class is a model of a modulation scheme.
 */
class ModulationScheme {
  public:
    virtual Gain get_SNR_Per_Bit() = 0;
    virtual unsigned int get_M() = 0;
    virtual Gain get_Threshold_SNR(TransmissionBitrate Bitrate) = 0;
};

#endif // MODULATIONSCHEME_H
