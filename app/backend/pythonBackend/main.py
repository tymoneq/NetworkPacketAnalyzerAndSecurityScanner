from . import packetCapturePy
from .dataBaseManager.functions import savePacket, readData
from .dataBaseManager.database import init_db, SessionLocal
from .dataBaseManager.models import FeaturePacketModel

init_db()
session = SessionLocal()

packetCapturePy.initialize()
fp = packetCapturePy.FeaturePacket()

for i in range(5):
    fp = packetCapturePy.getPacket()
    print(savePacket(fp,session))


readData(session)


packetCapturePy.stop()  # signal the capturer to stop
