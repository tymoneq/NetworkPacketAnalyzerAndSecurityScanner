from . import packetCapturePy
from .dataBaseManager.functions import savePacket, getFeatures
from .dataBaseManager.database import init_db, SessionLocal
from .dataBaseManager.models import FeaturePacketModel
from .logger.logger import *

writeToLogPy(info, "Starting Python")

init_db()
session = SessionLocal()

packetCapturePy.initialize()
fp = packetCapturePy.FeaturePacket()

writeToLogPy(info, "Starting capturing packages")
for i in range(5):
    fp = packetCapturePy.getPacket()
    savePacket(fp, session)


getFeatures(session)


packetCapturePy.stop()  # signal the capturer to stop
