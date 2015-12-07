#ifndef MODULATIONSCHEME_H
#define MODULATIONSCHEME_H

#include <GeneralClasses/Gain.h>

/**
 * @brief The ModulationScheme class represents a modulation scheme.
 */
class ModulationScheme {
  public:
	ModulationScheme(unsigned int M, Gain SNR_Per_Bit);
	unsigned int get_M();
	Gain get_SNR_Per_Bit();

  private:
	unsigned int M;
	Gain SNR_Per_Bit;
};

#endif // MODULATIONSCHEME_H
