from .models import FeaturePacketModel, DerivedFeaturesModel
from collections import defaultdict
from ..logger.logger import *


def savePacket(pkt, session):
    writeToLogPy(info, "Saving packet to database")
    packetRow = FeaturePacketModel(
        srcMac=pkt.srcMac,
        dstMac=pkt.dstMac,
        etherType=pkt.etherType,
        srcIp=pkt.srcIp,
        dstIp=pkt.dstIp,
        protocol=pkt.protocol,
        ttl=pkt.ttl,
        totalLength=pkt.totalLength,
        fragmentFlags=pkt.fragmentFlags,
        sourcePort=pkt.sourcePort,
        destinationPort=pkt.destinationPort,
        lengthOrWindow=pkt.lengthOrWindow,
        tcpFlags=pkt.tcpFlags,
    )
    session.add(packetRow)
    session.commit()
    return packetRow.id


def getFeatures(session):

    writeToLogPy(info, "Extracting packet features")

    packetCountPerIp = defaultdict(int)
    unusualPorts = defaultdict(list)
    ipFrequency = defaultdict(int)

    common_ports = set([20, 21, 22, 23, 25, 53, 80, 443, 110, 143])

    packets = session.query(FeaturePacketModel).all()

    for pack in packets:
        packetCountPerIp[pack.srcIp] += 1
        ipFrequency[pack.srcIp] += 1
        ipFrequency[pack.dstIp] += 1

        if pack.sourcePort not in common_ports:
            unusualPorts[pack.srcIp].append(pack.sourcePort)

        if pack.destinationPort not in common_ports:
            unusualPorts[pack.srcIp].append(pack.destinationPort)

    for ip in packetCountPerIp:
        derivedPacketRow = DerivedFeaturesModel(
            srcIp=ip,
            packetCount=packetCountPerIp[ip],
            ipFrequency=ipFrequency[ip],
            unusualPorts=",".join(map(str, list(set(unusualPorts[ip])))),
        )
        session.add(derivedPacketRow)

    writeToLogPy(info, "Saving features into database")
    session.commit()
