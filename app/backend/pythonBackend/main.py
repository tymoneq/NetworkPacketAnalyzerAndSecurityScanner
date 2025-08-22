from . import packetCapturePy
from .dataBaseManager.functions import savePacket
from .dataBaseManager.database import initDb, SessionLocal, closeDb
from .logger.logger import *
from .threatDetection.synDetection.synFloodDetection import SynFloodDetection
from .threatDetection.featuresEngineering.featuresEngineering import GetFeatures

writeToLogPy(info, "Starting Python")

initDb()
session = SessionLocal()

packetCapturePy.initialize()
fp = packetCapturePy.FeaturePacket()

writeToLogPy(info, "Starting capturing packages")

for i in range(50):
    fp = packetCapturePy.getPacket()
    savePacket(fp, session)


# Run in a separate thread

synFloodDetection = SynFloodDetection(session)
synFloodDetection.startSynFloodDetection()

print("TEST")
features = GetFeatures(session)
features.loadModelToDf()


packetCapturePy.stop()  # signal the capturer to stop
closeDb(session)
