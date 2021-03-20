#include <cmath>
#include "ns3/core-module.h"
#include "ns3/command-line.h"
#include "ns3/config.h"
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
#include "myapp.h"
#include "ns3/netanim-module.h"
 
using namespace ns3;
 
NS_LOG_COMPONENT_DEFINE ("my-3nodes");

static void CwndChange (uint32_t oldCwnd, uint32_t newCwnd)
{
  NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << "\t" << newCwnd);
}

void showPosition (Ptr<Node> node0, Ptr<Node> node1, Ptr<Node> node2, double deltaTime)
{
   Ptr<MobilityModel> mobModel0 = node0->GetObject<MobilityModel> ();
   Ptr<MobilityModel> mobModel1 = node1->GetObject<MobilityModel> ();
   Ptr<MobilityModel> mobModel2 = node2->GetObject<MobilityModel> ();
   Vector3D pos0 = mobModel0->GetPosition ();
   Vector3D pos1 = mobModel1->GetPosition ();
   Vector3D pos2 = mobModel2->GetPosition ();

   uint32_t so_si = sqrt(pow(pos2.x - pos0.x,2)+pow(pos2.y-pos0.y,2)+pow(pos2.z-pos0.z,2));
   uint32_t so_ot = sqrt(pow(pos2.x - pos1.x,2)+pow(pos2.y-pos1.y,2)+pow(pos2.z-pos1.z,2));
   uint32_t ot_si = sqrt(pow(pos1.x - pos0.x,2)+pow(pos1.y-pos0.y,2)+pow(pos1.z-pos0.z,2));

   std::cout << "At " << Simulator::Now ().GetSeconds ()
   			 << ": so-si: " << so_si << "m;"
   			 << " so-ot: "  << so_ot << "m;"
   			 << " ot-si: "  << ot_si << "m;"
             << std::endl;
   
   Simulator::Schedule (Seconds (deltaTime), &showPosition, node0, node1, node2, deltaTime);
 }
 
int main (int argc, char *argv[])
{
	std::string phyMode ("DsssRate1Mbps");
	double rss = -40;  // -dBm
	uint32_t packetSize = 1000; // bytes
	uint32_t numPackets = 10;
	double interval = 10.0; // seconds
	bool verbose = false;
	int nodeSpeed = 19; //in m/s
	int nodePause = 0; //in s
	double deltaTime = 1; //in s
 
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
       
	wifi.SetStandard (WIFI_PHY_STANDARD_80211b);

	//YansWifiPhyHelper wifiPhy;
	YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
	wifiPhy.Set ("RxSensitivity", DoubleValue (-40.0));
	wifiPhy.Set ("TxPowerStart", DoubleValue (20.0));
	wifiPhy.Set ("TxPowerEnd", DoubleValue (20.0));
	wifiPhy.Set ("TxGain", DoubleValue(0));
	wifiPhy.Set ("RxGain", DoubleValue(0));
  	   
	wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11);

	YansWifiChannelHelper wifiChannel;
	wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
	wifiChannel.AddPropagationLoss ("ns3::TwoRayGroundPropagationLossModel",
      										"Frequency", DoubleValue(51500000.0),
      										"MinDistance", DoubleValue(0.5),
      										"HeightAboveZ", DoubleValue(5.0));
	wifiPhy.SetChannel (wifiChannel.Create ());

	// Add a mac and disable rate control
	WifiMacHelper wifiMac;
	wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                 "DataMode",StringValue (phyMode),
                                 "ControlMode",StringValue (phyMode));
	// Set it to adhoc mode
	wifiMac.SetType ("ns3::AdhocWifiMac");
	NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, c);

	// MobilityHelper mobility;
	// Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
	// positionAlloc->Add (Vector (0.0, 0.0, 0.0));
	// positionAlloc->Add (Vector (158.0, 0.0, 0.0));
	// positionAlloc->Add (Vector (300.0, 0.0, 0.0));
	// mobility.SetPositionAllocator (positionAlloc);
	// mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
	// mobility.Install (c);

	MobilityHelper mobilityAdhoc;
	int64_t streamIndex = 0; // used to get consistent mobility across scenarios

	ObjectFactory pos;
	pos.SetTypeId ("ns3::RandomRectanglePositionAllocator"); // in region 300x300
	pos.Set ("X", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=300.0]"));
	pos.Set ("Y", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=300.0]"));

	Ptr<PositionAllocator> taPositionAlloc = pos.Create ()->GetObject<PositionAllocator> ();
	streamIndex += taPositionAlloc->AssignStreams (streamIndex);

	std::stringstream ssSpeed;
	ssSpeed << "ns3::UniformRandomVariable[Min=0.0|Max=" << nodeSpeed << "]"; //nodeSpeed 5m/s
	std::stringstream ssPause;
	ssPause << "ns3::ConstantRandomVariable[Constant=" << nodePause << "]"; //nodePause 0
	mobilityAdhoc.SetMobilityModel ("ns3::RandomWaypointMobilityModel",
	                                  "Speed", StringValue (ssSpeed.str ()),
	                                  "Pause", StringValue (ssPause.str ()),
	                                  "PositionAllocator", PointerValue (taPositionAlloc));
	mobilityAdhoc.SetPositionAllocator (taPositionAlloc);
	mobilityAdhoc.Install (c);
	streamIndex += mobilityAdhoc.AssignStreams (c, streamIndex);
	NS_UNUSED (streamIndex); // From this point, streamIndex is unused

	OlsrHelper olsr;

	// Install the routing protocol
	Ipv4ListRoutingHelper list;
	list.Add (olsr, 20);

	// Set up internet stack
	InternetStackHelper internet;
	internet.SetRoutingHelper (list);
	internet.Install (c);
 
	Ipv4AddressHelper ipv4;
	NS_LOG_INFO ("Assign IP Addresses.");
	ipv4.SetBase ("10.1.1.0", "255.255.255.0");
	Ipv4InterfaceContainer i = ipv4.Assign (devices);

	uint16_t sinkPort = 8080;
	Address sinkAddress (InetSocketAddress (i.GetAddress (0), sinkPort));
	PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), sinkPort));
	ApplicationContainer sinkApps = packetSinkHelper.Install (c.Get (0));
	sinkApps.Start (Seconds (50.));
	sinkApps.Stop (Seconds (65.));

	Ptr<Socket> ns3TcpSocket = Socket::CreateSocket (c.Get (2), TcpSocketFactory::GetTypeId ());
	ns3TcpSocket->TraceConnectWithoutContext ("CongestionWindow", MakeCallback (&CwndChange));

	Ptr<MyApp> app = CreateObject<MyApp> ();
	app->Setup (ns3TcpSocket, sinkAddress, packetSize, numPackets, DataRate ("0.01Mbps"));
	c.Get (2)->AddApplication (app);
	app->SetStartTime (Seconds (51.));
	app->SetStopTime (Seconds (65.));
	
	// Tracing
	wifiPhy.EnablePcap ("my-3nodes", devices);
 
	// Output what we are doing
	NS_LOG_UNCOND ("Testing " << numPackets  << " packets sent with receiver rss " << rss );

	Simulator::Schedule (Seconds (49.0), &showPosition, c.Get(0), c.Get(1), c.Get(2), deltaTime);

	AnimationInterface anim ("animation2.xml");
	anim.SetConstantPosition(c.Get(0), 0.0, 0.0);
	anim.SetConstantPosition(c.Get(1), 0.0, 140.0);
	anim.SetConstantPosition(c.Get(2), 0.0, 280.0);

	Simulator::Stop (Seconds(65.0));
	Simulator::Run ();
	Simulator::Destroy ();

	return 0;
}
