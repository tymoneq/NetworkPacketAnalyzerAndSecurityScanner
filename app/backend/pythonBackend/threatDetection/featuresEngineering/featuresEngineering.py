import pandas as pd
import numpy as np
from ...dataBaseManager.models import FeaturePacketModel
import datetime
from ...logger.logger import *
import warnings
from sklearn.preprocessing import StandardScaler
from .columnNames import columnList, choseColumns

warnings.simplefilter(action="ignore", category=FutureWarning)


class GetFeatures:
    # time window is in minutes
    def __init__(self, session, timeWindow=5, daysBack=7):
        """The function initializes an object with session, time window, days back, and a DataFrame.

        Parameters
        ----------
        session
            The `session` parameter in the `__init__` method is typically used to store a reference to the
        current session or connection object. This could be a session object for making API requests, a
        database connection object, or any other relevant session object that the class needs to interact
        with during its lifetime
        timeWindow, optional
            The `timeWindow` parameter in the `__init__` method is set to a default value of 5. This
        parameter is used to specify the time window for some operation or calculation within the class.
        When an instance of the class is created, the user can provide a specific value for `
        daysBack, optional
            The `daysBack` parameter in the `__init__` method is used to specify the number of days to look
        back for data. In this case, the default value is set to 7 days. This parameter is likely used in
        the code to retrieve historical data or perform calculations based on data

        """
        self.session = session
        self.timeWindow = timeWindow
        self.daysBack = daysBack
        self.df = pd.DataFrame(columns=columnList)
        self.scaler = StandardScaler()

    def run(self):
        '''The `run` function loads a model to a DataFrame, extracts features from the packets, aggregates
        the data, scales it, and returns the extracted columns.
        
        Returns
        -------
            The `extractedColumns()` method is being called and its return value is being returned from the
        `run()` method.
        
        '''
        packets = self.loadModelToDf()
        self.extractFeatures(packets)
        self.aggregate()
        self.scale()
        return self.extractedColumns()

    def loadModelToDf(self):
        """This function is intended to load a model into a DataFrame."""
        writeToLogPy(info, "Loading features into memory")

        endDate = datetime.datetime.now(datetime.timezone.utc)
        startDate = endDate - datetime.timedelta(days=self.daysBack)

        packets = (
            self.session.query(FeaturePacketModel)
            .filter(FeaturePacketModel.createdAt > startDate)
            .all()
        )

        return packets

    def extractFeatures(self, packets):
        """This function extracts features from a list of packets.

        Parameters
        ----------
        packets
            A list of network packets that you want to extract features from. Each packet in the list
        contains information such as source IP address, destination IP address, protocol type, packet
        size, timestamp, etc.

        """

        writeToLogPy(info, "Starting to extract features")
        for pack in packets:
            newRowsData = self.extractDate(pack.createdAt)
            newRowsData["srcIp"] = pack.srcIp
            newRowsData["dstIp"] = pack.dstIp
            newRowsData["sourcePort"] = pack.sourcePort
            newRowsData["destinationPort"] = pack.destinationPort
            newRowsData["totalLength"] = int(pack.totalLength)
            newRowsData["tcpFlags"] = pack.tcpFlags
            newRowsData["protocol"] = pack.protocol
            newRowsData["ttl"] = pack.ttl

            newRowsDf = pd.DataFrame(newRowsData)
            newRowsDf["isWeekend"] = newRowsDf["dayOfTheWeek"].isin([5, 6]).astype(int)
            newRowsDf["isNight"] = (
                newRowsDf["hour"].isin([22, 23, 24, 0, 1, 2, 3, 4, 5, 6]).astype(int)
            )

            self.df = pd.concat([self.df, newRowsDf], ignore_index=True)

    def aggregate(self):
        '''The function `aggregate` aggregates network traffic features based on specified groupings and
        calculates additional derived features.
        
        '''
       
        writeToLogPy(info, "Starting to aggregate features")

        agg_features = (
            self.df.groupby(
                ["srcIp", "timeWindow", "isWeekend", "isNight", "dayOfTheWeek", "hour"]
            )
            .agg(
                {
                    "dstIp": ["count", "nunique"],
                    "sourcePort": "nunique",
                    "destinationPort": [
                        "nunique",
                        lambda x: x.mode().iloc[0] if not x.empty else 0,
                    ],
                    "totalLength": ["mean", "std", "sum"],
                    "tcpFlags": lambda x: (x == 2).sum(),
                    "protocol": lambda x: x.mode().iloc[0] if not x.empty else 0,
                    "ttl": ["mean", "std"],
                }
            )
            .reset_index()
        )

        # Flatten column names
        agg_features.columns = [
            "srcIp",
            "timeWindow",
            "isWeekend",
            "isNight",
            "dayOfTheWeek",
            "hour",
        ] + [
            "connCount",
            "uniqueDstIps",
            "uniqueSrcPorts",
            "uniqueDstPorts",
            "commonDstPort",
            "avgPacketSize",
            "stdPacketSize",
            "totalBytes",
            "synCount",
            "mainProtocol",
            "avgTtl",
            "stdTtl",
        ]

        agg_features["synRatio"] = agg_features["synCount"] / agg_features["connCount"]
        agg_features["dstIpEntropy"] = (
            agg_features["uniqueDstIps"] / agg_features["connCount"]
        )
        agg_features["portEntropy"] = (
            agg_features["uniqueDstPorts"] / agg_features["connCount"]
        )
        agg_features["bytesPerConn"] = (
            agg_features["totalBytes"] / agg_features["connCount"]
        )

        agg_features = agg_features.fillna(0)

        self.df = agg_features

    def scale(self):
        '''The function `scale` performs cyclical encoding on time-related columns and scales the remaining
        columns using a scaler.
        
        '''
        
        writeToLogPy(info, "Starting scaling data")
        # encode timeStamp
        # Cyclical encoding

        self.df["hourSin"] = np.sin(2 * np.pi * self.df["hour"] / 24)
        self.df["hourCos"] = np.cos(2 * np.pi * self.df["hour"] / 24)
        self.df["dowSin"] = np.sin(2 * np.pi * self.df["dayOfTheWeek"] / 7)
        self.df["dowCos"] = np.cos(2 * np.pi * self.df["dayOfTheWeek"] / 7)

        for column, val in self.df.items():
            if column not in [
                "hour",
                "timeWindow",
                "dayOfTheWeek",
                "isWeekend",
                "isNight",
            ]:
                self.df[column] = self.scaler.fit_transform(self.df[[column]])

    def extractDate(self, date):
        """This function extracts the date from a given input.

        Parameters
        ----------
        date

        """

        new_rows_data = {
            "hour": [date.hour],
            "dayOfTheWeek": [date.weekday()],
            "timeWindow": [self.floorTo5Minutes(date)],
        }

        return new_rows_data

    def floorTo5Minutes(self, date):
        """
        Rounds a datetime object down to the nearest time window interval.
        """
        minutesPastHour = date.minute

        minutesToSubtract = minutesPastHour % self.timeWindow

        flooredDt = date.replace(
            minute=minutesPastHour - minutesToSubtract,
            second=0,
            microsecond=0,
        )

        return flooredDt

    def printHead(self):
        print(self.df.head())

    def extractedColumns(self):
        '''The function `extractedColumns` creates a new DataFrame with selected columns from the original
        DataFrame.
        
        Returns
        -------
            A new DataFrame `newDf` containing only the columns specified in the `choseColumns` list that
        exist in the original DataFrame `self.df` is being returned. If a column from `choseColumns` does
        not exist in `self.df`, a warning message is logged and that column is not included in the new
        DataFrame.
        
        '''
        newDf = pd.DataFrame()

        for columnName in choseColumns:
            if columnName in self.df.columns:
                writeToLogPy(info, f"Writing {columnName} to newDf")
                newDf[columnName] = self.df[columnName]
            else:
                writeToLogPy(warn, f"Could not find {columnName} in df")

        return newDf
