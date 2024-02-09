#ifndef TRAPEZOIDAL_APPROXIMATOR_HPP_
#define TRAPEZOIDAL_APPROXIMATOR_HPP_

#include "approximator.hpp"

namespace sctoolbox
{
    class TrapezoidalApproximator : public Approximator
    {
    public:
        TrapezoidalApproximator(const double a, const double b, const std::string f, const unsigned int n) : Approximator(a, b, f, n) {}

    private:
        double approximate_step(const double first, const double second) const;
    };
}
#endif