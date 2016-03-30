# Vine

var duration 30
var stepInterval 0.033333

#var steps 40

var delta 2
var n 4
var r 1

var leaf "data/compiled/cordateLeaf.lsc"

var numSegments 8

axiom R(0, 10)
rule R(d, a) : d <  numSegments  -> -(a)FR(d+1, a)
rule R(d, a) : d == numSegments  -> [R(d*0.6, a+10)][L(0, a)][A]
rule R(d, a) : d >  numSegments  -> [A]

rule L(d, a) : d <  numSegments  -> +(a)FL(d+1, a)
rule L(d, a) : d == numSegments  -> [L(d*0.6, a+10)][R(0, a)][A]
rule L(d, a) : d >  numSegments  -> [A]

rule A                           -> F[^(45)+(30)~(leaf)][^(-45)-(30)~(leaf)]
