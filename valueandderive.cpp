#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "stdlib.h"
#include "assert.h"

#include "parseExpr.h"
#include "valueandderive.h"

using namespace std;

static int fg_count = 0;

// The cache memorizes the factors and operators (See comments in parseExpr.h)
// Just making sure that this will be set once per expression.
class cCache
{
private:
  vector<cNode> m_avLists;
  vector<bool> m_abOps;

public:
  vector<cNode> GetList() const
  {
    return m_avLists;
  }

  vector<bool> GetOps() const
  {
    return m_abOps;
  }

  void set(vector<cNode> avLists, vector<bool> abOps)
  {
    assert(fg_count == 0);
    m_avLists = avLists;
    m_abOps = abOps;
    fg_count++;
  }
}fg_Cache;

// Calcuating value and first derivaties of a expression.
class cCalculateValueAndDerivaties
{
private:
  double m_dValue; // Value of the expression f(x)
  double m_dFirstDerive; // First derivatie of the expression f'(x)

public:
  cCalculateValueAndDerivaties(double dVal, double dFirstDeriv = 0)
  {
    m_dValue = dVal;
    m_dFirstDerive = dFirstDeriv;
  }

  void SetValue(double dVal, double dFirstDeriv)
  {
    m_dValue = dVal;
    m_dFirstDerive = dFirstDeriv;
  }

  void SetFirstExpr(vector<cNode> avlists, vector<bool> avOps);

  double GetValue() const
  {
    return m_dValue;
  }

  double GetDerivative() const
  {
    return m_dFirstDerive;
  }

  // According to spec, Sin and Cos tokens are expected in expression.
  const cCalculateValueAndDerivaties& ProcessSpecialTokens(double dValue, double dDerive);

  const cCalculateValueAndDerivaties& operator+=(cCalculateValueAndDerivaties const& other);
  const cCalculateValueAndDerivaties& operator-=(cCalculateValueAndDerivaties const& other);
  const cCalculateValueAndDerivaties& operator*=(cCalculateValueAndDerivaties const& other);


  friend const cCalculateValueAndDerivaties operator+(cCalculateValueAndDerivaties const& a,
                                                      cCalculateValueAndDerivaties const& b)
  {
    cCalculateValueAndDerivaties r(a);
    r+=b;
    return r;
  }

  friend const cCalculateValueAndDerivaties operator-(cCalculateValueAndDerivaties const& a,
                                                      cCalculateValueAndDerivaties const& b)
  {
    cCalculateValueAndDerivaties r(a);
    r-=b;
    return r;
  }

  friend const cCalculateValueAndDerivaties operator*(cCalculateValueAndDerivaties const& a,
                                                      cCalculateValueAndDerivaties const& b)
  {
    cCalculateValueAndDerivaties r(a);
    r*=b;
    return r;
  }
};

const cCalculateValueAndDerivaties& cCalculateValueAndDerivaties::
                            ProcessSpecialTokens(double dValue, double dDerive)
{
  m_dFirstDerive = m_dFirstDerive * dDerive;
  m_dValue = dValue;
}

const cCalculateValueAndDerivaties& cCalculateValueAndDerivaties::
                                    operator+=(cCalculateValueAndDerivaties const& other)
{
  m_dValue += other.m_dValue;
  m_dFirstDerive += other.m_dFirstDerive;
  return *this;
}

const cCalculateValueAndDerivaties& cCalculateValueAndDerivaties::
                                    operator-=(cCalculateValueAndDerivaties const& other)
{
  m_dValue -= other.m_dValue;
  m_dFirstDerive -= other.m_dFirstDerive;
  return *this;
}

const cCalculateValueAndDerivaties& cCalculateValueAndDerivaties::
                                    operator*=(cCalculateValueAndDerivaties const& other)
{
  m_dFirstDerive = m_dFirstDerive * other.m_dValue + m_dValue * other.m_dFirstDerive;
  m_dValue *= other.m_dValue;
  return *this;
}

cCalculateValueAndDerivaties sin(cCalculateValueAndDerivaties t)
{
  using std::sin;
  using std::cos;
  double v = t.GetValue();
  t.ProcessSpecialTokens(sin(v),cos(v));
  return t;
}

cCalculateValueAndDerivaties cos(cCalculateValueAndDerivaties t)
{
  using std::sin;
  using std::cos;
  double v = t.GetValue();
  t.ProcessSpecialTokens(cos(v), -sin(v));
  return t;
}

cCalculateValueAndDerivaties GetValAndDeriv(cCalculateValueAndDerivaties x)
{
  // Buidling up each factor (3x^2) (x^2sin(x)) (1)
  vector<cCalculateValueAndDerivaties> lists;
  for (int i = 0; i < fg_Cache.GetList().size(); i++)
  {
    cNode vCurr = fg_Cache.GetList()[i];
    cCalculateValueAndDerivaties res(0);

    if (vCurr.IsConstant())
      res.SetValue(vCurr.GetDigits(), 0);
    else
    {
      res = x;
      for (int j = 0; j < vCurr.GetPowers() - 1; j++)
        res = res * x;

      if (vCurr.GetDigits() > 0)
        res = res * vCurr.GetDigits();

      if (vCurr.HasSinToken())
        res = res * sin(x);
      if (vCurr.HasCosineToken())
          res = res * cos(x);

      lists.push_back(res);
    }
  }

  // summation or subtraction of these factors.
  cCalculateValueAndDerivaties res = lists[0];
  for (int i = 0; i < fg_Cache.GetOps().size(); i++)
  {
    if (fg_Cache.GetOps()[i])
      res += lists[i];
    else
      res -= lists[i];
  }

  return res;
}

bool SetExpression(string sExpr, string& rsError)
{
  rsError = "";
  assert(sExpr != "");

  vector<bool> abOperators;
  vector<cNode> avListExprs;

  if (!ParseExpression(sExpr, avListExprs, abOperators, rsError))
    return false;

  if(avListExprs.size() - 1 != abOperators.size())
  {
    assert(false);
    exit(1);
  }

  fg_Cache.set(avListExprs, abOperators);
  return true;
}

void GetValueAndFirstDerivative(double dStartValue, double& rdValue, double& rdDerivatives)
{
  cCalculateValueAndDerivaties x (dStartValue, 1);

  cCalculateValueAndDerivaties y = GetValAndDeriv(x);
  rdValue = y.GetValue();
  rdDerivatives = y.GetDerivative();
}
