# Conditional LSystem test case
var steps 5
axiom b(1)aaaaaa
rule b(x) < a : x<3 -> b(x+1)
rule b(x) < a : x>=3 ->b(x)
rule b -> a
