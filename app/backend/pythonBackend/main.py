from . import packetCapturePy
from .dataBaseManager.functions import savePacket
from .dataBaseManager.database import initDb, SessionLocal, closeDb
from .logger.logger import *
from .threatDetection.synDetection.synFloodDetection import SynFloodDetection
from .threatDetection.machineLearning.isolatedForest import IsolationForestAnalysis

writeToLogPy(info, "Starting Python")

initDb()
session = SessionLocal()

packetCapturePy.initialize()
fp = packetCapturePy.FeaturePacket()

writeToLogPy(info, "Starting capturing packages")

for i in range(5):
    fp = packetCapturePy.getPacket()
    savePacket(fp, session)


# Run in a separate thread

synFloodDetection = SynFloodDetection(session)
synFloodDetection.startSynFloodDetection()


isoForest = IsolationForestAnalysis(session)
isoForest.run()



packetCapturePy.stop()  # signal the capturer to stop
closeDb(session)
