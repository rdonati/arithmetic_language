<b>Description:</b> This is a project for CS203 computer organization. It is an implementation of a lisp-like arithmetic language that evaluates expressions.

The program can:

-Create variables<br>
-Create functions<br>
-Print the values of functions/variables<br>
-Evaluate those functions/variables<br>
-Prints subsets of expressions<br>
-Append to expressions<br>

<h4>Example Evaluations</h4>

<b>Evaluation:</b><br>
(+ 1 2 3) will evaluate to 6<br>
(- 1 2 3) will evaluate to -4<br>
(\* 1 2 3) will evaluate to 6<br>

<b>Subsets:</b><br>
r = move to the next term<br>
f = get the value of the current term<br>


(f(r(r(+ 1 2 3)))) will evaluate to 2<br>
(f(r(r(+ 1 (+ 1 2))))) will evaluate to 3<br>
(f(+ 1 2)) will evaluate to +<br>



