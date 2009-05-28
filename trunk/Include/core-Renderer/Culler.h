#pragma once

#include <windows.h>
#include "core\Array.h"


///////////////////////////////////////////////////////////////////////////////

class Camera;
class AbstractGraphicalNode;
typedef AbstractGraphicalNode* AbstractGraphicalNodeP;

///////////////////////////////////////////////////////////////////////////////

class Culler
{
public:
   virtual ~Culler() {}

   virtual void setup(Camera& camera,
                      Array<AbstractGraphicalNodeP>& renderingQueue) = 0;

   virtual void operator()(AbstractGraphicalNode& node) = 0;
};

///////////////////////////////////////////////////////////////////////////////
