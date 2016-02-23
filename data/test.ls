# Bracketed OL System Test 1
var delta 25.7
var n 4
axiom F
rule F -> F[+F]F[-F]F


# Bracketed OL System Test 2
#var delta 22.5
#var n 4
#axiom F
#rule F -> FF-[-F+F+F]+[+F-F-F]

# Koch curve
#var n 2
#var delta 90
#axiom F-F-F-F
#rule F -> F+FF-FF-F-F+F+F

# Koch curve
#var n 4
#var delta 90
#axiom -F
#rule F -> F+F-F-F+F

# FASS Curve
#var n 6
#var delta 90
#axiom -L
#rule L ->LF+RFR+FL-F-LFLFL-FRFR+
#rule R -> -LFLF+RFRFR+F+RF-LFL-FR



#var n 8
#var delta 22.5
#axiom X
#rule X -> F[[X]+X]+F[+FX]-X
#rule F -> FF

#var n 7
#var delta 25.7
#axiom X
#rule X -> F[+X][-X]FX
#rule F -> FF

# axiom b(1)aaaaaa
#
# rule b(x) < a : x<3 -> b(x+1)
# rule b(x) < a : x>=3 ->b(x)
# rule b -> a

# rule A -> [&FL!A]/////'[&FL!A]///////'[&FL!A]
# rule F -> S/////F
# rule S -> F L
# rule L -> ['''^^{-f+f+f-|-f+f+f}]
