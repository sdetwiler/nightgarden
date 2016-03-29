# Figure 1.26 A Plant

# Explicit step configuration
# var steps 300

# Time based step configuration
var stepInterval 0.016666
var duration 5

var n 10
var delta 18

axiom P
rule P -> I+[P+W]--//[--L]I[++L]-[PW]++PW
rule I -> FS[//&&L][//^^L]FS
rule S -> SFS
rule L -> X
rule W -> [&&&D'/G////G////G////G////G]
rule D -> FF
rule G -> ['^F]
