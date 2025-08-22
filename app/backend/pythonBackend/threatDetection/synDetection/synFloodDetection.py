import pandas as pd
import numpy as np
from ..featuresEngineering.synFloodFeaturesEngineering import AnalyzePackets
from ...logger.logger import *


class SynFloodDetection:
    def __init__(
        self,
        session,
        timeWindow=5,  # in minutes
        synThreshold=100,
        ratioThreshold=10,
        halfOpenThreshold=50,
    ):
        self.session = session
        self.timeWindow = timeWindow
        self.synThreshold = synThreshold
        self.ratioThreshold = ratioThreshold
        self.halfOpenThreshold = halfOpenThreshold
        self.derivedFeaturesDf = pd.DataFrame()
        self.analyzePackets = AnalyzePackets(session, timeWindow)

    def checkIfDataFrameNotNone(self):
        '''This function checks if a DataFrame object is not None.
        
        '''
        if self.derivedFeaturesDf.empty:
            self.reloadDataframe()

    def printDf(self):
        self.checkIfDataFrameNotNone()
        print(self.derivedFeaturesDf.head())

    def reloadDataframe(self):
        writeToLogPy(info, "Reloading data frame")
        self.derivedFeaturesDf = self.analyzePackets.getFeatures()

    def detectSynFlood(self):
        halfOpenCnt = self.halfOpenThreshold

        writeToLogPy(info, "Starting Syn Flood Detection")

        # This part of the code is iterating over each row in the `self.derivedFeaturesDf` DataFrame
        # and performing checks related to SYN flood detection. Here's a breakdown of what it does:
        for index, row in self.derivedFeaturesDf.iterrows():
            if row["synFrequency"] >= self.synThreshold:
                writeToLogPy(warn, "Syn Attack Detected")
                # add blocking
                break

            if row["synFrequency"] > 0:
                halfOpenCnt -= row["synFrequency"]

            if row["ackFrequency"] > 0:
                halfOpenCnt += row["ackFrequency"]

            if halfOpenCnt <= 0:
                writeToLogPy(warn, "Syn Attack Detected")
                # add blocking
                break

    def startSynFloodDetection(self):
        self.reloadDataframe()
        self.detectSynFlood()

    def saveDataFrameToCsv(self):
        writeToLogPy(info, "Saving Data to csv")
        self.checkIfDataFrameNotNone()
        self.derivedFeaturesDf.to_csv("derivedFeatures.csv", index=False)
