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
// //               n0   n1   r    n2   n3
// //               |    |    _    |    |
// // LAN 10.1.1.0  =========|_|========= LAN 10.1.2.0
// //                       router 
// //



using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("RouterScriptExample");

int 
main (int argc, char *argv[])
{
	bool verbose = true;

	CommandLine cmd;
	cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);

	cmd.Parse (argc,argv);

	if (verbose) {
		LogComponentEnable ("RouterScriptExample", LOG_LEVEL_INFO);
		LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
		LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
	}
	
	NS_LOG_INFO ("Create nodes.");
	Ptr<Node> n0 = CreateObject<Node> ();
	Ptr<Node> n1 = CreateObject<Node> ();
	Ptr<Node> r = CreateObject<Node> ();
	Ptr<Node> n2 = CreateObject<Node> ();
	Ptr<Node> n3 = CreateObject<Node> ();

	NodeContainer net1 (n0, n1, r);
	NodeContainer net2 (r, n2, n3);
	NodeContainer all (n0, n1, r, n2, n3);

	NS_LOG_INFO ("Create IPv4 Internet Stack");
	InternetStackHelper internetv4;
	internetv4.Install (all);

	NS_LOG_INFO ("Create channels.");
	CsmaHelper csma;
	csma.SetChannelAttribute ("DataRate", DataRateValue (5000000));
	csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
	NetDeviceContainer d1 = csma.Install (net1);
	NetDeviceContainer d2 = csma.Install (net2);

	NS_LOG_INFO ("Create networks and assign IPv4 Addresses.");
	Ipv4AddressHelper ipv4;
	ipv4.SetBase ("10.1.1.0", "255.255.255.0");
	Ipv4InterfaceContainer i1 = ipv4.Assign (d1);
	i1.SetRouter (2, true);
	ipv4.SetBase ("10.1.2.0", "255.255.255.0");
	Ipv4InterfaceContainer i2 = ipv4.Assign (d2);
	i2.SetRouter (0, true);

	UdpEchoServerHelper echoServer (9);

	ApplicationContainer serverApps = echoServer.Install (net2.Get (2));
	serverApps.Start (Seconds (1.0));
	serverApps.Stop (Seconds (10.0));

	UdpEchoClientHelper echoClient (i2.GetAddress (2), 9);
	echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
	echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
	echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

	ApplicationContainer clientApps = echoClient.Install (net1.Get (0));
	clientApps.Start (Seconds (2.0));
	clientApps.Stop (Seconds (10.0));

	AsciiTraceHelper ascii;
	csma.EnableAsciiAll (ascii.CreateFileStream ("router.tr"));
	csma.EnablePcapAll ("router", true);

	NS_LOG_INFO ("Run Simulation.");
	Simulator::Run ();
	Simulator::Destroy ();
	NS_LOG_INFO ("Done.");
	
	return 0;
}
