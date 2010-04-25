#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\Geometry.h"
#include "core-Renderer\Renderable.h"
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

TEST(Renderable, onlyOneGeometryEntityCanBeAttachedAtATime)
{
   Renderable renderable;

   renderable.add(new Geometry());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, renderable.getEntityChildren().size());

   renderable.add(new Geometry());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, renderable.getEntityChildren().size());
}

///////////////////////////////////////////////////////////////////////////////
