import pandas as pd
import numpy as np
from ...dataBaseManager.models import FeaturePacketModel
import datetime
from ...logger.logger import *


class GetFeatures:

    def __init__(self, session, timeWindow=5, daysBack=7):
        self.session = session
        self.timeWindow = timeWindow
        self.daysBack = daysBack
        self.df = pd.DataFrame()

    def loadModelToDf(self):
        writeToLogPy(info, "Loading features into memory")

        endDate = datetime.datetime.now(datetime.timezone.utc)
        startDate = endDate - datetime.timedelta(days=self.daysBack)

        packets = (
            self.session.query(FeaturePacketModel)
            .filter(FeaturePacketModel.createdAt > startDate)
            .all()
        )

        self.df = pd.DataFrame(packets)
        print(self.df.head())
