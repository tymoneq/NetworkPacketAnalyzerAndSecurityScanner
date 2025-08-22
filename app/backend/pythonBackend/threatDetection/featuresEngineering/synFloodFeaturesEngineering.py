from ...logger.logger import *
import datetime
import pandas as pd
from collections import defaultdict
from ...dataBaseManager.models import FeaturePacketModel


class AnalyzePackets:

    def __init__(self, session, timeWindow):
        self.session = session

        self.packetCountPerIp = defaultdict(int)
        self.unusualPorts = defaultdict(list)
        self.ipFrequency = defaultdict(int)
        self.synPacketsPerIp = defaultdict(int)
        self.synActPacketsPerIp = defaultdict(int)
        self.actPacketsPerIp = defaultdict(int)
        self.timeWindow = timeWindow

        self.common_ports = set([20, 21, 22, 23, 25, 53, 80, 443, 110, 143])

    def getFeatures(self):

        writeToLogPy(info, "Extracting packet features")

        # Query for packets
        timeDelta = datetime.datetime.now(datetime.timezone.utc) - datetime.timedelta(
            minutes=self.timeWindow
        )
        self.analyzePackets(timeDelta)

        # Saving features into dataFrame
        return self.convertFeaturesToDf()

    def analyzePackets(self, timeDelta):
        packets = (
            self.session.query(FeaturePacketModel)
            .filter(FeaturePacketModel.createdAt > timeDelta)
            .all()
        )

        # Extracting Features
        for pack in packets:

            # checking if syn flag
            if pack.tcpFlags == 2:
                self.synPacketsPerIp[pack.srcIp] += 1

            # checking syn ack flag
            if pack.tcpFlags == 18:
                self.synActPacketsPerIp[pack.srcIp] += 1

            # checking ack flag
            if pack.tcpFlags == 16:
                self.actPacketsPerIp[pack.srcIp] += 1

            self.packetCountPerIp[pack.srcIp] += 1
            self.ipFrequency[pack.srcIp] += 1
            self.ipFrequency[pack.dstIp] += 1

            if pack.sourcePort not in self.common_ports:
                self.unusualPorts[pack.srcIp].append(pack.sourcePort)

            if pack.destinationPort not in self.common_ports:
                self.unusualPorts[pack.srcIp].append(pack.destinationPort)

    def convertFeaturesToDf(self):
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

        # Making sure that act Packets goes first
        self.actPacketsPerIp = dict(
            sorted(self.actPacketsPerIp.items(), key=lambda item: item[1], reverse=True)
        )
        i = 0

        for ip in self.actPacketsPerIp:
            df.loc[i] = [
                ip,
                self.packetCountPerIp[ip],
                list(set(self.unusualPorts[ip])),
                self.ipFrequency[ip],
                self.synPacketsPerIp[ip],
                self.synActPacketsPerIp[ip],
                self.actPacketsPerIp[ip],
            ]
            i += 1

        for ip in self.ipFrequency:
            if ip not in self.actPacketsPerIp:
                df.loc[i] = [
                    ip,
                    self.packetCountPerIp[ip],
                    list(set(self.unusualPorts[ip])),
                    self.ipFrequency[ip],
                    self.synPacketsPerIp[ip],
                    self.synActPacketsPerIp[ip],
                    0,
                ]
                i += 1

        writeToLogPy(info, "Extraction of features was successful")
        return df
