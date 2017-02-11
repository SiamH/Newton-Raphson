#include <iostream>
#include <string>

using namespace std;

// Step 1: Set the expression and build up the cache
bool SetExpression(string sExpr, string& rsError);

// Using the cache, get values and derivatives.
void GetValueAndFirstDerivative(double dStartValue, double& rdValue, double& rdDerivatives);
