#include <vector>
#include <string>

using namespace std;

// This class represent every term in a polynomial expression.
// Sunh 3x^3 + x^2sin(x) + 1 will produce three instances of this
// class (1--> 3x^3) (2---> x^2sin(x)) (3--> 1).
// The main responsibility of this class is to remember
// the multiply factor, power value, sin and cos value.

class cNode
{
public:
  cNode()
  {
    m_bHasSinToken = false; // The expreesion contains sin(x)
    m_bHasCosineToken = false; // The expreesion contains cos(x)
    m_bIsConstant = true; // The expression is constant (x is absent)
  }

  void SetDigits(int idigit);
  int GetDigits() const;

  // Get the power of x. x^23 --> 23
  void SetPowers(vector<int> aiPowers);
  int GetPowers() const;

  void SetSinToken();
  bool HasSinToken() const;

  void SetCosineToken();
  bool HasCosineToken() const;

  void SetIfConstant();
  bool IsConstant() const;

private:
  vector<int> m_aiDigits;
  vector<int> m_aiPowers;
  bool m_bHasSinToken;
  bool m_bHasCosineToken;
  bool m_bIsConstant;
};


bool ParseExpression(string sExpr, vector<cNode>& ravlistOfExprs,
                     vector<bool>& rabOperators, string& rsError);
