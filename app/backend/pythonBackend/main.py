import packetCapturePy


packetCapturePy.initialize()
fp = packetCapturePy.FeaturePacket()

for i in range(5):
    fp = packetCapturePy.getPacket()
    print(fp)


packetCapturePy.stop()  # signal the capturer to stop
