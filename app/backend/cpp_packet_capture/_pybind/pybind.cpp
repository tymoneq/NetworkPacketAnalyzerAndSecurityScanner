#include <pybind11/pybind11.h>
#include "../include/manage.h"

using namespace std;
namespace py = pybind11;

PYBIND11_MODULE(packetCapturePy, m)
{
    m.doc() = "Packet capturer written in C++";

    // Releases the GIL for the entire duration of start() and stop()
    m.def("start", &start,
          py::call_guard<py::gil_scoped_release>(),
          "Start the packet capturer");

    m.def("stop", &stop,
          py::call_guard<py::gil_scoped_release>(),
          "Stop the packet capturer");
}