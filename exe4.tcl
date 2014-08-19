set ns_ [new Simulator]
set fd [open exe4.tr w]
$ns_ trace-all $fd
set nfd [open exe4.nam w]
$ns_ namtrace-all-wireless $nfd 500 500
proc stop {} {
	global ns_ fd
	$ns_ flush-trace
	close $fd
}

set topo [new Topography]
$topo load_flatgrid 500 500
set chan [new Channel/WirelessChannel]

$ns_ node-config \
	-adhocRouting DSDV \
	-llType LL \
	-macType Mac/802_11 \
	-ifqType Queue/DropTail/PriQueue \
	-ifqLen 50 \
	-antType Antenna/OmniAntenna \
	-propType Propagation/TwoRayGround \
	-phyType Phy/WirelessPhy \
	-channelType Channel/WirelessChannel \
	-topoInstance $topo \
	-agentTrace ON \
	-routerTrace ON \
	-macTrace OFF \
	-movementTrace OFF

for {set i 0} {$i < 2} {incr i} {
	set node_($i) [$ns_ node]
	$node_($i) random-motion 0
	$ns_ at 170.0 "$node_($i) reset";
}

$node_(0) set X_ 5.0
$node_(0) set Y_ 2.0
$node_(0) set Z_ 0.0

$node_(1) set X_ 390.0
$node_(1) set Y_ 385.0
$node_(1) set Z_ 0.0

$ns_ at 10.0 "$node_(0) setdest 20.0 18.0 1.0"
$ns_ at 50.0 "$node_(1) setdest 25.0 20.0 15.0 "
$ns_ at 100.0 "$node_(1) setdest 490.0 480.0 15.0 "

set tcp [$ns_ create-connection \
	TCP $node_(0) TCPSink $node_(1) $i]
set ftp [$tcp attach-source FTP]
$ns_ at 20.0 "$ftp start"

$ns_ at 170.0 "stop"
$ns_ at 170.0 1 "puts \"NS EXITING...\" ; $ns_ halt"

$ns_ run

