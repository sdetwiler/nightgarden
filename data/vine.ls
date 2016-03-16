# Vine

#var steps 12
var duration 15
var stepInterval 0.0333
var delta 40
var n 20
var r 1

axiom A
rule A -> F[+B][-C]
rule B -> F-A
rule C -> F+A
