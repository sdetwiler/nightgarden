var steps 5
var n 10
var r 2

var leaf "/Users/steve/projects/nightgarden/data/cordateLeaf.lsc"

axiom A(0)

rule A(d) -> F(d)[^(80)~(leaf)]+(d+10)A(d+10)
