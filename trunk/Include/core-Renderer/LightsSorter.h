#pragma once

/// @file   core-Renderer\LightsSorter.h
/// @brief  an algorithm for sorting lights in terms of their influence

#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class Light;

///////////////////////////////////////////////////////////////////////////////

/**
 * Light sorter will sort lights based on their influence
 * on a given point in space. The algorithm is designed
 * for dynamical lights, as it recalculates the influence
 * for each light.
 *
 * It's a helper class to RenderLights class, and is also a functor
 * that can be used with STL sorting algorithms.
 */
class LightsSorter
{
private:
   D3DXVECTOR3 m_influencedPos;

public:
   LightsSorter();

   /**
    * Set the position in reference to which the calcultions should
    * be carried out.
    *
    * @param pos     light influence sampling position
    */
   void setInfluencedPos(const D3DXVECTOR3& pos);

   /**
    * A functor comparison operation that will compare
    * two lights in terms of influence over a particular point
    * in space and indicate the more influential one
    *
    * @param lhs        first light
    * @param rhs        second light
    * @return           'true' indicates the first light has 
    *                   stronger influence
    */
   bool operator()(Light* lhs, Light* rhs) const;

private:
   float getLightContribution(Light& light) const;
};

///////////////////////////////////////////////////////////////////////////////
