/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Hajime Tazaki, NICT
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Hajime Tazaki <tazaki@nict.go.jp>
 */

#include "ns3/network-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/dce-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/quagga-helper.h"

#include "ns3/csma-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("DceQuaggaOspfd");

static void RunIp (Ptr<Node> node, Time at, std::string str)
{
    DceApplicationHelper process;
    ApplicationContainer apps;
    process.SetBinary ("ip");
    process.SetStackSize (1 << 16);
    process.ResetArguments ();
    process.ParseArguments (str.c_str ());
    apps = process.Install (node);
    apps.Start (at);
}

static void AddAddress (Ptr<Node> node, Time at, const char *name, const char *address)
{
    std::ostringstream oss;
    oss << "-f inet addr add " << address << " dev " << name;
    RunIp (node, at, oss.str ());
}

// Parameters
uint32_t stopTime = 300;

int main (int argc, char *argv[])
{
    CommandLine cmd;
    cmd.AddValue ("stopTime", "Time to stop(seconds)", stopTime);
    cmd.Parse (argc,argv);
    
    LogComponentEnable ("DceQuaggaOspfd", LOG_LEVEL_INFO);

    //
    // Step 1
    // Node Basic Configuration
    //
    NS_LOG_INFO ("Create nodes.");
    
    NodeContainer routers;
    routers.Create (6);
    
    NodeContainer ptp01 (routers.Get (0), routers.Get (1));
    NodeContainer ptp02 (routers.Get (0), routers.Get (2));
    NodeContainer ptp13 (routers.Get (1), routers.Get (3));
    NodeContainer ptp14 (routers.Get (1), routers.Get (4));
    NodeContainer ptp24 (routers.Get (2), routers.Get (4));
    NodeContainer ptp35 (routers.Get (3), routers.Get (5));
    NodeContainer ptp45 (routers.Get (4), routers.Get (5));

    NS_LOG_INFO ("Create point-to-point.");
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
    pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
    NetDeviceContainer pp01 = pointToPoint.Install (ptp01);
    NetDeviceContainer pp02 = pointToPoint.Install (ptp02);
    NetDeviceContainer pp13 = pointToPoint.Install (ptp13);
    NetDeviceContainer pp14 = pointToPoint.Install (ptp14);
    NetDeviceContainer pp24 = pointToPoint.Install (ptp24);
    NetDeviceContainer pp35 = pointToPoint.Install (ptp35);
    NetDeviceContainer pp45 = pointToPoint.Install (ptp45);
    
    //
    // Step 2
    // Address Configuration
    //
    //
    DceManagerHelper processManager;
    //processManager.SetLoader ("ns3::DlmLoaderFactory");
    processManager.SetTaskManagerAttribute ("FiberManagerType", EnumValue (0));
    processManager.SetNetworkStack ("ns3::LinuxSocketFdFactory", "Library", StringValue("liblinux.so"));
    processManager.Install (routers);

    NS_LOG_INFO ("Create networks and assign IP Addresses.");
    AddAddress (routers.Get (0), Seconds (0.1), "sim0", "10.0.1.1/24");
    AddAddress (routers.Get (0), Seconds (0.1), "sim1", "10.0.2.1/24");
    RunIp (routers.Get (0), Seconds (0.11), "link set lo up");
    RunIp (routers.Get (0), Seconds (0.11), "link set sim0 up");
    RunIp (routers.Get (0), Seconds (0.11), "link set sim1 up");
    
    AddAddress (routers.Get (1), Seconds (0.1), "sim0", "10.0.1.2/24");
    AddAddress (routers.Get (1), Seconds (0.1), "sim1", "10.1.3.1/24");
    AddAddress (routers.Get (1), Seconds (0.1), "sim2", "10.1.4.1/24");
    RunIp (routers.Get (1), Seconds (0.11), "link set lo up");
    RunIp (routers.Get (1), Seconds (0.11), "link set sim0 up");
    RunIp (routers.Get (1), Seconds (0.11), "link set sim1 up");
    RunIp (routers.Get (1), Seconds (0.11), "link set sim2 up");
    
    AddAddress (routers.Get (2), Seconds (0.1), "sim0", "10.0.2.2/24");
    AddAddress (routers.Get (2), Seconds (0.1), "sim1", "10.2.4.1/24");
    RunIp (routers.Get (2), Seconds (0.11), "link set lo up");
    RunIp (routers.Get (2), Seconds (0.11), "link set sim0 up");
    RunIp (routers.Get (2), Seconds (0.11), "link set sim1 up");
    
    AddAddress (routers.Get (3), Seconds (0.1), "sim0", "10.1.3.2/24");
    AddAddress (routers.Get (3), Seconds (0.1), "sim1", "10.3.5.1/24");
    RunIp (routers.Get (3), Seconds (0.11), "link set lo up");
    RunIp (routers.Get (3), Seconds (0.11), "link set sim0 up");
    RunIp (routers.Get (3), Seconds (0.11), "link set sim1 up");
    
    AddAddress (routers.Get (4), Seconds (0.1), "sim0", "10.1.4.2/24");
    AddAddress (routers.Get (4), Seconds (0.1), "sim1", "10.2.4.2/24");
    AddAddress (routers.Get (4), Seconds (0.1), "sim2", "10.4.5.1/24");
    RunIp (routers.Get (4), Seconds (0.11), "link set lo up");
    RunIp (routers.Get (4), Seconds (0.11), "link set sim0 up");
    RunIp (routers.Get (4), Seconds (0.11), "link set sim1 up");
    RunIp (routers.Get (4), Seconds (0.11), "link set sim2 up");
    
    AddAddress (routers.Get (5), Seconds (0.1), "sim0", "10.3.5.2/24");
    AddAddress (routers.Get (5), Seconds (0.1), "sim1", "10.4.5.2/24");
    RunIp (routers.Get (5), Seconds (0.11), "link set lo up");
    RunIp (routers.Get (5), Seconds (0.11), "link set sim0 up");
    RunIp (routers.Get (5), Seconds (0.11), "link set sim1 up");
    
    RunIp (routers.Get (0), Seconds (0.2), "link show");
    RunIp (routers.Get (0), Seconds (0.3), "route show table all");
    RunIp (routers.Get (0), Seconds (0.4), "addr list");

    QuaggaHelper quagga;
    quagga.EnableOspf (ptp01, "10.0.1.0/24");
    quagga.EnableOspf (ptp02, "10.0.2.0/24");
    quagga.EnableOspf (ptp13, "10.1.3.0/24");
    quagga.EnableOspf (ptp14, "10.1.4.0/24");
    quagga.EnableOspf (ptp24, "10.2.4.0/24");
    quagga.EnableOspf (ptp35, "10.3.5.0/24");
    quagga.EnableOspf (ptp45, "10.4.5.0/24");
    quagga.EnableOspfDebug (routers);
    quagga.EnableZebraDebug (routers);
    quagga.Install (routers);
      
    NS_LOG_INFO ("Create applications.");
    DceApplicationHelper dce;
    ApplicationContainer apps;

    dce.SetStackSize (1 << 20);

    // Launch iperf client on node 0
    /*dce.SetBinary ("iperf");
    dce.ResetArguments ();
    dce.ResetEnvironment ();
    dce.AddArgument ("-c");
    dce.AddArgument ("10.4.5.1");

    apps = dce.Install (routers.Get (0));
    apps.Start (Seconds (60.5));
    apps.Stop (Seconds (stopTime));

    // Launch iperf server on node 
    dce.SetBinary ("iperf");
    dce.ResetArguments ();
    dce.ResetEnvironment ();
    dce.AddArgument ("-s");

    apps = dce.Install (routers.Get (4));

    apps.Start (Seconds (60.0));*/
    
  dce.SetBinary ("iperf");
  dce.ResetArguments ();
  dce.ResetEnvironment ();
  dce.AddArgument ("-c");
  dce.AddArgument ("10.4.5.1");
  dce.AddArgument ("-i");
  dce.AddArgument ("1");
  dce.AddArgument ("--time");
  dce.AddArgument ("10");

  apps = dce.Install (routers.Get (0));
  apps.Start (Seconds (60.7));
  apps.Stop (Seconds (80));

  // Launch iperf server on node 1
  dce.SetBinary ("iperf");
  dce.ResetArguments ();
  dce.ResetEnvironment ();
  dce.AddArgument ("-s");
  dce.AddArgument ("-P");
  dce.AddArgument ("1");

  apps = dce.Install (routers.Get (4));
  apps.Start (Seconds (60.2));
      
  RunIp (routers.Get (1), Seconds (64.5), "link set sim2 down");
  //RunIp (routers.Get (2), Seconds (64.5), "link set sim1 down");

    pointToPoint.EnablePcapAll ("dce-quagga-ospfd");

    //
    // Step 9
    // Now It's ready to GO!
    //
    NS_LOG_INFO ("Run Simulation");
    if (stopTime != 0)
        {
            Simulator::Stop (Seconds (stopTime));
        }
    Simulator::Run ();
    Simulator::Destroy ();

    return 0;
}
