from . import packetCapturePy
from .dataBaseManager.functions import savePacket
from .dataBaseManager.database import initDb, SessionLocal, closeDb
from .logger.logger import *
from .threatDetection.synDetection.synFloodDetection import SynFloodDetection


writeToLogPy(info, "Starting Python")

initDb()
session = SessionLocal()

packetCapturePy.initialize()
fp = packetCapturePy.FeaturePacket()

writeToLogPy(info, "Starting capturing packages")

for i in range(5):
    fp = packetCapturePy.getPacket()
    savePacket(fp, session)


synFloodDetection = SynFloodDetection(session)
synFloodDetection.printDf()


packetCapturePy.stop()  # signal the capturer to stop
closeDb(session)
