#include "core-TestFramework\TestFramework.h"
#include "core-AI\CompositeSteeringBehavior.h"
#include "core-AI\SteeringBehavior.h"
#include "core\Vector.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
 
   class SteeringBehaviorMock : public SteeringBehavior
   {
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
   CompositeSteeringBehavior composite;
   composite.add(new SteeringBehaviorMock(Vector(10, 0, 0)), 1);
   composite.add(new SteeringBehaviorMock(Vector(0, 10, 0)), 1);

   Vector outVel;
   composite.calculateVelocity( 1, outVel );
   COMPARE_VEC( Vector(10, 10, 0), outVel );
}

///////////////////////////////////////////////////////////////////////////////

TEST(CompositeSteeringBehavior, mixingBehaviors)
{
   CompositeSteeringBehavior composite;
   composite.add(new SteeringBehaviorMock(Vector(10, 0, 0)), 0.5);
   composite.add(new SteeringBehaviorMock(Vector(0, 10, 0)), 1);

   Vector outVel;
   composite.calculateVelocity( 1, outVel );
   COMPARE_VEC( Vector(5, 10, 0), outVel );
}

///////////////////////////////////////////////////////////////////////////////
