# Vine

#var duration 5
#var stepInterval 0.0333
var steps 5
var delta 10
var n 20
var r 1

var leaf "/Users/steve/projects/nightgarden/data/cordateLeaf.lsc"

axiom A
rule A -> ST[-B]C[+C]
rule B -> ST+S-T-S[STSC][+C]-S-TC
rule C -> ST-S+T+S[B][-B]+S+TB
rule S -> F[^(110)~(leaf)]
rule T -> F[^(80)~(leaf)]