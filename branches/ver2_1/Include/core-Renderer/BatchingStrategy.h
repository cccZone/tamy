#pragma once

#include <set>
#include <d3dx9.h>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class Renderable;
class Material;

///////////////////////////////////////////////////////////////////////////////

class BatchComparator
{
public:
   bool operator()(Renderable* lhs, Renderable* rhs) const;
};

///////////////////////////////////////////////////////////////////////////////