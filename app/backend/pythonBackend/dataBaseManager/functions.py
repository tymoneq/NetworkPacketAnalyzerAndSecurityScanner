from .models import FeaturePacketModel
from .database import engine
from collections import defaultdict
from ..logger.logger import *
import datetime
import pandas as pd


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


def getFeatures(session, timeWindow=1):

    writeToLogPy(info, "Extracting packet features")

    packetCountPerIp = defaultdict(int)
    unusualPorts = defaultdict(list)
    ipFrequency = defaultdict(int)
    synPacketsPerIp = defaultdict(int)
    synActPacketsPerIp = defaultdict(int)
    actPacketsPerIp = defaultdict(int)

    common_ports = set([20, 21, 22, 23, 25, 53, 80, 443, 110, 143])

    # Query for packets
    timeDelta = datetime.datetime.now(datetime.timezone.utc) - datetime.timedelta(
        seconds=timeWindow
    )

    packets = (
        session.query(FeaturePacketModel)
        .filter(FeaturePacketModel.createdAt > timeDelta)
        .all()
    )

    # Extracting Features
    for pack in packets:

        # checking if syn flag
        if pack.tcpFlags == 2:
            synPacketsPerIp[pack.srcIp] += 1

        # checking syn ack flag
        if pack.tcpFlags == 18:
            synActPacketsPerIp[pack.srcIp] += 1

        # checking ack flag
        if pack.tcpFlags == 16:
            actPacketsPerIp[pack.srcIp] += 1

        packetCountPerIp[pack.srcIp] += 1
        ipFrequency[pack.srcIp] += 1
        ipFrequency[pack.dstIp] += 1

        if pack.sourcePort not in common_ports:
            unusualPorts[pack.srcIp].append(pack.sourcePort)

        if pack.destinationPort not in common_ports:
            unusualPorts[pack.srcIp].append(pack.destinationPort)

    # Saving features into dataFrame

    df = pd.DataFrame(
        columns=[
            "srcIp",
            "packetCount",
            "unusualPorts",
            "ipFrequency",
            "synFrequency",
            "synAckFrequency",
            "ackFrequency",
        ]
    )

    i = 0
    for ip in packetCountPerIp:
        df.loc[i] = [
            ip,
            packetCountPerIp[ip],
            list(set(unusualPorts[ip])),
            ipFrequency[ip],
            synPacketsPerIp[ip],
            synActPacketsPerIp[ip],
            actPacketsPerIp[ip],
        ]
        i += 1

    return df


def convertFeaturePacketToPandas(session):

    query = session.query(FeaturePacketModel)
    return pd.read_sql(query.statement, engine)
