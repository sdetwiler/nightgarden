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

LSystem Symbol Commands
===

Implemented symbols as defined in The Algorithmic Beauty of Plants.

| Symbol | Interpretation | Status |
|--------|----------------|--------|
| F      | Move forward and draw a line. | Implemented |
| f      | Move forward without drawing a line. | Implemented |
| +      | Turn left. | Implemented |
| -      | Turn right. | Implemented |
| ^      | Pitch up. | Implemented |
| &      | Pitch down. | Implemented |
| \      | Roll left. | Implemented |
| /      | Roll right. | Implemented |
| "|"      | Turn around. | Implemented |
| $      | Rotate to vertical. | Not implemented |
| [      | Start a branch. | Implemented |
| ]      | Complete a branch. | Implemented |
| {      | Start a polygon. | Implemented |
| G      | Move forward and draw a line. Do not record a vertex. | Not implemented |
| .      | Record a vertex in the current polygon. | Implemented |
| }      | Complete a polygon. | Implemented |
| ~      | Incorporate a predefined surface. | Not implemented |
| !      | Decrement the diameter of segments. | Not implemented |
| '      | Increment the current color index. | Not implemented |
| %      | Cut off the remainter of the branch. | Not implemented |





Build Notes
===

Flex and Bison Support in Xcode requires that the project.pbxproj is edited to include

`YACC_GENERATED_FILE_STEM = InputFileStem;`

For every build target.

Find this fragment and add the indicated line:
```
        );
      SDKROOT = macosx;
      YACC_GENERATED_FILE_STEM = InputFileStem;
    };
    name = Debug;
  };  
```

If project.pbxproj is XML use this:
```
	<key>YACC_GENERATED_FILE_STEM</key>
	<string>InputFileStem</string>
```

Do the same for the Release target.

