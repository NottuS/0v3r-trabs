/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA	02111-1307	USA
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"

// Network topology
// //
// //                  10.2.1.0
// //    -------------------------------------
// //    |                                   |
// //   [r0]--------[r1]--------[r2]--------[r3]
// //    |           |           |           |
// //    |  n0  n1   |  n2  n3   |  n4  n5   |  n6  n7
// //    |   |   |   |   |   |   |   |   |   |   |   |
// //    =========   =========   =========   =========
// //    10.1.1.0    10.1.2.0    10.1.3.0    10.1.4.0
// //


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ospfv2ScriptExample");

int 
main (int argc, char *argv[])
{
	bool verbose = true;

	CommandLine cmd;
	cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);

	cmd.Parse (argc,argv);

	if (verbose) {
		LogComponentEnable ("ospfv2ScriptExample", LOG_LEVEL_INFO);
		LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
		LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
		LogComponentEnable ("GlobalRoutingHelper", LOG_LEVEL_INFO);
	}

	NS_LOG_INFO ("Create nodes.");
	
	Ptr<Node> r0 = CreateObject<Node> (true);
	Ptr<Node> n0 = CreateObject<Node> (false);
	Ptr<Node> n1 = CreateObject<Node> (false);
	
	Ptr<Node> r1 = CreateObject<Node> (true);
	Ptr<Node> n2 = CreateObject<Node> (false);
	Ptr<Node> n3 = CreateObject<Node> (false);

	Ptr<Node> r2 = CreateObject<Node> (true);
	Ptr<Node> n4 = CreateObject<Node> (false);
	Ptr<Node> n5 = CreateObject<Node> (false);
	
	Ptr<Node> r3 = CreateObject<Node> (true);
	Ptr<Node> n6 = CreateObject<Node> (false);
	Ptr<Node> n7 = CreateObject<Node> (false);

	NodeContainer net1 (r0, n0, n1);
	NodeContainer net2 (r1, n2, n3);
	NodeContainer net3 (r2, n4, n5);
	NodeContainer net4 (r3, n6, n7);
	NodeContainer ppp1 (r0, r1);
	NodeContainer ppp2 (r1, r2);
	NodeContainer ppp3 (r2, r3);
	NodeContainer ppp4 (r3, r0);
	NodeContainer all (net1, net2, net3, net4);

	NS_LOG_INFO ("Create IPv4 Internet Stack");
	InternetStackHelper internetv4;
	internetv4.Install (all);

	NS_LOG_INFO ("Create point-to-point.");
	PointToPointHelper pointToPoint;
	pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
	pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
	NetDeviceContainer ptp1 = pointToPoint.Install (ppp1);
	NetDeviceContainer ptp2 = pointToPoint.Install (ppp2);
	NetDeviceContainer ptp3 = pointToPoint.Install (ppp3);
	NetDeviceContainer ptp4 = pointToPoint.Install (ppp4);

	NS_LOG_INFO ("Create channels.");
	CsmaHelper csma;
	csma.SetChannelAttribute ("DataRate", DataRateValue (5000000));
	csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
	NetDeviceContainer d1 = csma.Install (net1);
	NetDeviceContainer d2 = csma.Install (net2);
	NetDeviceContainer d3 = csma.Install (net3);
	NetDeviceContainer d4 = csma.Install (net4);

	NS_LOG_INFO ("Create networks and assign IPv4 Addresses.");
	Ipv4AddressHelper ipv4;
	ipv4.SetBase ("10.1.1.0", "255.255.255.0");
	Ipv4InterfaceContainer inet1 = ipv4.Assign (d1);
	inet1.SetRouter (0, true);
	ipv4.SetBase ("10.1.2.0", "255.255.255.0");
	Ipv4InterfaceContainer inet2 = ipv4.Assign (d2);
	inet2.SetRouter (0, true);
	ipv4.SetBase ("10.1.3.0", "255.255.255.0");
	Ipv4InterfaceContainer inet3 = ipv4.Assign (d3);
	inet3.SetRouter (0, true);
	ipv4.SetBase ("10.1.4.0", "255.255.255.0");
	Ipv4InterfaceContainer inet4 = ipv4.Assign (d4);
	inet4.SetRouter (0, true);
	ipv4.SetBase ("10.2.1.0", "255.255.255.0", "0.0.0.1");
	Ipv4InterfaceContainer iptp1 = ipv4.Assign (ptp1);
	ipv4.SetBase ("10.2.1.0", "255.255.255.0", "0.0.0.3");
	Ipv4InterfaceContainer iptp2 = ipv4.Assign (ptp2);
	ipv4.SetBase ("10.2.1.0", "255.255.255.0", "0.0.0.5");
	Ipv4InterfaceContainer iptp3 = ipv4.Assign (ptp3);
	ipv4.SetBase ("10.2.1.0", "255.255.255.0", "0.0.0.7");
	Ipv4InterfaceContainer iptp4 = ipv4.Assign (ptp4);

	UdpEchoServerHelper echoServer (9);

	ApplicationContainer serverApps = echoServer.Install (net3.Get (2));
	serverApps.Start (Seconds (1.0));
	serverApps.Stop (Seconds (10.0));

	UdpEchoClientHelper echoClient (inet3.GetAddress (2), 9);
	echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
	echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
	echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

	ApplicationContainer clientApps1 = echoClient.Install (net1.Get (2));
	clientApps1.Start (Seconds (2.0));
	clientApps1.Stop (Seconds (10.0));
	
	ApplicationContainer clientApps2 = echoClient.Install (net2.Get (2));
	clientApps2.Start (Seconds (2.0));
	clientApps2.Stop (Seconds (10.0));
	
	ApplicationContainer clientApps3 = echoClient.Install (net4.Get (2));
	clientApps3.Start (Seconds (2.0));
	clientApps3.Stop (Seconds (10.0));

    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

	AsciiTraceHelper ascii;
	csma.EnableAsciiAll (ascii.CreateFileStream ("ospfv2.tr"));
	csma.EnablePcapAll ("ospfv2", true);
	pointToPoint.EnablePcapAll ("ospfv2", true);

	NS_LOG_INFO ("Run Simulation.");
	Simulator::Run ();
	Simulator::Destroy ();
	NS_LOG_INFO ("Done.");
	
	return 0;
}
