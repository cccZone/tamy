#pragma once


///////////////////////////////////////////////////////////////////////////////

/**
 * It's a visitor interface that allows to penetrate the nodes hierearchy
 */
class NodeVisitor
{
public:
   virtual ~NodeVisitor() {}
};

///////////////////////////////////////////////////////////////////////////////

/**
 * The macro can be used to register multiple visitors
 * within an onAccept method of a Node derrived class,
 */
#define REGISTER_NODE_VISITOR(VisitorClass)                                  \
{                                                                            \
   VisitorClass* v = dynamic_cast<VisitorClass*> (&visitor);                 \
   if (v != NULL)                                                            \
   {                                                                         \
      v->visit(*this);                                                       \
   }                                                                         \
}

///////////////////////////////////////////////////////////////////////////////