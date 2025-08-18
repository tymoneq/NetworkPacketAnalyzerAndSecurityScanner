from sqlalchemy import Column, BigInteger, Integer, Float
from .database import Base


class FeaturePacketModel(Base):
    __tablename__ = "featurePacket"

    id = Column(Integer, primary_key=True, autoincrement=True)
   
    # layer 2
    srcMac = Column(BigInteger)
    dstMac = Column(BigInteger)
    etherType = Column(Integer)
    
    # layer 3
    srcIp = Column(BigInteger)
    dstIp = Column(BigInteger)
    protocol = Column(Integer)
    ttl = Column(Integer)
    totalLength = Column(Integer)
    fragmentFlags = Column(Integer)
    
    # layer 4
    sourcePort = Column(Integer)
    destinationPort = Column(Integer)
    lengthOrWindow = Column(Integer)
    tcpFlags = Column(Integer)
    
    # Optional
    # packetRate = Column(Float)
    # bytesPerFlow = Column(Float)