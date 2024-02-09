#include "../include/statistics.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include <iostream>

namespace py = pybind11;
using namespace sctoolbox;


PYBIND11_MODULE(statistics, m)
{
    py::class_<Statistics>(m, "Statistics")
        .def(py::init<const std::string, const std::string>())
        .def(py::init<const data_type, const std::string>())
        .def("__repr__", [](const Statistics &stat) { // __repr__ magic method is binded with a lambda that makes use of the print c++ method redirected to a string
            std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
            std::ostringstream string_output;
            stat.print();
            std::cout.rdbuf(oldCoutStreamBuf);
            return string_output.str();
        })
        .def("print", py::overload_cast<>(&Statistics::print, py::const_))
        .def("print", py::overload_cast<const std::string>(&Statistics::print, py::const_))
        .def("__iter__", [](const Statistics &stat) {
            return py::make_iterator(stat.begin(), stat.end());
            }, py::keep_alive<0, 1>()
        )
        .def("mean", &Statistics::mean)
        .def("write_mean", &Statistics::write_mean)
        .def("median", &Statistics::median)
        .def("write_median", &Statistics::write_median)
        .def("variance", &Statistics::variance)
        .def("write_variance", &Statistics::write_variance)
        .def("sdeviation", &Statistics::sdeviation)
        .def("write_sdeviation", &Statistics::write_sdeviation)
        .def("count", &Statistics::count)
        .def("write_count", &Statistics::write_count)
        .def("frequency", &Statistics::frequency)
        .def("write_frequency", &Statistics::write_frequency)
        .def("correlation", &Statistics::correlation)
        .def("write_correlation", &Statistics::write_correlation)
        .def("classification", &Statistics::classification);
}