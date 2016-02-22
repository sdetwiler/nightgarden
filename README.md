The Night Garden
===

An interactive environment for exploration.



Notes and Papers
--
The Algorithmic Beauty of Plants
http://algorithmicbotany.org/papers/abop/abop.pdf


Examples
--


Simple context-free lsystem:

	axiom AA

	rule A -> AB
	rule B -> B


Context-free lsystem with turtle operators:

	axom A

	rule A -> [&FL!A]/////'[&FL!A]///////'[&FL!A]
	rule F -> S/////F
	rule S -> F L
	rule L -> ['''^^{-f+f+f-|-f+f+f}]


Context-free parametric lsystem:

	axiom a(1,2)b(3)

	rule a(x,y) -> ab(x+1,y)
	rule b(z) -> b(z)


Context-aware parametric lsystem with conditions:

	var n 3
	axiom b(1)aaaaaa

	rule b(x) < a : x<n -> b(x+1)
	rule b(x) < a : x>=n -> b(x)
	rule b -> a



