// This program configures a grid (default 5x5) of nodes on an
// 802.11b physical layer, with
// 802.11b NICs in adhoc mode
//
// The default layout is like this, on a 2-D grid.
//
// n20  n21  n22  n23  n24
// n15  n16  n17  n18  n19
// n10  n11  n12  n13  n14
// n5   n6   n7   n8   n9
// n0   n1   n2   n3   n4
//
// the layout is affected by the parameters given to GridPositionAllocator;
// by default, GridWidth is 5 and numNodes is 25.
//
// There are a number of command-line options available to control
// the default behavior.  The list of available command-line options
// can be listed with the following command:
// ./waf --run "wifi-simple-adhoc-grid --help"
//
// Note that all ns-3 attributes (not just the ones exposed in the below
// script) can be changed at command line; see the ns-3 documentation.
//
// For instance, for this configuration, the physical layer will
// stop successfully receiving packets when distance increases beyond
// the default of 368m.
// To see this effect, try running:
//
// ./waf --run "wifi-simple-adhoc-grid --distance=368"
// ./waf --run "wifi-simple-adhoc-grid --distance=500"
// ./waf --run "wifi-simple-adhoc-grid --distance=260"
//
// Three UDP traffic flows, one along each diagonal and one
// along the middle (at high rates of transmission).
//
// ./waf --run "wifi-simple-adhoc-grid --verbose=1"
//
// By default, trace file writing is off-- to enable it, try:
// ./waf --run "wifi-simple-adhoc-grid --tracing=1"
//
// Three UDP traffic flows, one along each diagonal and one
// along the middle (at high rates of transmission).
//
// When you are done tracing, you will notice many pcap trace files
// in your directory.  If you have tcpdump installed, you can try this:
//
// tcpdump -r wifi-simple-adhoc-grid-0-0.pcap -nn -tt
//
// To analyse FlowMonitor out file:
// python flowmon-parse-results.py my-gridflow.flowmon

#include "ns3/command-line.h"
#include "ns3/config.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "ns3/string.h"
#include "ns3/log.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/mobility-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/mobility-model.h"
#include "ns3/olsr-helper.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/ipv4-list-routing-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/netanim-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/socket.h"
#include "ns3/packet-socket.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("my-grid3");


void ReceivePacket (Ptr<Socket> socket)
{
  Ptr<Packet> packet;
  Address senderAddress;
  while ((packet = socket->RecvFrom (senderAddress)))
    {
      if (InetSocketAddress::IsMatchingType (senderAddress))
      {
        InetSocketAddress addr = InetSocketAddress::ConvertFrom (senderAddress);
        NS_LOG_UNCOND(Simulator::Now ().GetSeconds () << " node:" << socket->GetNode()->GetId () << " received one packet from " << addr.GetIpv4 ());
      }
    else
      {
        NS_LOG_UNCOND(Simulator::Now ().GetSeconds () << " node:" << socket->GetNode()->GetId () << " received one packet");
      }
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
  double distance = 368;  // m
  uint32_t packetSize = 1000; // bytes
  uint32_t numPackets = 10;
  uint32_t numNodes = 25;  // by default, 5x5
  double interval = 5.0; // seconds
  bool verbose = false;
  std::string tr_name ("my-gridflow");
  bool tracing = false;

  CommandLine cmd (__FILE__);
  cmd.AddValue ("phyMode", "Wifi Phy mode", phyMode);
  cmd.AddValue ("distance", "distance (m)", distance);
  cmd.AddValue ("packetSize", "size of application packet sent", packetSize);
  cmd.AddValue ("numPackets", "number of packets generated", numPackets);
  cmd.AddValue ("interval", "interval (seconds) between packets", interval);
  cmd.AddValue ("verbose", "turn on all WifiNetDevice log components", verbose);
  cmd.AddValue ("tracing", "turn on ascii and pcap tracing", tracing);
  cmd.AddValue ("numNodes", "number of nodes", numNodes);
  cmd.Parse (argc, argv);

  // Convert to time object
  Time interPacketInterval = Seconds (interval);

  // Fix non-unicast data rate to be the same as that of unicast
  Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode",
                      StringValue (phyMode));

  NodeContainer c;
  c.Create (numNodes);

  // The below set of helpers will help us to put together the wifi NICs we want
  WifiHelper wifi;
  if (verbose)
    {
      wifi.EnableLogComponents ();  // Turn on all Wifi logging
    }

  YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();
  // set it to zero; otherwise, gain will be added
  wifiPhy.Set ("RxGain", DoubleValue (0) );
  wifiPhy.Set ("TxGain", DoubleValue(0));
  // ns-3 supports RadioTap and Prism tracing extensions for 802.11b
  wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11);

  YansWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.AddPropagationLoss ("ns3::FriisPropagationLossModel");
  wifiPhy.SetChannel (wifiChannel.Create ());

  // Add an upper mac and disable rate control
  WifiMacHelper wifiMac;
  wifi.SetStandard (WIFI_PHY_STANDARD_80211b);
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode",StringValue (phyMode),
                                "ControlMode",StringValue (phyMode));
  // Set it to adhoc mode
  wifiMac.SetType ("ns3::AdhocWifiMac");
  NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, c);

  MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (distance),
                                 "DeltaY", DoubleValue (distance),
                                 "GridWidth", UintegerValue (5),
                                 "LayoutType", StringValue ("RowFirst"));
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (c);

  // Enable OLSR
  OlsrHelper olsr;
  // Ipv4StaticRoutingHelper staticRouting;

  Ipv4ListRoutingHelper list;
  // list.Add (staticRouting, 0);
  list.Add (olsr, 20);

  InternetStackHelper internet;
  internet.SetRoutingHelper (list); // has effect on the next Install ()
  internet.Install (c);

  Ipv4AddressHelper ipv4;
  NS_LOG_INFO ("Assign IP Addresses.");
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i = ipv4.Assign (devices);

  // UDP connection from N0 to N24
  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
  Ptr<Socket> recvSink1 = Socket::CreateSocket (c.Get (24), tid);
  InetSocketAddress local1 = InetSocketAddress (Ipv4Address ("10.1.1.25"), 80);//(Ipv4Address::GetAny (), 80);
  recvSink1->Bind (local1);
  recvSink1->SetRecvCallback (MakeCallback (&ReceivePacket));

  Ptr<Socket> source1 = Socket::CreateSocket (c.Get (0), tid);
  InetSocketAddress remote1 = InetSocketAddress (i.GetAddress (24),80);
  source1->Bind ();
  source1->Connect (remote1);

  // UDP connection from N10 to N14
  Ptr<Socket> recvSink2 = Socket::CreateSocket (c.Get (14), tid);
  InetSocketAddress local2 = InetSocketAddress (Ipv4Address ("10.1.1.15"), 80);//(Ipv4Address::GetAny (), 80);
  recvSink2->Bind (local2);
  recvSink2->SetRecvCallback (MakeCallback (&ReceivePacket));

  Ptr<Socket> source2 = Socket::CreateSocket (c.Get (10), tid);
  InetSocketAddress remote2 = InetSocketAddress (i.GetAddress (14),80);
  source2->Bind ();
  source2->Connect (remote2);

  // UDP connection from N20 to N4
  Ptr<Socket> recvSink3 = Socket::CreateSocket (c.Get (4), tid);
  InetSocketAddress local3 = InetSocketAddress (Ipv4Address ("10.1.1.5"), 80);//(Ipv4Address::GetAny (), 80);
  recvSink3->Bind (local3);
  recvSink3->SetRecvCallback (MakeCallback (&ReceivePacket));

  Ptr<Socket> source3 = Socket::CreateSocket (c.Get (20), tid);
  InetSocketAddress remote3 = InetSocketAddress (i.GetAddress (4),80);
  source3->Bind ();
  source3->Connect (remote3);


  if (tracing == true)
    {
      AsciiTraceHelper ascii;
      wifiPhy.EnableAsciiAll (ascii.CreateFileStream ("wifi-simple-adhoc-grid.tr"));
      wifiPhy.EnablePcap ("wifi-simple-adhoc-grid", devices);
      // Trace routing tables
      Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> ("wifi-simple-adhoc-grid.routes", std::ios::out);
      olsr.PrintRoutingTableAllEvery (Seconds (2), routingStream);
      Ptr<OutputStreamWrapper> neighborStream = Create<OutputStreamWrapper> ("wifi-simple-adhoc-grid.neighbors", std::ios::out);
      olsr.PrintNeighborCacheAllEvery (Seconds (2), neighborStream);

      // To do-- enable an IP-level trace that shows forwarding events only
    }

  // Give OLSR time to converge-- 30 seconds perhaps

    Simulator::ScheduleWithContext (source1->GetNode ()->GetId (),
                                   Seconds (30.0), &GenerateTraffic,
                                   source1, packetSize, numPackets, interPacketInterval);

    Simulator::ScheduleWithContext (source2->GetNode ()->GetId (),
                                   Seconds (31.0), &GenerateTraffic,
                                   source2, packetSize, numPackets, interPacketInterval);

    Simulator::ScheduleWithContext (source3->GetNode ()->GetId (),
                                   Seconds (32.0), &GenerateTraffic,
                                   source3, packetSize, numPackets, interPacketInterval);

    AnimationInterface anim ("animation1.xml");

  // Output what we are doing
  NS_LOG_UNCOND ("Testing with grid distance " << distance);

  Ptr<FlowMonitor> flowmon;
  FlowMonitorHelper flowmonHelper;
  flowmon = flowmonHelper.InstallAll ();

  Simulator::Stop (Seconds (85.0));
  Simulator::Run ();
  flowmon->SerializeToXmlFile ((tr_name + ".flowmon").c_str(), false, false);
  Simulator::Destroy ();

  return 0;
}

