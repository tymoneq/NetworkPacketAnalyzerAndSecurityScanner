import pandas as pd
import numpy as np
from ...dataBaseManager.functions import getFeatures


class SynFloodDetection:
    def __init__(
        self,
        session,
        timeWindow=1200,  # in seconds
        synThreshold=100,
        ratioThreshold=10,
        halfOpenThreshold=50,
    ):
        self.session = session
        self.derivedFeaturesDf = getFeatures(self.session, timeWindow)
        self.timeWindow = timeWindow
        self.synThreshold = synThreshold
        self.ratioThreshold = ratioThreshold
        self.halfOpenThreshold = halfOpenThreshold

        self.detectSynFlood()

    def printDf(self):
        print(self.derivedFeaturesDf.head())

    def detectSynFlood(self):
        for index, row in self.derivedFeaturesDf.iterrows():
            if row["synFrequency"] >= self.synThreshold:
                print("Syn Flood Detected")


    # implement half open threshold detection