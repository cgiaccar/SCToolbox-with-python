#ifndef APPROXIMATOR_HPP_
#define APPROXIMATOR_HPP_

#include <string>
namespace sctoolbox
{
    class Approximator
    {
    public:
        virtual ~Approximator() {}

        Approximator(const double a, const double b, const std::string f, const unsigned int n);

        double approximate() const;

    protected:
        double a;
        double b;
        std::string f;
        unsigned int n;

        virtual double approximate_step(const double first, const double second) const = 0;
    };
}

#endif