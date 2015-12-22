#include <Devices/Amplifiers/EDFA.h>
#include <GeneralClasses/PhysicalConstants.h>

Gain EDFA::NoiseFigure = Gain(5);

EDFA::EDFA(Gain G) : Amplifier(G) {
    calculate_NoisePower();
}

void EDFA::calculate_NoisePower() {
    //This is the ASE Noise Modelling.
    double Noise = 0.5 * PhysicalConstants::numPolarizations *
                        PhysicalConstants::h * PhysicalConstants::freq * PhysicalConstants::BRef *
                        NoiseFigure.in_Linear() * (AmplifierGain.in_Linear() - 1);
    NoisePower = Power(Noise, Power::Watt);
}
