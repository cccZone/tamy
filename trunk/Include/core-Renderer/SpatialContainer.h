#pragma once

#include "core\Array.h"


///////////////////////////////////////////////////////////////////////////////

class AbstractGraphicalNode;
typedef AbstractGraphicalNode* AbstractGraphicalNodeP;
struct Frustum;

///////////////////////////////////////////////////////////////////////////////

class SpatialContainer
{
public:
   virtual ~SpatialContainer() {}

   virtual void insert(const AbstractGraphicalNodeP& elem) = 0;

   virtual void remove(const AbstractGraphicalNodeP& elem) = 0;

   virtual void query(const Frustum& boundingVol, 
                      Array<AbstractGraphicalNodeP>& output) const = 0;
};

///////////////////////////////////////////////////////////////////////////////
