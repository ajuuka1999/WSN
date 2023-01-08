# create the simulator
set ns [new Simulator]

set f [open out.tr w]
$ns trace-all $f

# node and connection
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
$ns duplex-link $n0 $n1 5Mb 2ms DropTail
$ns duplex-link $n1 $n2 1Mb 10ms DropTail

# application
# SHOULD specify the node to which the application is attached
set app0 [new Application/Test $n0]
set app1 [new Application/Test $n1]
set app2 [new Application/Test $n2]

# send packet
$ns at 1.2 "$app0 send $app1"
$ns at 2.4 "$app0 send $app2"
$ns at 2.1 "$app1 send $app0"
$ns at 0.9 "$app2 send $app0"

# start & stop simulation
$ns at 5.0 "finish"
proc finish {} {
    global ns f
    $ns flush-trace
    close $f
    exit 0
}

$ns run

