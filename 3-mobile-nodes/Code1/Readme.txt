3 nodes
10 packets each of 1000 bytes are being sent at the rate of 0.01Mbps.
Max TCP payload = 536 bytes. So, segmentation --> 1000 = 536 + 464.


Wifi Phy Mode               - DsssRate1Mbps
Wifi Standard               - WIFI_PHY_STANDARD_80211b
Pcap Data Link Type         - DLT_IEEE802_11
Propogation Delay Model     - ConstantSpeedPropagationDelayModel
Propogation Loss Model      - TwoRayGroundPropagationLossModel [Range: 158.11m]
Wifi Remote Station Manager - ConstantRateWifiManager
Wifi Mac                    - AdhocWifiMac
Mobility Model              - RandomWaypointMobilityModel
Routing Protocol            - OLSR
Transport Layer Protocol    - TCP


nodes:
source            	- node 2 [ip: 10.1.1.3; mac: 00:00:00:00:00:03]
other node 			- node 1 [ip: 10.1.1.2; mac: 00:00:00:00:00:02]
sink              	- node 0 [ip: 10.1.1.1; mac: 00:00:00:00:00:01]


Mobility:
Mobility Model: 	RandomWaypointMobilityModel
PositionAllocator: 	RandomRectanglePositionAllocator
Region: 			300x300 m^2
X & Y coordinate: 	UniformRandomVariable (0-300m)

Note: nodePause and nodeSpeed variables can be tweaked to see the effect of mobility.


Printing:
Distances b/w the nodes: so_si, so_ot, ot_si [so(source), ot(other node), si(sink)]
Congestion Window when ever it changes.


Pcap Tracing is enabled.
NetAnim Code is added.