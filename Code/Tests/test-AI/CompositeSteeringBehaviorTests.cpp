#include "core-TestFramework\TestFramework.h"
#include "core-AI\CompositeSteeringBehavior.h"
#include "core-AI\SteeringBehavior.h"
#include "core\Vector.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
 
   class SteeringBehaviorMock : public SteeringBehavior
   {
      DECLARE_ALLOCATOR( SteeringBehaviorMock, AM_ALIGNED_16 );

   private:
      Vector m_vel;

   public:
      SteeringBehaviorMock(const Vector& vel) : m_vel(vel) {}

      void calculateVelocity( float timeElapsed, Vector& outVelocity )
      {
         outVelocity = m_vel;
      }
   };

} // namespace anonymous


///////////////////////////////////////////////////////////////////////////////

TEST(CompositeSteeringBehavior, multipleBehaviors)
{
   Vector testPos;
   CompositeSteeringBehavior composite;

   testPos.set( 10, 0, 0 );
   composite.add(new SteeringBehaviorMock( testPos ), 1);
   testPos.set( 0, 10, 0 );
   composite.add(new SteeringBehaviorMock( testPos ), 1);

   Vector outVel;
   composite.calculateVelocity( 1, outVel );
   testPos.set( 10, 10, 0 );
   COMPARE_VEC( testPos, outVel );
}

///////////////////////////////////////////////////////////////////////////////

TEST(CompositeSteeringBehavior, mixingBehaviors)
{
   Vector testPos;
   CompositeSteeringBehavior composite;

   testPos.set( 10, 0, 0 );
   composite.add( new SteeringBehaviorMock( testPos ), 0.5 );
   testPos.set( 0, 10, 0 );
   composite.add( new SteeringBehaviorMock( testPos ), 1 );

   Vector outVel;
   composite.calculateVelocity( 1, outVel );
   testPos.set( 5, 10, 0 );
   COMPARE_VEC( testPos, outVel );
}

///////////////////////////////////////////////////////////////////////////////
