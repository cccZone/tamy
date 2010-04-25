#include "core-TestFramework\TestFramework.h"
#include "core-AI\CompositeSteeringBehavior.h"
#include "core-AI\SteeringBehavior.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
 
   class SteeringBehaviorMock : public SteeringBehavior
   {
   private:
      D3DXVECTOR3 m_vel;

   public:
      SteeringBehaviorMock(const D3DXVECTOR3& vel) : m_vel(vel) {}

      D3DXVECTOR3 calculateVelocity(float timeElapsed)
      {
         return m_vel;
      }
   };

} // namespace anonymous


///////////////////////////////////////////////////////////////////////////////

TEST(CompositeSteeringBehavior, multipleBehaviors)
{
   CompositeSteeringBehavior composite;
   composite.add(new SteeringBehaviorMock(D3DXVECTOR3(10, 0, 0)), 1);
   composite.add(new SteeringBehaviorMock(D3DXVECTOR3(0, 10, 0)), 1);

   COMPARE_VEC(D3DXVECTOR3(10, 10, 0), composite.calculateVelocity(1));
}

///////////////////////////////////////////////////////////////////////////////

TEST(CompositeSteeringBehavior, mixingBehaviors)
{
   CompositeSteeringBehavior composite;
   composite.add(new SteeringBehaviorMock(D3DXVECTOR3(10, 0, 0)), 0.5);
   composite.add(new SteeringBehaviorMock(D3DXVECTOR3(0, 10, 0)), 1);

   COMPARE_VEC(D3DXVECTOR3(5, 10, 0), composite.calculateVelocity(1));
}

///////////////////////////////////////////////////////////////////////////////
