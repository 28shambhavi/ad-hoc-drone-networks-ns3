25 nodes
3 flows; 10 packets in each flow; 1000 bytes per packet; interpacket interval is 5sec


Wifi Phy Mode               - DsssRate1Mbps
Wifi Standard               - WIFI_PHY_STANDARD_80211b
Pcap Data Link Type         - DLT_IEEE802_11
Propogation Delay Model     - ConstantSpeedPropagationDelayModel
Propogation Loss Model      - FriisPropagationLossModel [Range: 368m]
Wifi Remote Station Manager - ConstantRateWifiManager
Wifi Mac                    - AdhocWifiMac
Mobility Model              - ConstantPositionMobilityModel
Routing Protocol            - Static Routing
Transport Layer Protocol    - UDP


nodes:
  n20  n21  n22  n23  n24
  n15  n16  n17  n18  n19
  n10  n11  n12  n13  n14
  n5   n6   n7   n8   n9
  n0   n1   n2   n3   n4

FlowID: 1 (UDP 10.1.1.21/49153 --> 10.1.1.1/80) starts at 30sec [n20 -> n15 -> n10 -> n5 -> n0]
FlowID: 2 (UDP 10.1.1.25/49153 --> 10.1.1.1/80) starts at 31sec [n24 -> n18 -> n12 -> n6 -> n0]
FlowID: 3 (UDP 10.1.1.5 /49153 --> 10.1.1.1/80) starts at 32sec [n4  -> n3  -> n2  -> n1 -> n0]

Note: Can assume n0 as our base station here.


Mobility:
Mobility Model    : ConstantPositionMobilityModel
PositionAllocator : GridPositionAllocator
DeltaX            : 260 m
DeltaY            : 260 m
GridWidth         : 5

Note: With DeltaX = DeltaY = 260m, node can send packet to it's 8-neighbours.


Printing:
Time when a node receive a packet
Node Id of receiving node and ip address of sender node


Pcap tracing enabled.
FlowMonitor code is added.
NetAnim code is added.

Output of FlowMonitor:
FlowID: 1 (UDP 10.1.1.21/49153 --> 10.1.1.1/80)
	TX bitrate: 1.83 kbit/s
	RX bitrate: 1.83 kbit/s
	Mean Delay: 38.91 ms
	Packet Loss Ratio: 0.00 %
FlowID: 2 (UDP 10.1.1.25/49153 --> 10.1.1.1/80)
	TX bitrate: 1.83 kbit/s
	RX bitrate: 1.83 kbit/s
	Mean Delay: 39.24 ms
	Packet Loss Ratio: 0.00 %
FlowID: 3 (UDP 10.1.1.5/49153 --> 10.1.1.1/80)
	TX bitrate: 1.83 kbit/s
	RX bitrate: 1.83 kbit/s
	Mean Delay: 38.35 ms
	Packet Loss Ratio: 0.00 %