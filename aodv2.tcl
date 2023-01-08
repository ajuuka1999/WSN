#===================================
#     Simulation parameters setup
#===================================
set val(chan)   Channel/WirelessChannel    ;# channel type
set val(prop)   Propagation/TwoRayGround   ;# radio-propagation model
set val(netif)  Phy/WirelessPhy            ;# network interface type
set val(mac)    Mac/802_11                 ;# MAC type
set val(ifq)    Queue/DropTail/PriQueue    ;# interface queue type
set val(ll)     LL                         ;# link layer type
set val(ant)    Antenna/OmniAntenna        ;# antenna model
set val(ifqlen) 10                         ;# max packet in ifq
set val(nn)     14                         ;# number of mobilenodes
set val(rp)     AODV                       ;# routing protocol
set val(x)      1301                       ;# X dimension of topography
set val(y)      601                        ;# Y dimension of topography
set val(stop)   40.0                       ;# time of simulation end

#===================================
#        Initialization        
#===================================
#Create a ns simulator
set ns [new Simulator]

#Setup topography object
set topo       [new Topography]
$topo load_flatgrid $val(x) $val(y)
set god [create-god $val(nn)]

#Open the NS trace file
set tracefile [open aodv2.tr w]
$ns trace-all $tracefile


#Open the NAM trace file
set namfile [open aodv2.nam w]
$ns namtrace-all $namfile
$ns namtrace-all-wireless $namfile $val(x) $val(y)
set chan [new $val(chan)];#Create wireless channel

#===================================
#     Mobile node parameter setup
#===================================
$ns node-config -adhocRouting  $val(rp) \
                -llType        $val(ll) \
                -macType       $val(mac) \
                -ifqType       $val(ifq) \
                -ifqLen        $val(ifqlen) \
                -antType       $val(ant) \
                -propType      $val(prop) \
                -phyType       $val(netif) \
                -channel       $chan \
                -topoInstance  $topo \
                -agentTrace    ON \
                -routerTrace   ON \
                -macTrace      ON \
                -movementTrace ON

#===================================
#        Nodes Definition        
#===================================
#Create 14 nodes
set n0 [$ns node]
$n0 set X_ 101
$n0 set Y_ 500
$n0 set Z_ 0.0
$ns initial_node_pos $n0 20
set n1 [$ns node]
$n1 set X_ 203
$n1 set Y_ 499
$n1 set Z_ 0.0
$ns initial_node_pos $n1 20
set n2 [$ns node]
$n2 set X_ 300
$n2 set Y_ 501
$n2 set Z_ 0.0
$ns initial_node_pos $n2 20
set n3 [$ns node]
$n3 set X_ 395
$n3 set Y_ 500
$n3 set Z_ 0.0
$ns initial_node_pos $n3 20
set n4 [$ns node]
$n4 set X_ 1102
$n4 set Y_ 77
$n4 set Z_ 0.0
$ns initial_node_pos $n4 20
set n5 [$ns node]
$n5 set X_ 1004
$n5 set Y_ 78
$n5 set Z_ 0.0
$ns initial_node_pos $n5 20
set n6 [$ns node]
$n6 set X_ 951
$n6 set Y_ 77
$n6 set Z_ 0.0
$ns initial_node_pos $n6 20
set n7 [$ns node]
$n7 set X_ 876
$n7 set Y_ 77
$n7 set Z_ 0.0
$ns initial_node_pos $n7 20
set n8 [$ns node]
$n8 set X_ 603
$n8 set Y_ 129
$n8 set Z_ 0.0
$ns initial_node_pos $n8 20
set n9 [$ns node]
$n9 set X_ 601
$n9 set Y_ 466
$n9 set Z_ 0.0
$ns initial_node_pos $n9 20
set n10 [$ns node]
$n10 set X_ 696
$n10 set Y_ 265
$n10 set Z_ 0.0
$ns initial_node_pos $n10 20
set n11 [$ns node]
$n11 set X_ 491
$n11 set Y_ 323
$n11 set Z_ 0.0
$ns initial_node_pos $n11 20
set n12 [$ns node]
$n12 set X_ 726
$n12 set Y_ 265
$n12 set Z_ 0.0
$ns initial_node_pos $n12 20
set n13 [$ns node]
$n13 set X_ 541
$n13 set Y_ 293
$n13 set Z_ 0.0
$ns initial_node_pos $n13 20


$god new_node $n0
$n0 color black
$god new_node $n1
$n1 color black
$god new_node $n2
$n2 color black
$god new_node $n3
$n3 color black
$god new_node $n4
$n4 color black
$god new_node $n5
$n5 color black
$god new_node $n6
$n6 color black
$god new_node $n7
$n7 color black
$god new_node $n8
$n8 color black
$god new_node $n9
$n9 color black
$god new_node $n10
$n10 color black
$god new_node $n11
$n11 color black
$god new_node $n12
$n12 color black
$god new_node $n13
$n13 color black

#===================================
#        Agents Definition        
#===================================
set tcp0 [new Agent/TCP]
$ns attach-agent $n8 $tcp0
set sink0 [new Agent/TCPSink]
$ns attach-agent $n9 $sink0
$ns connect $tcp0 $sink0
$tcp0 set packetSize_ 1700
#===================================
#        Applications Definition        
#===================================
#Setup a FTP Application over TCP connection
set ftp0 [new Application/FTP]
$ftp0 attach-agent $tcp0
$ns at 4.0 "$ftp0 start"
$ns at 39.5 "$ftp0 stop"
#===================================
#        Node Movement    
#===================================
$ns at 1.0 "$n0 setdest 1200 500 15"
$ns at 1.0 "$n1 setdest 1175 500 15"
$ns at 1.0 "$n2 setdest 1150 500 15"
$ns at 1.0 "$n3 setdest 1125 500 15"
$ns at 1.0 "$n4 setdest 50 75 15"
$ns at 1.0 "$n5 setdest 75 75 15"
$ns at 1.0 "$n6 setdest 100 75 15"
$ns at 1.0 "$n7 setdest 125 75 15"
$ns at 1.0 "$n8 setdest 1200 129 15"
$ns at 1.0 "$n9 setdest 1200 466 13"
$ns at 1.0 "$n10 setdest 50 265 12"
$ns at 1.0 "$n11 setdest 1200 323 13"
$ns at 1.0 "$n13 setdest 1250 293 20"

#===================================
#        Termination        
#===================================
#Define a 'finish' procedure
proc finish {} {
    global ns tracefile namfile
    $ns flush-trace
    close $tracefile
    close $namfile
    exec ./nam aodv2.nam &
    exit 0
}
for {set i 0} {$i < $val(nn) } { incr i } {
    $ns at $val(stop) "\$n$i reset"
}
$ns at $val(stop) "$ns nam-end-wireless $val(stop)"
$ns at $val(stop) "finish"
$ns at $val(stop) "puts \"done\" ; $ns halt"
$ns run
