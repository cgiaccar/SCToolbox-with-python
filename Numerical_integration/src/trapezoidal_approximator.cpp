#include "../include/trapezoidal_approximator.hpp"
#include "muparserx/mpParser.h"

using namespace mup;
using namespace sctoolbox;

double TrapezoidalApproximator::approximate_step(const double first, const double second) const
{
    ParserX p;
    Value x_1(first);  // x_1 = a
    Value x_2(second); // x_2 = b

    p.DefineVar("x", Variable(&x_1)); // x = x_1
    p.SetExpr(f);
    try
    {
        Value result_1 = p.Eval(); // Evaluate f(x_1)

        x_1 = x_2;                 // x = x_2
        Value result_2 = p.Eval(); // Evaluate f(x_2)

        return (second - first) * (((double)result_1 + (double)result_2) / 2); // Trapezoidal rule
    }
    catch (ParserError &e) // Avoid having to catch a ParserX-specific exception in main
    {
        throw std::invalid_argument("Invalid expression.");
    }
}