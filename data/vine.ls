# Vine

#var duration 5
#var stepInterval 0.0333
var steps 5
var delta 10
var n 20
var r 1

var leaf "cordateLeaf.ls"

axiom A
rule A -> FF[-B]C[+C]
rule B -> ~(leaf)FF+F-F-F[FFFC][+C]-F-FC
rule C -> ~(leaf)FF-F+F+F[B][-B]+F+FB
