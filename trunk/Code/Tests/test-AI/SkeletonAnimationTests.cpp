#include "core-TestFramework\TestFramework.h"
#include "core-MVC\Entity.h"
#include "core-MVC\SpatialEntity.h"
#include "core-AI\SkeletonAnimationController.h"
#include "core-AI/SkeletonAnimation.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{

   class MockNonAnimatedEntity : public Entity
   {
      DECLARE_CLASS( MockNonAnimatedEntity )
   public:
      MockNonAnimatedEntity( const std::string& id = "" ) : Entity( id ) {}

   protected:
      Entity* cloneSelf() const { return new MockNonAnimatedEntity(); }
   };
   BEGIN_OBJECT( MockNonAnimatedEntity, Entity );
   END_OBJECT();
   REGISTER_RTTI( MockNonAnimatedEntity );

} // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST( SkeletonAnimationController, architecture )
{
   // create a hierarchy of nodes
   SpatialEntity root( "root" );
   SpatialEntity* hip = new SpatialEntity( "hip" );
   SpatialEntity* leg = new SpatialEntity( "leg" );
   SpatialEntity* anotherLeg = new SpatialEntity( "leg" ); // this one won't get updated due to a duplicated name
   MockNonAnimatedEntity* notANode = new MockNonAnimatedEntity( "notANode" );
   SpatialEntity* gun = new SpatialEntity( "gun" ); // this one won't get updated, because as you'll see in a few lines - it's connected
                                                    // via a non-spatial entity
   root.add( hip );
   root.add( notANode );
   hip->add( leg );
   hip->add( anotherLeg );
   notANode->add( gun );

   // provide an animation source
   SkeletonAnimation animSource;
   animSource.addKey( "root", 0.f, D3DXQUATERNION( 0, 0, 0, 1 ), D3DXVECTOR3( 1, 0, 0 ) );
   animSource.addKey( "hip", 0.f, D3DXQUATERNION( 0, 0, 0, 1 ), D3DXVECTOR3( 1, 0, 0 ) );
   animSource.addKey( "leg", 0.f, D3DXQUATERNION( 0, 0, 0, 1 ), D3DXVECTOR3( 1, 0, 0 ) );

   // add an animation controller to the hierarchy
   SkeletonAnimationController* animController = new SkeletonAnimationController();
   animController->setAnimationSource( animSource );
   root.add( animController );

   // run the controller and verify the controller requests the animations for all the connected
   // spatial entities - but not the ones that are connected via non-spatial entities
   animController->update( 0.f );
   D3DXMATRIX result, expected;

   D3DXMatrixTranslation( &expected, 1, 0, 0 );
   result = root.getGlobalMtx();
   COMPARE_MTX( expected, result );

   D3DXMatrixTranslation( &expected, 2, 0, 0 );
   result = hip->getGlobalMtx();
   COMPARE_MTX( expected, result );

   D3DXMatrixTranslation( &expected, 3, 0, 0 );
   result = leg->getGlobalMtx();
   COMPARE_MTX( expected, result );

   D3DXMatrixTranslation( &expected, 2, 0, 0 );
   result = anotherLeg->getGlobalMtx();
   COMPARE_MTX( expected, result );

   // this one is connected via a non-node entity - so it's considered to be detached from the skeleton
   D3DXMatrixTranslation( &expected, 0, 0, 0 );
   result = gun->getGlobalMtx();
   COMPARE_MTX( expected, result );
}

///////////////////////////////////////////////////////////////////////////////

TEST( SkeletonAnimationController, trackTimeControl )
{
   // create the rig
   SpatialEntity root( "root" );
   SkeletonAnimationController* animController = new SkeletonAnimationController();
   root.add( animController );
   SkeletonAnimation animSource;
   animSource.addKey( "root", 3.f, D3DXQUATERNION( 0, 0, 0, 0 ), D3DXVECTOR3( 0, 0, 0 ) );

   animController->setAnimationSource( animSource );

   // run the animation updates
   animController->update( 1.f );
   CPPUNIT_ASSERT_EQUAL( 1.f, animController->getTrackTime() );

   animController->update( 1.5f );
   CPPUNIT_ASSERT_EQUAL( 2.5f, animController->getTrackTime() );

   // the track time depends on the longest defined animation - so the next update will wrap the time around
   animController->update( 1.0f );
   CPPUNIT_ASSERT_EQUAL( 0.5f, animController->getTrackTime() );

   // .. the same applied for updates that last longer then the animation time
   animController->update( 7.0f );
   CPPUNIT_ASSERT_EQUAL( 1.5f, animController->getTrackTime() );
}

///////////////////////////////////////////////////////////////////////////////

