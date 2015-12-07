#ifndef SIGNAL_H
#define SIGNAL_H

#include <GeneralClasses/Power.h>

/**
 * @brief The Signal class represents a signal that propagates through the
 * network.
 */
class Signal {
  public:

	static Gain InputOSNR;
	static Power InputPower;

	Signal();

	void operator*=(Gain);
	void operator+=(Power);

	long double get_OSNR();

  private:
	Power SignalPower;
	Power NoisePower;
};

#endif // SIGNAL_H
