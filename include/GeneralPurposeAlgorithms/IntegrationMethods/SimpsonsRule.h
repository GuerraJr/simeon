#ifndef SIMPSONSRULE_H
#define SIMPSONSRULE_H

#include <include/GeneralPurposeAlgorithms/IntegrationMethods/IntegrationMethod.h>

namespace NumericMethods
{
/**
 * @brief The SimpsonsRule class represents the Simpson's Rule numerical integration method.
 */
class SimpsonsRule : public IntegrationMethod
{
public:
    /**
     * @brief SimpsonsRule is a constructor for a SimpsonsRule object.
     */
    SimpsonsRule(arma::mat &, double);
    /**
     * @brief calculate is used to compute the SimpsonsRule integration method.
     * @return the result of the calculations.
     */
    double calculate();
};
}

#endif // SIMPSONSRULE_H
