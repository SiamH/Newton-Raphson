compile : g++ -o parse parseExpr.cpp parseExpr.h valueandderive.cpp valueandderive.h main.cpp
run: ./parse
help command: ./parse -h


The main difficulties of this function is to find the value of the first derivatives. I have used
forward looking approach rather than numerical / syntax check. We need to test this function
to make sure that correct derivatives are produced for a valid expression.
For simple testing, please check the help menu and you will find out how to define the expression and initial guess.

The assumption of initial guess is very important to find out the accuracy of the root and
how fast we can achieve those. We need to pass different initial guess to find the speed
of estimating root. The rules provided in the requirement has been followed here.

The expression can easily be parsed by boost library or using regex. However, I wanted
to keep it simple one pass search. We need to test this with different types of
polynomial expressions. From my experience, it would be best to create a parsing tree
if the requirement grows.
