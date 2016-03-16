var steps 4
var n 10
var r 2

axiom A(0)

rule A(d) -> F(d)+(d+10)A(d+10)
rule A -> F