#include <iostream>
#include <string>
#include "stdlib.h"
#include "valueandderive.h"

using namespace std;

double FindRootwithNewtonRapsonMethod(double dValue)
{
  double dFuncValue = 0.0; // Value of function --> F(X)
  double dDerivative = 0.0; // Value of first derivative ---> F'(X)
  GetValueAndFirstDerivative(dValue, dFuncValue, dDerivative);
  // According to the newton-rapson method, if F(X) == F'(X) then we assume that root is found.
  if (dValue == dFuncValue)
    return dValue;
  // From the requirement, we try to assume the root until F(X) becomes 0.
  if (dFuncValue == 0.0)
    return dValue;

  // The calculation: X + 1 = X - F(X) /F'(X).
  dValue = dValue - (dFuncValue / dDerivative);
  return FindRootwithNewtonRapsonMethod(dValue);
}

void helpmenu()
{
  cout << "Newton Rapton Method program needs two arguments, an expression and an initial guess." << endl;
  cout << "The expression is expected to be in this format:\n";
  cout << "power of the variable must be expressed as x^pow --> x^3\n";
  cout << "Any polynomial factor should be prefixed of that variable: numberx -->3x not xnumber -->x3\n";
  cout << "Only Sin(x) and Cos(x) functions are supported at this moment.\n";
  cout << "One proper examples is '3x^3 + x^2sin(x) + 1' 0.123";
}

int main(int argc, char** argv)
{
  if (argc == 1)
  {
    cout << "Use -h for help";
    exit(1);
  }
  if (argc == 2)
  {
    string sOption = argv[1];
    if (sOption == "-h")
      helpmenu();
    else
      cout << "Invalid command line options";

    exit(1);
  }
  if (argc == 3)
  {
    string sExpr = argv[1];
    string sValue = argv[2];
    double dValue = atof(sValue.c_str());
    string sError = "";
    if (!SetExpression(sExpr, sError))
    {
      cout << "Failed parse the expression: " + sError << endl;
      return 0;
    }

    double droot =  FindRootwithNewtonRapsonMethod(dValue);
    cout << "Root of the expression: " << droot << endl;
  }

  return 0;
}
