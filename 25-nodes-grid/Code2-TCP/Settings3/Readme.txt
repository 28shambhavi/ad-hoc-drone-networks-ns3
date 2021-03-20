Changes wrt Code1 Settings1

Transport Layer Protocol - TCP

Maximum segment size = 536 bytes; Packet size = 1000 bytes
Thus, segmentation 536+464=1000

Data Rate = 0.002Mbps

Mobility:
DeltaX : 260 m
DeltaY : 260 m

FlowID: 1 (TCP 10.1.1.5 /49153 --> 10.1.1.1/8080) starts at 30sec
FlowID: 2 (TCP 10.1.1.21/49153 --> 10.1.1.1/8080) starts at 30sec
FlowID: 3 (TCP 10.1.1.25/49153 --> 10.1.1.1/8080) starts at 30sec

Output of FlowMonitor:
FlowID: 1 (TCP 10.1.1.5/49153 --> 10.1.1.1/8080)
	TX bitrate: 2.48 kbit/s
	RX bitrate: 2.47 kbit/s
	Mean Delay: 43.77 ms
	Packet Loss Ratio: 0.00 %
FlowID: 2 (TCP 10.1.1.21/49153 --> 10.1.1.1/8080)
	TX bitrate: 2.69 kbit/s
	RX bitrate: 2.40 kbit/s
	Mean Delay: 129.09 ms
	Packet Loss Ratio: 12.00 %
FlowID: 3 (TCP 10.1.1.25/49153 --> 10.1.1.1/8080)
	TX bitrate: 2.63 kbit/s
	RX bitrate: 2.39 kbit/s
	Mean Delay: 112.59 ms
	Packet Loss Ratio: 8.33 %
FlowID: 4 (TCP 10.1.1.1/8080 --> 10.1.1.5/49153)
	TX bitrate: 0.14 kbit/s
	RX bitrate: 0.14 kbit/s
	Mean Delay: 8.65 ms
	Packet Loss Ratio: 0.00 %
FlowID: 5 (TCP 10.1.1.1/8080 --> 10.1.1.25/49153)
	TX bitrate: 0.17 kbit/s
	RX bitrate: 0.17 kbit/s
	Mean Delay: 15.09 ms
	Packet Loss Ratio: 0.00 %
FlowID: 6 (TCP 10.1.1.1/8080 --> 10.1.1.21/49153)
	TX bitrate: 0.17 kbit/s
	RX bitrate: 0.17 kbit/s
	Mean Delay: 14.62 ms
	Packet Loss Ratio: 0.00 %