#include <pybind11/pybind11.h>
#include "../include/manage.h"
#include "../include/networkStructures.h"
#include "../include/logger.h"

using namespace std;
namespace py = pybind11;

PYBIND11_MODULE(packetCapturePy, m)
{
      m.doc() = "Packet capturer written in C++";

      py::class_<FeaturePacket>(m, "FeaturePacket")
          .def(py::init<>()) // default constructor

          // Layer 2
          .def_readwrite("srcMac", &FeaturePacket::srcMac)
          .def_readwrite("dstMac", &FeaturePacket::dstMac)
          .def_readwrite("etherType", &FeaturePacket::etherType)

          // Layer 3
          .def_readwrite("srcIp", &FeaturePacket::srcIp)
          .def_readwrite("dstIp", &FeaturePacket::dstIp)
          .def_readwrite("protocol", &FeaturePacket::protocol)
          .def_readwrite("ttl", &FeaturePacket::ttl)
          .def_readwrite("totalLength", &FeaturePacket::totalLength)
          .def_readwrite("fragmentFlags", &FeaturePacket::fragmentFlags)

          // Layer 4
          .def_readwrite("sourcePort", &FeaturePacket::sourcePort)
          .def_readwrite("destinationPort", &FeaturePacket::destinationPort)
          .def_readwrite("lengthOrWindow", &FeaturePacket::lengthOrWindow)
          .def_readwrite("tcpFlags", &FeaturePacket::tcpFlags)

          // Derived features
          .def_readwrite("packetRate", &FeaturePacket::packetRate)
          .def_readwrite("bytesPerFlow", &FeaturePacket::bytesPerFlow);


      py::enum_<MessagesTypes>(m,"MessagesTypes")
            .value("info", info)
            .value("critical", critical)
            .value("warn", warn)
            .value("error", error)
            .value("trace", trace)
            .value("debug", debug)
            .export_values(); // optional: allows access via MessagesTypes.info

      // Releases the GIL for the entire duration of start() and stop()
      m.def("initialize", &initialize,
            py::call_guard<py::gil_scoped_release>(),
            "Initialize the packet capturer");

      m.def("getPacket", &getPacket, py::call_guard<py::gil_scoped_release>(), "Get internet packet");

      m.def("stop", &stop,
            py::call_guard<py::gil_scoped_release>(),
            "Stop the packet capturer");

      m.def("writeToLog", &writeToLog,  py::call_guard<py::gil_scoped_release>(), "Write to log");
}