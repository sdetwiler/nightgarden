# Cordate Leaf
var steps 20
var n 20
var delta 10

axiom [A][B]
rule A -> [+A{.].C.}
rule B -> [-B{.].C.}
rule C -> GC