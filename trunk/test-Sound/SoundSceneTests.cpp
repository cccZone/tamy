#include "core-TestFramework\TestFramework.h"
#include "core\Node.h"
#include <d3dx9.h>
#include "core-Sound\Sound3D.h"
#include "core-Sound\SoundSceneManager.h"


//////////////////////////////////////////////////////////////////////////////

TEST(Sound3D, positionInfluences)
{
   SoundSceneManager soundScene;

   Node dog("dog");
   Sound3D& soundSource = soundScene.addSoundSource(dog);

   D3DXMatrixTranslation(&(dog.accessLocalMtx()), 20, 5, 3);
   CPPUNIT_ASSERT_EQUAL(dog.getLocalMtx(), soundSource.getGlobalMtx());
}

//////////////////////////////////////////////////////////////////////////////
