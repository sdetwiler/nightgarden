# Vine

#var duration 5
#var stepInterval 0.0333
var steps 5
var delta 10
var n 20
var r 1

var leaf "/Users/steve/projects/nightgarden/data/cordateLeaf.lsc"

axiom A
rule A -> FF[-B]C[+C]
rule B -> [^(110)~(leaf)]FF+F-F-F[FFFC][+C]-F-FC
rule C -> [^(80)~(leaf)]FF-F+F+F[B][-B]+F+FB
