#pragma once

#include <windows.h>
#include "core\Array.h"


///////////////////////////////////////////////////////////////////////////////

struct Frustum;
class AbstractGraphicalNode;
typedef AbstractGraphicalNode* AbstractGraphicalNodeP;

///////////////////////////////////////////////////////////////////////////////

class Culler
{
public:
   virtual ~Culler() {}

   virtual void setup(Frustum& frustum,
                      Array<AbstractGraphicalNodeP>& renderingQueue) = 0;

   virtual void operator()(AbstractGraphicalNode& node) = 0;
};

///////////////////////////////////////////////////////////////////////////////
