Only changes (wrt settings 1)

Mobility Model      - RandomWalk2dMobilityModel [This model is often identified as a brownian motion model]
Routing Protocol    - AODV

Mobility:
DeltaX    : 260 m
DeltaY    : 260 m
Bounds    : X->0|1300 & Y->0|1300
Speed     : UniformRandomVariable[Min=8.0|Max=10.0]
Distance  : 20.0 [Direction and Speed changes after moving this distance]

FlowID: 1 starts at 30sec
FlowID: 2 starts at 30sec
FlowID: 3 starts at 30sec

Output of FlowMonitor: [Flow of our interest (out of more than 100 flow; because of aodv and random mobility)]
FlowID: 1 (UDP 10.1.1.1/49153 --> 10.1.1.25/80)
	TX bitrate: 1.83 kbit/s
	RX bitrate: 1.73 kbit/s
	Mean Delay: 699.00 ms
	Packet Loss Ratio: 10.00 %
FlowID: 2 (UDP 10.1.1.11/49153 --> 10.1.1.15/80)
	TX bitrate: 1.83 kbit/s
	RX bitrate: 1.65 kbit/s
	Mean Delay: 570.25 ms
	Packet Loss Ratio: 10.00 %
FlowID: 3 (UDP 10.1.1.21/49153 --> 10.1.1.5/80)
	TX bitrate: 1.83 kbit/s
	RX bitrate: 1.13 kbit/s
	Mean Delay: 977.37 ms
	Packet Loss Ratio: 40.00 %
