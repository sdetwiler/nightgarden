The Night Garden
===

An interactive environment for exploration.



Notes and Papers
--
The Algorithmic Beauty of Plants
http://algorithmicbotany.org/papers/abop/abop.pdf


LSystem Overview
---

LSystems define a set of rules that manipulate symbols which are further interpreted to control a <a href="https://en.wikipedia.org/wiki/Turtle_graphics">turtle</a> to generate computer graphics.

Each LSystem starts with a string of symbols called the "axiom" on which rules are applied to generate a new string of symbols with every step.

Each symbol is a single character [A-Z,a-z] or one of the defined operators. The symbols either change the state of the turtle or serve as placeholders for future productions.


LSystem Syntax
===

LSystem Keywords
---
| Keyword | Interpretation | Example |
|---------|----------------|--------|
| axiom   | Declares the LSystem axiom. | `axiom F` |
| var     | Declares a global variable. | `var pi 3.1415926` |
| rule    | Declares a production rule. | `rule A -> AB` |


LSystem Symbol Commands
---

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
| \|      | Turn around. | Implemented |
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


Symbols can optionally carry parameters that change the symbol's behavior.

Symbol without parameter
```
A
```

Symbol with parameter
```
A(x, y)
```

LSystem Reserved Variables
---

These variables have special meaning when interpreting the LSystem:

| Variable | Interpretation | Default |
|----------|----------------|---------|
| n        | Default length of line generated by F and moved by f. | 5 |
| r        | F generates an 4 sided cylinder with radius r | .25 |
| delta    | Default rotation angle in degrees for rotation operators.| 25 |
| steps    | The number of explicit production steps to take when evaluating the LSystem.| 4 |
| duration | The amount of simulated time in seconds to run the LSystem.| none. If not defined, explicit `steps` is used. |
| stepInterval | The amount of simulated time in seconds to advace per step when a `duration` is defined.| 1 second. Only used if `duration` is defined. |


Examples
--

The simplest LSystem only defines the axiom that draws a line:

```
axiom F
```

A slightly more complex LSystem that defines two production rules:
```
var steps 4

axiom AA

rule A -> AB
rule B -> B
```

Evaluating this system generates the following sequence with every step:

Step 0
`AA`

Step 1
`ABAB`

Step 2
`ABBABB`

Step 3
`ABBBABBB`


LSystems symbols can optionally carry parameters which can be used to influence the generated output. The axiom defines the starting values for parameters, which are matched to the production rules.

For example, an LSystem that sets the angle used on the `+` rotation operator by passing the angle as a parameter:

```
var steps 4

axiom A(0)

rule A(d) -> F(d)+(d+10)A(d+10)
rule A -> F
```

The final production is:
```
F(0.000000)+(10.000000)F(10.000000)+(20.000000)F(20.000000)+(30.000000)A(30.000000)
```

LSystem rules can also contain conditions that must be satisfied for the rule to apply to a given symbol:

```
var steps 6

axiom A(0)

rule A(d) : d<10 -> F(d)+(d+10)A(d+10)
rule A(d) : d>=10 -> F(d)+(d+20)A(d+20)
rule A -> F

```

Variables can be declared to control values:

```
var steps 6
var theta 10
axiom A(0)

rule A(d) : d<theta -> F(d)+(d+theta)A(d+theta)
rule A(d) : d>=theta -> F(d)+(20 + theta + d)A(20 + theta + d)
rule A -> F

```

The final production is:
```
F(0.000000)+(10.000000)F(10.000000)+(40.000000)F(40.000000)+(70.000000)F(70.000000)+(100.000000)F(100.000000)+(130.000000)A(130.000000)
```






Further Examples
---


Context-free lsystem with turtle operators:

```
axom A

rule A -> [&FL!A]/////'[&FL!A]///////'[&FL!A]
rule F -> S/////F
rule S -> F L
rule L -> ['''^^{-f+f+f-|-f+f+f}]
```






Xcode Build Notes
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

