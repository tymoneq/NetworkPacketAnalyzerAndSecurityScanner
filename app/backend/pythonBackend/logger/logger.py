from ...pythonBackend import packetCapturePy

# macros for easier logging

info = packetCapturePy.MessagesTypes.info
critical = packetCapturePy.MessagesTypes.critical
warn = packetCapturePy.MessagesTypes.warn
error = packetCapturePy.MessagesTypes.error
trace = packetCapturePy.MessagesTypes.trace
debug = packetCapturePy.MessagesTypes.debug


def writeToLogPy(messageType, message):
    packetCapturePy.writeToLog(messageType, message)
