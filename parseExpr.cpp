#include <iostream>
#include "stdlib.h"
#include <cmath>
#include "assert.h"
#include "parseExpr.h"

using namespace std;

// Expected variable for the expression. Anything else than 'x' will output parse failure.
#define VAR 'x'

// Parsing the values from left to right. So the vector containts most valubale
// number at start and least valuable number at the end.
int GetValueFromLists(vector<int> aiValues)
{
  int result = 0;
  int base = 1;
  while(aiValues.size() > 0)
  {
      result += aiValues.back() * base;
      aiValues.pop_back();
      base = base * 10;
  }

  return result;
}

void cNode::SetDigits(int idigit)
{
  m_aiDigits.push_back(idigit);
}

int cNode::GetDigits() const
{
  return GetValueFromLists(m_aiDigits);
}

void cNode::SetPowers(vector<int> aiPowers)
{
  m_aiPowers = aiPowers;
}

int cNode::GetPowers() const
{
  return GetValueFromLists(m_aiPowers);
}

void cNode::SetSinToken() { m_bHasSinToken = true; }
bool cNode::HasSinToken() const { return m_bHasSinToken; }

void cNode::SetCosineToken() { m_bHasCosineToken = true;}
bool cNode::HasCosineToken() const { return m_bHasCosineToken; }

void cNode::SetIfConstant() { m_bIsConstant = false; }
bool cNode::IsConstant() const { return m_bIsConstant; }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// This class parses every term in a polynomial expression separated by operators ( + / -).
// As if you built the parse tree, then the low precedence operator will be at the top level
// of the expreesion tree. It can easily be extended to parse other tokens such as log(x).
// This class definitely needs lots of test cases (polynomial fucntions) in unit test to ensure that
// it is parsing properly.

class cParser
{
private:
  vector<cNode> m_avlistOfExprs; // list of all the factors seperated by operators
  vector<bool> m_abOperators; // All the operators in the expression (+,-)

  string m_sExpr;
  bool m_bFailed; // Parse failed or not.
  // Pos of the index on the parsed expression. Here the main goal is to parse
  // and populate the list in one pass O(N).
  int m_iPos;
  int m_iExprLength;
  string m_sError; // Parsing error, helpful for the user to determine where it went wrong.

  char GetCurrChar() const
  {
    // The boundary check
    if (m_iPos < 0 ||  m_iPos >= m_iExprLength)
    {
      assert(false);
      exit(1);
    }

    return m_sExpr[m_iPos];
  }

  bool IsAllowedChar(char ch) const;
  bool IsOperator(char ch, bool& rbAdd) const;

  bool isSinToken();
  bool isCosineToken();
  bool isPowerToken(vector<int>& rvalue);

  void ParseExpression();

public:
  cParser(string s)
  {
      assert(s != "");
      m_sExpr = s;
      m_iExprLength = s.length();
      m_bFailed = false;
      m_iPos = 0;
      m_sError = "";

      ParseExpression();
  }

  string GetError() const
  {
    assert(m_bFailed);
    assert(m_sError != "");
    return m_sError;
  }

  bool GetListAndOperators(vector<cNode>& ravlistOfExprs, vector<bool>& rabOperators) const;
};

bool cParser::GetListAndOperators(vector<cNode>& ravlistOfExprs, vector<bool>& rabOperators) const
{
  assert(m_sExpr != "");
  if(m_bFailed)
    return false;

  ravlistOfExprs = m_avlistOfExprs;
  rabOperators = m_abOperators;

  return true;
}

bool cParser::isSinToken()
{
  char sintoken[3] = {'s', 'i', 'n'};

  int index  = 0;
  while (m_iPos < m_iExprLength)
  {
    if (index == 3)
      return true;

    if (GetCurrChar() != sintoken[index])
    {
      if (index > 0)
      {
        m_bFailed = (index > 0);
        m_sError = "Invalid Sin token";
      }
      return m_bFailed;
    }

    m_iPos++;
    index++;
  }

  return true;
}

bool cParser::isCosineToken()
{
  char costoken[3] = {'c', 'o', 's'};

  int index  = 0;
  while (m_iPos < m_iExprLength)
  {
    if (index == 3)
      return true;

    if (GetCurrChar() != costoken[index])
    {
      if (index > 0)
      {
        m_bFailed = (index > 0);
        m_sError = "Invalid Cosine token";
      }
      return m_bFailed;
    }

    m_iPos++;
    index++;
  }

  return true;
}

bool cParser::isPowerToken(vector<int>& rvalue)
{
  if (GetCurrChar() == '^')
  {
    m_iPos++;
    while(m_iPos < m_iExprLength && isdigit(GetCurrChar()))
    {
      int curr = GetCurrChar() - '0';
      rvalue.push_back(curr);
      m_iPos++;
    }

    if (rvalue.size() == 0)
    {
      m_bFailed = true;
      m_sError = "Invalid power token";
    }

    return true;
  }

  return false;
}

bool cParser::IsAllowedChar(char ch) const
{
  return (ch == '(' || ch == ')' || ch == '*' || ch == '/');
}

bool cParser::IsOperator(char ch, bool& rbAdd) const
{
  rbAdd = false;
  if (ch == '+')
  {
    rbAdd = true;
    return true;
  }

  return (ch == '-');
}

// Populating lists from the expression.
void cParser::ParseExpression()
{
  assert(m_iPos == 0);

  while (m_iPos < m_iExprLength)
  {
    cNode vCurr;
    while (true)
    {
      if (m_iPos == m_iExprLength)
        break;

      char ch = GetCurrChar();

      if (ch == ' ')
      {
        m_iPos++;
        continue;
      }

      vector<int> powValues;

      bool bAdd = false;
      if (IsOperator(ch, bAdd))
      {
        m_abOperators.push_back(bAdd);
        break;
      }

      if (isSinToken())
        vCurr.SetSinToken();
      else if (isCosineToken())
        vCurr.SetCosineToken();
      else if (isdigit(ch))
      {
        if (!vCurr.IsConstant())
        {
          m_bFailed = true;
          return;
        }

        vCurr.SetDigits((int)ch);
        m_iPos++;
      }
      else if (isPowerToken(powValues))
        vCurr.SetPowers(powValues);
      else if (ch == VAR)
      {
          vCurr.SetIfConstant();
          m_iPos++;
      }
      else if (IsAllowedChar(ch))
        m_iPos++;
      else
        m_bFailed = true;

      // Making the error message as much helpful as possbile.
      if (m_bFailed)
      {
        m_sError = m_sError + " at: '" + GetCurrChar() + "'";
        return;
      }
    }

    m_avlistOfExprs.push_back(vCurr);
    m_iPos++;
  }
}

bool ParseExpression(string sExpr, vector<cNode>& ravlistOfExprs,
                    vector<bool>& rabOperators, string& rsError)
{
  cParser vParser(sExpr);
  if (!vParser.GetListAndOperators(ravlistOfExprs, rabOperators))
  {
      rsError = vParser.GetError();
      return false;
  }

  return true;
}
