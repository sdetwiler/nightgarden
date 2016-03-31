# Vine

#var duration 30
#var stepInterval 0.01666

var steps 240

var delta 2
var n 4
var r 1

var leaf "data/compiled/cordateLeaf.lsc"

var numSegments 6

#axiom R(0, 2)
axiom X
rule R(d, a) : d <  numSegments  -> -(a)FR(d+1, a)
rule R(d, a) : d == numSegments  -> [L(0, a)][+(90)A]
rule R(d, a) : d >  numSegments  -> [A]

rule L(d, a) : d <  numSegments  -> +(a)FL(d+1, a)
rule L(d, a) : d == numSegments  -> [R(0, a)][-(90)A]
rule L(d, a) : d >  numSegments  -> [A]

rule X -> ~(leaf)

rule A                           -> [+(45)F~(leaf)][-(45)F~(leaf)]
