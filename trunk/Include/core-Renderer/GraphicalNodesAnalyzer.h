#pragma once

#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

template<class T>
class GraphicalNodesAnalyzer : public std::unary_function<void, AbstractGraphicalNode>
{
private:
   T& m_operation;

public:
   GraphicalNodesAnalyzer(T& operation) : m_operation(operation) {}

   void operator()(AbstractGraphicalNode* node)
   {
      m_operation(*node);
   }
};

///////////////////////////////////////////////////////////////////////////////
