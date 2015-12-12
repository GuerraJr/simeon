#ifndef POWER_H
#define POWER_H

#include <GeneralClasses/Gain.h>

class Power {
  public:
    /**
    * @brief The InitType enum is used to choose how to init the object.
    */
    enum InitType {
        dBm, /*!< Use dBm to provide an initial value in dBm */
        Watt /*!< Use Watt to provide an initial value in watts */
    };
    /**
     * @brief Power is the standard constructor for a Power.
     * @param value is the value of the power, either in dBm or in Watts.
     */
    Power(long double value, InitType = dBm);
    Power(const Power &);

    Power operator*(Gain G);
    Power operator+(Power &P);
    Power &operator*=(Gain &G);
    Power &operator+=(Power &P);

    long double in_dBm() const;
    long double in_Watts() const;
  private:
    long double value_Watts;
};

#endif // POWER_H
