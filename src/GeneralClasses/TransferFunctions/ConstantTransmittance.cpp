#include "include/GeneralClasses/TransferFunctions/ConstantTransmittance.h"

using namespace TF;

ConstantTransmittance::ConstantTransmittance(Gain scale) : Transmittance(scale)
{

}

Gain ConstantTransmittance::get_TransmittanceAt(double)
{
    return scale;
}
