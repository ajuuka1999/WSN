
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
set val(ifqlen) 100                        ;# max packet in ifq
set val(nn)     13                         ;# number of mobilenodes
set val(rp)     AODV                       ;# routing protocol
set val(x)      1045                       ;# X dimension of topography
set val(y)      637                        ;# Y dimension of topography
set val(stop)   15.0                       ;# time of simulation end
set val(energy) 100.0                      ;# node initial energy value
set nd_()       null                       ;# Empty array to point at nodes

#===================================
#        Initialization        
#===================================
#Create a ns simulator
set ns [new Simulator]

#Setup topography object
set topo       [new Topography]
$topo load_flatgrid $val(x) $val(y)
create-god $val(nn)

#Open the NS trace file
set tracefile [open outa.tr w]
$ns trace-all $tracefile

#Open the NAM trace file
set namfile [open outa.nam w]
$ns namtrace-all $namfile
$ns namtrace-all-wireless $namfile $val(x) $val(y)


#######################################
#loop to initialise authentication value as zero 

for {set m 0} {$m < $val(nn) } { incr m } {

              set fid [open wsndata/crl/$m.txt w]

              puts $fid "0"

              close $fid
            }

######################################

#===================================
#     Mobile node parameter setup
#===================================

set chan [new $val(chan)];#Create wireless channel
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
#Create 25 nodes
set nd_(0) [$ns node]
$nd_(0) set X_ 100
$nd_(0) set Y_ 500
$nd_(0) set Z_ 0.0
$ns initial_node_pos $nd_(0) 40

set nd_(1) [$ns node]
$nd_(1) set X_ 100
$nd_(1) set Y_ 350
$nd_(1) set Z_ 0.0
$ns initial_node_pos $nd_(1) 40

set nd_(2) [$ns node]
$nd_(2) set X_ 100
$nd_(2) set Y_ 200
$nd_(2) set Z_ 0.0
$ns initial_node_pos $nd_(2) 40

set nd_(3) [$ns node]
$nd_(3) set X_ 100
$nd_(3) set Y_ 50
$nd_(3) set Z_ 0.0
$ns initial_node_pos $nd_(3) 40

set nd_(4) [$ns node]
$nd_(4) set X_ 250
$nd_(4) set Y_ 500
$nd_(4) set Z_ 0.0
$ns initial_node_pos $nd_(4) 40

set nd_(5) [$ns node]
$nd_(5) set X_ 250
$nd_(5) set Y_ 350
$nd_(5) set Z_ 0.0
$ns initial_node_pos $nd_(5) 40

set nd_(6) [$ns node]
$nd_(6) set X_ 250
$nd_(6) set Y_ 200
$nd_(6) set Z_ 0.0
$ns initial_node_pos $nd_(6) 40

set nd_(7) [$ns node]
$nd_(7) set X_ 250
$nd_(7) set Y_ 50
$nd_(7) set Z_ 0.0
$ns initial_node_pos $nd_(7) 40

set nd_(8) [$ns node]
$nd_(8) set X_ 400
$nd_(8) set Y_ 500
$nd_(8) set Z_ 0.0
$ns initial_node_pos $nd_(8) 40

set nd_(9) [$ns node]
$nd_(9) set X_ 400
$nd_(9) set Y_ 350
$nd_(9) set Z_ 0.0
$ns initial_node_pos $nd_(9) 40

set nd_(10) [$ns node]
$nd_(10) set X_ 400
$nd_(10) set Y_ 200
$nd_(10) set Z_ 0.0
$ns initial_node_pos $nd_(10) 40

set nd_(11) [$ns node]
$nd_(11) set X_ 400
$nd_(11) set Y_ 50
$nd_(11) set Z_ 0.0
$ns initial_node_pos $nd_(11) 40

#set nd_(12) [$ns node]
#$nd_(12) set X_ 550
#$nd_(12) set Y_ 500
#$nd_(12) set Z_ 0.0
#$ns initial_node_pos $nd_(12) 40

set nd_(12) [$ns node]
$nd_(12) set X_ 475
$nd_(12) set Y_ 275
$nd_(12) set Z_ 0.0
$ns initial_node_pos $nd_(12) 40


$ns at 0.0 "$nd_(12) label ZINK"
$ns at 0.0 "$nd_(12) color blue"
$nd_(12) color blue
$ns at 0.0 "$nd_(12) add-mark m1 blue hexagon"

$ns at 0.0 "$nd_(6) label DC2"
$ns at 0.0 "$nd_(6) color green"
$nd_(6) color green
$ns at 0.0 "$nd_(6) add-mark m1 blue hexagon"

$ns at 0.0 "$nd_(5) label Attacker"
$ns at 0.0 "$nd_(5) color red"
$nd_(5) color red
$ns at 0.0 "$nd_(5) add-mark m1 red hexagon"


#===================================
#        Authentication Application
#===================================
set app0 [new Application/ECDSA $nd_(6)]
set app1 [new Application/ECDSA $nd_(1)]
$ns at 0.1 "$app0 send $app1"

set app2 [new Application/ECDSA $nd_(6)]
set app3 [new Application/ECDSA $nd_(2)]
$ns at 0.1 "$app2 send $app3"

set app4 [new Application/ECDSA $nd_(6)]
set app5 [new Application/ECDSA $nd_(3)]
$ns at 0.1 "$app4 send $app5"

set app6 [new Application/ECDSA $nd_(6)]
set app7 [new Application/ECDSA $nd_(4)]
$ns at 0.1 "$app6 send $app7"

set app8 [new Application/ECDSA $nd_(6)]
set app9 [new Application/ECDSA $nd_(7)]
$ns at 0.1 "$app8 send $app9"

set app10 [new Application/ECDSA $nd_(6)]
set app11 [new Application/ECDSA $nd_(8)]
$ns at 0.1 "$app10 send $app11"

set app12 [new Application/ECDSA $nd_(6)]
set app13 [new Application/ECDSA $nd_(9)]
$ns at 0.1 "$app12 send $app13"

set app14 [new Application/ECDSA $nd_(6)]
set app15 [new Application/ECDSA $nd_(10)]
$ns at 0.1 "$app14 send $app15"

set app16 [new Application/ECDSA $nd_(6)]
set app17 [new Application/ECDSA $nd_(11)]
$ns at 0.1 "$app16 send $app17"

set app18 [new Application/ECDSA $nd_(6)]
set app19 [new Application/ECDSA $nd_(0)]
$ns at 0.1 "$app18 send $app19"

########################################
#       Data Generator
########################################
proc Randch { min max } {
    set maxFactor [expr [expr $max + 1] - $min]
    set value [expr int([expr rand() * 100])]
    set value [expr [expr $value % $maxFactor] + $min]
    #puts $value
    
    ######################
    
    set fid [open wsndata/data.txt w]
    puts $fid $value
    close $fid
    
    ######################

    return $value
    
}

#===================================
#        Agents Definition        
#===================================
set udp1 [new Agent/UDP]
set null1 [new Agent/Null]
$ns attach-agent $nd_(12) $null1
$ns attach-agent $nd_(6) $udp1
$ns connect $udp1 $null1

#===================================
#        Applications Definition        
#===================================
#Setup a CBR Application over UDP connection
set cbr0 [new Application/Traffic/CBR]
$cbr0 attach-agent $udp1
$cbr0 set packetSize_ 64
$cbr0 set rate_ 0.01Mb
#$cbr0 set random_ null
$ns at 1.0 "$cbr0 start"
$ns at 3.0 "$cbr0 stop"


proc senddat {st et dat} {

  global ns nd_ 
  
  if { $dat <= 50} {
    set dc 6
    for {set i 0} {$i < 11 } { incr i } {  
    
    set udp [new Agent/UDP]
    set null [new Agent/Null]
    $ns attach-agent $nd_($i) $udp
    $ns attach-agent $nd_($dc) $null
    $ns connect $udp $null
   

    set cbr [new Application/Traffic/CBR]
    $cbr attach-agent $udp
    if {$i == 5} {
    $cbr set packetSize_ 512
    $cbr set rate_ 0.7Mb
    } else {
    $cbr set packetSize_ 64
    $cbr set rate_ 0.01Mb
    }
    #$cbr0 set random_ null
  
    $ns at $st "$cbr start"
    $ns at $et "$cbr stop"
    }
   }

 
  return 0
}

proc readdat {} {
  
  set fp [open "wsndata/data.txt" r]
  set file_data [read $fp]
  puts $file_data
  puts "File Data"
  close $fp
  return $file_data
}



for {set i 0} {$i < 3 } { incr i } {

  set st1 [expr $i+4]
  set et1 [expr $st1+5]

  $ns at $i "set a [Randch 1 45 ]" 

  $ns at $i.2 "senddat $st1 $et1 [readdat]" 
  
}            





#===================================
#        Termination        
#===================================
#Define a 'finish' procedure
proc finish {} {
    global ns tracefile namfile
    $ns flush-trace
    close $tracefile
    close $namfile
    exec nam outa.nam &
    exit 0
}
for {set i 0} {$i < $val(nn) } { incr i } {
    $ns at $val(stop) "\$nd_($i) reset"
}
$ns at $val(stop) "$ns nam-end-wireless $val(stop)"
$ns at $val(stop) "finish"
$ns at $val(stop) "puts \"done\" ; $ns halt"
$ns run
