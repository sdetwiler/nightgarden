# Cordate Leaf
var steps 18
var n 1
var delta 10

axiom [A][B]
rule A -> [+A{.].C.}
rule B -> [-B{.].C.}
rule C -> G(1)C