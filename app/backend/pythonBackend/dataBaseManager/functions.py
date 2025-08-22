from .models import FeaturePacketModel
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


