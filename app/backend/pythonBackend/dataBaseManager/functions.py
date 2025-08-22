from .models import FeaturePacketModel
from ..logger.logger import *


def savePacket(pkt, session):
    '''This function saves a packet and associates it with a session.
    
    Parameters
    ----------
    pkt
        The `pkt` parameter likely refers to a packet of data that needs to be saved. This could be a
    network packet, a data packet, or any other unit of data being transmitted or processed.
    session
        The `savePacket` function seems to be missing some information about the `session` parameter. Could
    you please provide more details or context about what the `session` parameter represents or how it
    is used within the function? This will help me provide a more accurate explanation or code snippet.
    
    '''
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


