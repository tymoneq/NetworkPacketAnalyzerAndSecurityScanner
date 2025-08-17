#include <pybind11/pybind11.h>
#include "../include/manage.h"
#include "../include/networkStructures.h"
#include "../include/preprocessing.h"

using namespace std;
namespace py = pybind11;

// void bind_feature_packet(py::module_ &m)
// {
//       py::class_<FeaturePacket>(m, "FeaturePacket")
//           .def(py::init<>())
//           .def_readwrite("srcMac", &FeaturePacket::srcMac)
//           .def_readwrite("dstMac", &FeaturePacket::dstMac)
//           .def_readwrite("etherType", &FeaturePacket::etherType)
//           .def_readwrite("srcIp", &FeaturePacket::srcIp)
//           .def_readwrite("dstIp", &FeaturePacket::dstIp)
//           .def_readwrite("protocol", &FeaturePacket::protocol)
//           .def_readwrite("ttl", &FeaturePacket::ttl)
//           .def_readwrite("totalLength", &FeaturePacket::totalLength)
//           .def_readwrite("fragmentFlags", &FeaturePacket::fragmentFlags)
//           .def_readwrite("sourcePort", &FeaturePacket::sourcePort)
//           .def_readwrite("destinationPort", &FeaturePacket::destinationPort)
//           .def_readwrite("lengthOrWindow", &FeaturePacket::lengthOrWindow)
//           .def_readwrite("tcpFlags", &FeaturePacket::tcpFlags)
//           .def_readwrite("packetRate", &FeaturePacket::packetRate)
//           .def_readwrite("bytesPerFlow", &FeaturePacket::bytesPerFlow);
// }

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

//      bind_feature_packet(m);

//      py::class_<Preprocessing>(m, "Preprocessing")
//         .def(py::init<>())
//         .def("preprocess", &Preprocessing::preprocess)
//         .def("getPacket", &Preprocessing::getPacket);  // return struct
}