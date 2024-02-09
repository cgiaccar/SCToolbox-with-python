#include "../include/simpson_approximator.hpp"
#include "muparserx/mpParser.h"

using namespace mup;
using namespace sctoolbox;

double SimpsonApproximator::approximate_step(const double first, const double second) const
{
    ParserX p;
    Value x_1(first);                // x_1 = a
    Value x_2((first + second) / 2); // x_2 = (a+b)/2
    Value x_3(second);               // x_3 = b

    p.DefineVar("x", Variable(&x_1)); // x = x_1
    p.SetExpr(f);
    try
    {
        Value result_1 = p.Eval(); // Evaluate f(x_1)

        x_1 = x_2;                 // x = x_2
        Value result_2 = p.Eval(); // Evaluate f(x_2)

        x_1 = x_3;                 // x = x_3
        Value result_3 = p.Eval(); // Evaluate f(x_3)

        return (second - first) * (((double)result_1 + 4 * (double)result_2 + (double)result_3) / 6); // Simpson's rule
    }
    catch (ParserError &e) // Avoid having to catch a ParserX-specific exception in main
    {
        throw std::invalid_argument("Invalid expression.");
    }
}