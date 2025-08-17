import packetCapturePy
import threading
import time


t = threading.Thread(target=packetCapturePy.start)
t.start()



time.sleep(5)  # let it run for 5 seconds

packetCapturePy.stop()  # signal the capturer to stop
t.join()  # wait for thread to finish
print("Capture stopped.")
