# Cylindrical model test
var steps 12
var a 137.5281
var h 35.3
var r 500
var rd 100.0
axiom A
rule A -> [&(90)f(r)~D]f(h)/(a)A
rule D -> [^(90){(239, 217, 17)VVVVVVVV}]
rule V -> f(rd)+(45)
