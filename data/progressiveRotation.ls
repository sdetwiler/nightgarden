var steps 4

axiom A(0)

rule A(d) -> F(d)+(d+10)A(d+10)
rule A -> F