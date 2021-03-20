 #include "ns3/command-line.h"
 #include "ns3/config.h"
 #include "ns3/double.h"
 #include "ns3/string.h"
 #include "ns3/log.h"
 #include "ns3/yans-wifi-helper.h"
 #include "ns3/mobility-helper.h"
 #include "ns3/ipv4-address-helper.h"
 #include "ns3/yans-wifi-channel.h"
 #include "ns3/mobility-model.h"
 #include "ns3/aodv-module.h"
 #include "ns3/olsr-module.h"
 #include "ns3/internet-stack-helper.h"
 #include "ns3/network-module.h"
 #include "ns3/internet-module.h"
 #include "ns3/netanim-module.h"
 
 using namespace ns3;
 
 NS_LOG_COMPONENT_DEFINE ("my-3nodes");
 
 void ReceivePacket (Ptr<Socket> socket)
 {
   while (socket->Recv ())
     {
       NS_LOG_UNCOND ("Received one packet!");
     }
 }
 
 static void GenerateTraffic (Ptr<Socket> socket, uint32_t pktSize,
                              uint32_t pktCount, Time pktInterval )
 {
   if (pktCount > 0)
     {
       socket->Send (Create<Packet> (pktSize));
       Simulator::Schedule (pktInterval, &GenerateTraffic,
                            socket, pktSize,pktCount - 1, pktInterval);
     }
   else
     {
       socket->Close ();
     }
 }
 
 int main (int argc, char *argv[])
 {
   std::string phyMode ("DsssRate1Mbps");
   double rss = -80;  // -dBm
   uint32_t packetSize = 1000; // bytes
   uint32_t numPackets = 10;
   double interval = 10.0; // seconds
   bool verbose = false;
 
   CommandLine cmd (__FILE__);
   cmd.AddValue ("phyMode", "Wifi Phy mode", phyMode);
   cmd.AddValue ("rss", "received signal strength", rss);
   cmd.AddValue ("packetSize", "size of application packet sent", packetSize);
   cmd.AddValue ("numPackets", "number of packets generated", numPackets);
   cmd.AddValue ("interval", "interval (seconds) between packets", interval);
   cmd.AddValue ("verbose", "turn on all WifiNetDevice log components", verbose);
   cmd.Parse (argc, argv);

   // Convert to time object
   Time interPacketInterval = Seconds (interval);
 
   // Fix non-unicast data rate to be the same as that of unicast
   Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode", StringValue (phyMode));
 
   NodeContainer c;
   c.Create (3);
 
       // The below set of helpers will help us to put together the wifi NICs we want
       WifiHelper wifi;
       if (verbose)
       {
          wifi.EnableLogComponents ();  // Turn on all Wifi logging
       }

       //wifi.SetStandard (WIFI_STANDARD_80211b);
       wifi.SetStandard (WIFI_PHY_STANDARD_80211b);
       
       //YansWifiPhyHelper wifiPhy;
       YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
       wifiPhy.Set ("RxSensitivity", DoubleValue (-80.0));
       wifiPhy.Set ("TxPowerStart", DoubleValue (20.0));
       wifiPhy.Set ("TxPowerEnd", DoubleValue (20.0));

       // This is one parameter that matters when using FixedRssLossModel
       // set it to zero; otherwise, gain will be added
       // wifiPhy.Set ("RxGain", DoubleValue (0) );

       // ns-3 supports RadioTap and Prism tracing extensions for 802.11b
       wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11);
 
      YansWifiChannelHelper wifiChannel;
      wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
      // The below FixedRssLossModel will cause the rss to be fixed regardless
      // of the distance between the two stations, and the transmit power
      wifiChannel.AddPropagationLoss("ns3::LogDistancePropagationLossModel",
                             "Exponent",DoubleValue(3),
                             "ReferenceDistance",DoubleValue(1.0),
                             "ReferenceLoss",DoubleValue(40.02));
      wifiPhy.SetChannel (wifiChannel.Create ());
 
      // Add a mac and disable rate control
      WifiMacHelper wifiMac;
      wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                 "DataMode",StringValue (phyMode),
                                 "ControlMode",StringValue (phyMode));
      // Set it to adhoc mode
      wifiMac.SetType ("ns3::AdhocWifiMac");
      NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, c);
 
      // Note that with FixedRssLossModel, the positions below are not
      // used for received signal strength.
      MobilityHelper mobility;
      Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
      positionAlloc->Add (Vector (0.0, 0.0, 0.0));
      positionAlloc->Add (Vector (99.0, 0.0, 0.0));
      positionAlloc->Add (Vector (198.0, 0.0, 0.0));
      mobility.SetPositionAllocator (positionAlloc);
      mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
      mobility.Install (c);

      // Enable AODV
      AodvHelper aodv;

      // Enable OLSR
  	  // OlsrHelper olsr;

      // Install the routing protocol
      Ipv4ListRoutingHelper list;
      // Uncomment for AODV
      list.Add (aodv, 20);
      // Uncomment for OLSR
      // list.Add (olsr, 20);

      // Set up internet stack
      InternetStackHelper internet;
      internet.SetRoutingHelper (list);
      internet.Install (c);
 
      Ipv4AddressHelper ipv4;
      NS_LOG_INFO ("Assign IP Addresses.");
      ipv4.SetBase ("10.1.1.0", "255.255.255.0");
      Ipv4InterfaceContainer i = ipv4.Assign (devices);
 
      TypeId tid              = TypeId::LookupByName ("ns3::UdpSocketFactory");
      Ptr<Socket> recvSink    = Socket::CreateSocket (c.Get (0), tid);
      InetSocketAddress local = InetSocketAddress (Ipv4Address ("10.1.1.1"), 80);
      recvSink->Bind (local);
      recvSink->SetRecvCallback (MakeCallback (&ReceivePacket));
 
      Ptr<Socket> source = Socket::CreateSocket (c.Get (2), tid);
      InetSocketAddress remote = InetSocketAddress (Ipv4Address ("10.1.1.1"), 80);
      source->SetAllowBroadcast (true);//(false);
      source->Bind ();
      source->Connect (remote);
 
      // Tracing
      wifiPhy.EnablePcap ("my-3nodes", devices);
 
      // Output what we are doing
      NS_LOG_UNCOND ("Testing " << numPackets  << " packets sent with receiver rss " << rss );
 
      Simulator::ScheduleWithContext (source->GetNode ()->GetId (),
                                   Seconds (8.5), &GenerateTraffic,
                                   source, packetSize, numPackets, interPacketInterval);

      AnimationInterface anim ("animation1.xml");
      anim.SetConstantPosition(c.Get(0), 0.0, 0.0);
      anim.SetConstantPosition(c.Get(1), 0.0, 99.0);
      anim.SetConstantPosition(c.Get(2),0.0,198.0);
 
      
      Simulator::Stop (Seconds(100.0));
      Simulator::Run ();
      Simulator::Destroy ();
 
      return 0;
 }
