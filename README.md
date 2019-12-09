<b>Description:</b> This is a project for CS203 computer organization. It is an implementation of a lisp-like arithmetic language that evaluates expressions.

The program can:

-Create variables
-Create functions
-Print the values of functions/variables
-Evaluate those functions/variables
-Prints subsets of expressions
-Append to expressions

<h4>Example Evaluations</h4>

<b>Evaluation:</b>
(+ 1 2 3) will evaluate to 6
(- 1 2 3) will evaluate to -4
(\* 1 2 3) will evaluate to 6

<b>Subsets:</b>
r = move to the next term
f = get the value of the current term


(f(r(r(+ 1 2 3)))) will evaluate to 2
(f(r(r(+ 1 (+ 1 2))))) will evaluate to 3
(f(+ 1 2)) will evaluate to +



