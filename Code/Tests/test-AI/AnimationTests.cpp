#include "core-TestFramework\TestFramework.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{



} // namespace anonymous


// PAKSAS TODO: szkielet to tylko struktura node'ow - nic sie nie zmienia.
// Dodatkowo dodajemy SkinGeometry, ktore obsluguje renderowanie geometrii skory.
// Zeby zrobic taka geometrie, trzeba oddzielic vertex shader'y od pixel shader'ow - 
// vertex shader to domena geometrii, a pixel shadery sa odpalane przez efekty.
// 
// czyli:
// 1.) Stworzyc entity SkinGeometry
// 2.) odczytywanie SkinGeometry na podstawie danych z Collad'y
// 3.) rozdzielic vertex i pixel shader'y
// 4.) stworzyc domysny vertex shader, ktorego uzywaja domyslne implementacje geometrii
// 5.) stworzyc VS dla skory

// Animacje same w bibliotece AI - transformacje node'ow( spline i liniowa ),
// eventy, edytor

///////////////////////////////////////////////////////////////////////////////

TEST(Skeleton, skeletonTest)
{
}

///////////////////////////////////////////////////////////////////////////////
