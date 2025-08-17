import packetCapturePy
import sys

print(
    f"Python version: {sys.version_info.major}.{sys.version_info.minor}.{sys.version_info.micro}"
)

packetCapturePy.initialize()
fp = packetCapturePy.FeaturePacket()

for i in range(5):
    fp = packetCapturePy.getPacket()
    print(fp)


packetCapturePy.stop()  # signal the capturer to stop
