#include <GeneralClasses/dB.h>
#include <cmath>

dB::dB(long double value, InitType Type) {
    if (Type == InitType::init_dB) {
        value_dB = value;
        value_Linear = std::pow(10, 0.1 * value);
    } else if (Type == InitType::init_Linear) {
        value_Linear = value;
        value_dB = 10 * log10(value);
    }
}

dB::dB(const dB &Value) {
    value_dB = Value.in_dB();
    value_Linear = Value.in_Linear();
}

long double dB::in_dB() const {
    return value_dB;
}

long double dB::in_Linear() const {
    return value_Linear;
}
