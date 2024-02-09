#include "../include/approximator.hpp"
#include "../include/midpoint_approximator.hpp"
#include "../include/simpson_approximator.hpp"
#include "../include/trapezoidal_approximator.hpp"

#include <pybind11/pybind11.h>
namespace py = pybind11;
using namespace sctoolbox;


// trampoline
class PyApproximator : public Approximator
{
public:
    using Approximator::Approximator;
protected:
    double approximate_step(const double first, const double second) const override {
        PYBIND11_OVERLOAD_PURE(double, Approximator, approximate_step);
    }
};

PYBIND11_MODULE(approximator, m)
{
    py::class_<Approximator, PyApproximator>(m, "Approximator")
        .def(py::init<const double, const double, const std::string, const unsigned int>())
        .def("approximate", &Approximator::approximate);
    py::class_<MidpointApproximator, Approximator>(m, "MidpointApproximator")
        .def(py::init<const double, const double, const std::string, const unsigned int>());
    py::class_<SimpsonApproximator, Approximator>(m, "SimpsonApproximator")
        .def(py::init<const double, const double, const std::string, const unsigned int>());
    py::class_<TrapezoidalApproximator, Approximator>(m, "TrapezoidalApproximator")
        .def(py::init<const double, const double, const std::string, const unsigned int>());
}