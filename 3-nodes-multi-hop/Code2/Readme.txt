3 nodes; 2 hop communication; source & sink can communicate only via intermediate node
10 packets are being sent from source to sink with interpacket interval of 10sec


Wifi Phy Mode               - DsssRate1Mbps
Wifi Standard               - WIFI_PHY_STANDARD_80211b
Pcap Data Link Type         - DLT_IEEE802_11
Propogation Delay Model     - ConstantSpeedPropagationDelayModel
Propogation Loss Model      - LogDistancePropagationLossModel
Wifi Remote Station Manager - ConstantRateWifiManager
Wifi Mac                    - AdhocWifiMac
Mobility Model              - ConstantPositionMobilityModel [No Mobility]
Routing Protocol            - AODV [If OLSR is needed, comment AODV code and uncomment OLSR code]
Transport Layer Protocol    - UDP


Pcap Tracing is enabled.
NetAnim Code is added.


source            - node 2 [ip: 10.1.1.3; mac: 00:00:00:00:00:03]
intermediate node - node 1 [ip: 10.1.1.2; mac: 00:00:00:00:00:02]
sink              - node 0 [ip: 10.1.1.1; mac: 00:00:00:00:00:01]