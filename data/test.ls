# Bracketed OL System Test 1
#var delta 25.7
#var n 4
#axiom F
#rule F -> F[+F]F[-F]F


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

# Bush
#var n 20
#var delta 22.5
#axiom A
#rule A -> [&FL!A]/////'[&FL!A]///////'[&FL!A]
#rule F -> S/////F
#rule S -> F L
#rule L -> ['''^^{(0,128,0)-f+f+f-|-f+f+f}]


# Figure 1.26 A Plant
var n 8
var delta 18

axiom P

# Test Rule for debugging.
rule T -> F+F+F+F

rule P -> I+[P+W]--//[--L]I[++L]-[PW]++PW
rule I -> FS[//&&L][//^^L]FS
rule S -> SFS
rule L -> ['{(0,128,0)+f-ff-f+|+f-ff-f}]
rule W ->[&&&D'/G////G////G////G////G]
rule D -> FF
rule G -> ['^F][{(255,255,255)&&&&-f+f|-f+f}]

