#ifndef SIMPSON_APPROXIMATOR_HPP_
#define SIMPSON_APPROXIMATOR_HPP_

#include "approximator.hpp"

namespace sctoolbox
{
    class SimpsonApproximator : public Approximator
    {
    public:
        SimpsonApproximator(const double a, const double b, const std::string f, const unsigned int n) : Approximator(a, b, f, n) {}

    private:
        double approximate_step(const double first, const double second) const;
    };
}
#endif