from ..featuresEngineering.featuresEngineering import GetFeatures
import numpy as np
import pandas as pd
from sklearn.ensemble import IsolationForest
from ...logger.logger import *


class IsolationForestAnalysis:
    def __init__(self, session, contamination=0.1):
        self.session = session
        self.dataSet = None
        self.isoForest = IsolationForest(contamination=contamination)

    def run(self):
        writeToLogPy(info, "Starting Isolation Forest")
        self.getData()
        self.train()

    def getData(self):
        self.dataSet = GetFeatures(self.session).run()

    def train(self):
        self.isoForest.fit(self.dataSet)

    def predict(self, newData):
        predictions = self.isoForest.predict(newData)
        anomaly = self.isoForest.score_samples(newData)
        decision = self.isoForest.decision_function(newData)
        resultDf = pd.DataFrame(
            {
                "predictions": predictions,
                "anomaly": anomaly,
                "decision": decision,
                "isAnomaly": predictions == -1,
            }
        )
        return resultDf

    def test(self):
        return self.predict(self.dataSet)
