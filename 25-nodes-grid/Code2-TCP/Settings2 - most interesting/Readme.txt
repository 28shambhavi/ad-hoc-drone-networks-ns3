Changes wrt Code1 Settings1

Transport Layer Protocol - TCP

Maximum segment size = 536 bytes; Packet size = 1000 bytes
Thus, segmentation 536+464=1000

Data Rate = 0.002Mbps

Mobility:
DeltaX : 260 m
DeltaY : 260 m

FlowID: 1 (TCP 10.1.1.1 /49153 --> 10.1.1.25/8080) starts at 30sec
FlowID: 2 (TCP 10.1.1.11/49153 --> 10.1.1.15/8080) starts at 30sec
FlowID: 3 (TCP 10.1.1.21/49153 --> 10.1.1.5 /8080) starts at 30sec

Output of FlowMonitor:
FlowID: 1 (TCP 10.1.1.1/49153 --> 10.1.1.25/8080)
	TX bitrate: 3.09 kbit/s
	RX bitrate: 2.59 kbit/s
	Mean Delay: 55.17 ms
	Packet Loss Ratio: 14.81 %
FlowID: 2 (TCP 10.1.1.11/49153 --> 10.1.1.15/8080)
	TX bitrate: 2.57 kbit/s
	RX bitrate: 2.40 kbit/s
	Mean Delay: 237.27 ms
	Packet Loss Ratio: 8.33 %
FlowID: 3 (TCP 10.1.1.21/49153 --> 10.1.1.5/8080)
	TX bitrate: 3.13 kbit/s
	RX bitrate: 2.54 kbit/s
	Mean Delay: 134.84 ms
	Packet Loss Ratio: 18.52 %
FlowID: 4 (TCP 10.1.1.25/8080 --> 10.1.1.1/49153)
	TX bitrate: 0.23 kbit/s
	RX bitrate: 0.22 kbit/s
	Mean Delay: 8.84 ms
	Packet Loss Ratio: 5.00 %
FlowID: 5 (TCP 10.1.1.15/8080 --> 10.1.1.11/49153)
	TX bitrate: 0.17 kbit/s
	RX bitrate: 0.17 kbit/s
	Mean Delay: 10.81 ms
	Packet Loss Ratio: 0.00 %
FlowID: 6 (TCP 10.1.1.5/8080 --> 10.1.1.21/49153)
	TX bitrate: 0.22 kbit/s
	RX bitrate: 0.22 kbit/s
	Mean Delay: 78.11 ms
	Packet Loss Ratio: 0.00 %