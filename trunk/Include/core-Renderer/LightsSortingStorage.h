#ifndef _LIGHTS_SORTING_STRATEGY_H
#define _LIGHTS_SORTING_STRATEGY_H

/// @file   core-Renderer\LightsSortingStorage.h
/// @brief  light storage decorator that returns the qurey results 
///         sorted in descending order in terms of light influence

#include "core-Renderer\LightsStorage.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This storage sorts the lights that go through it using the specified sorting
 * algorithm.
 *
 * There's no reason to switch algorithms in runtime, and we kinda
 * opt for performance here since this code's gonna be hit very often 
 * in time critical places, that's why we're passing the sorter as
 * a template param instead of using a polimorphic strategy.
 *
 * Algorithm should have two methods accessible:
 *
 *    void setInfluencedPos(const D3DXVECTOR3& pos) - this will allow
 *          to set an arbitrary point with respect to which the calculations
 *          should be performed
 *
 *    bool operator()(Light* lhs, Light* rhs) const - the method should
 *          compare the two lights in terms of how do they influence the point
 *          specified using the previously dfescribed method, and return
 *          'true' if the one passed in 'lhs' param has greater influence
 *          than the one in the 'rhs' param.
 */
template <typename Sorter>
class LightsSortingStorage : public LightsStorage
{
private:
   LightsStorage* m_storage;
   Sorter* m_sorter;

public:
   /**
    * Constructor.
    *
    * @param storage    storage we want to wrap to have its results sorted
    * @param sorter     we can specify an existing instance of a sorter
    *                   by passing it here. If we don't pass anything,
    *                   a default instance based on the template type
    *                   will be created using a standard non-parametrized
    *                   constructor.
    */
   LightsSortingStorage(LightsStorage* storage, Sorter* sorter = NULL);
   ~LightsSortingStorage();

   void query(const D3DXVECTOR3& point, Array<Light*>& outLights);
};

///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer\LightsSortingStorage.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _LIGHTS_SORTING_STRATEGY_H
