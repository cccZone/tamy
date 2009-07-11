#pragma once

#include "core-Renderer\CoordinatesOperation.h"


///////////////////////////////////////////////////////////////////////////////

class CoordinatesOperationMock : public CoordinatesOperation
{
public:
   CoordinatesOperationMock(CoordsOpCode opCode) 
         : CoordinatesOperation(opCode)
   {}

   CoordinatesOperation* clone() {return new CoordinatesOperationMock(*this);}

   void setForRendering(unsigned char stageIdx) {}
};

///////////////////////////////////////////////////////////////////////////////
