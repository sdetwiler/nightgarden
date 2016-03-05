var steps 10

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
#var n 10
#var delta 18
#
#var leafRed 0
#var leafGreen 128
#var leafBlue 0
#
#var flowerRed 255
#var flowerGreen 255
#var flowerBlue 255
#
#axiom P
#
#rule P -> I+[P+W]--//[--L]I[++L]-[PW]++PW
#rule I -> FS[//&&L][//^^L]FS
#rule S -> SFS
#rule L -> ['{(leafRed, leafGreen, leafBlue)+f-ff-f+|+f-ff-f}]
#rule W -> [&&&D'/G////G////G////G////G]
#rule D -> FF
#rule G -> ['^F][{(flowerRed, flowerGreen, flowerBlue)&&&&-f+f|-f+f}]
#

# Leaf
#var n 20
#var delta 18
#axiom L
#rule L -> {X[++++G.][++GG.][+GGG.][GGGGG.][-GGG.][--GG.][----G.]}


# Cordate Leaf
var n 20
var delta 10

axiom [A][B]
rule A -> [+A{.].C.}
rule B -> [-B{.].C.}
rule C -> GC




# Lilly of the Valley
#axiom [X(36)A]/(72)[X(36)B]
#rule A -> [&GA{.].
#rule B -> B&.G.}
#rule X(a) -> X(a+4.5)

