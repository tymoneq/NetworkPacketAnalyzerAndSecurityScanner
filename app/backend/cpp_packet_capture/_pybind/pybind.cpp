#include <pybind11/pybind11.h>
#include "../include/start.h"

using namespace std;
namespace py = pybind11;

PYBIND11_MODULE(packetCapturePy, m)
{

    m.doc() = "Packet capturer written in C++";

    m.def("start", &start, "Class that starts the capturer");
    m.def("stop", &stop, "Stop packet capture");
}