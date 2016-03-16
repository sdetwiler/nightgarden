# Figure 1.26 A Plant

# Explicit step configuration
# var steps 300

# Time based step configuration
var stepInterval 0.016666
var duration 5

var r 1
var n 10
var delta 18
var leafRed 0
var leafGreen 128
var leafBlue 0
var flowerRed 255
var flowerGreen 255
var flowerBlue 255
axiom P
rule P -> I+[P+W]--//[--L]I[++L]-[PW]++PW
rule I -> FS[//&&L][//^^L]FS
rule S -> SFS
rule L -> ['{(leafRed, leafGreen, leafBlue)+f-ff-f+|+f-ff-f}]
rule W -> [&&&D'/G////G////G////G////G]
rule D -> FF
rule G -> ['^F][{(flowerRed, flowerGreen, flowerBlue)&&&&-f+f|-f+f}]
