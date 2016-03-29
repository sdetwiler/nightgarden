# Vine

#var duration 5
#var stepInterval 0.0333

var steps 40

var delta 2
var n 4
var r 1

var leaf "data/compiled/cordateLeaf.lsc"

var numSegments 8

axiom R(0)
rule R(d) : d <  numSegments  -> -(9)FR(d+1)
rule R(d) : d >= numSegments  -> [L(0)][A]
rule L(d) : d <  numSegments  -> +(9)FL(d+1)
rule L(d) : d >= numSegments  -> [R(0)][A]
rule A                        -> F[^(45)~(leaf)][^(-45)~(leaf)]
