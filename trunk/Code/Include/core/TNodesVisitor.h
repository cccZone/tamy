#pragma once


///////////////////////////////////////////////////////////////////////////////

/**
 * An implementor of this interface is guaranteed to have a method
 * allowing it to visit a particular type of node
 */
template<typename NodeType>
class TNodesVisitor
{
public:
   virtual ~TNodesVisitor() {}

   virtual void visit(NodeType& node) = 0;
};

///////////////////////////////////////////////////////////////////////////////
