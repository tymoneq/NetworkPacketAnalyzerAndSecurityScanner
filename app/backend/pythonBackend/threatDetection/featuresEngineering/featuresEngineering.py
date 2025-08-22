import pandas as pd
import numpy as np
from ...dataBaseManager.models import FeaturePacketModel
import datetime
from ...logger.logger import *


columnList = ["hour", "dayOfTheWeek", "timeWindow"]


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

    def run(self):
        self.loadModelToDf()
        self.extractFeatures()

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
            newRowsDf = pd.DataFrame(newRowsData)

            self.df = pd.concat([self.df, newRowsDf], ignore_index=True)

        print(self.df.head())

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
