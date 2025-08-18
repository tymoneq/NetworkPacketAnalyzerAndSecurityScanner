from .models import FeaturePacketModel


def savePacket(pkt,session):
    packetRow = FeaturePacketModel(
        srcMac = pkt.srcMac,
        dstMac = pkt.dstMac,
        etherType = pkt.etherType,
        
        srcIp = pkt.srcIp,
        dstIp = pkt.dstIp,
        protocol = pkt.protocol,
        ttl = pkt.ttl,
        totalLength = pkt.totalLength,
        fragmentFlags = pkt.fragmentFlags,
        
        sourcePort = pkt.sourcePort,
        destinationPort = pkt.destinationPort,
        lengthOrWindow = pkt.lengthOrWindow,
        tcpFlags = pkt.tcpFlags
    )
    session.add(packetRow)
    session.commit()
    return packetRow.id


def readData(session):
    packets = session.query(FeaturePacketModel).all()
    
    for p in packets:
        print(p)