#include "core-TestFramework\TestFramework.h"
#include "core-AI\GameWorld.h"
#include "core-AI\GameWorldView.h"
#include "core-AI\Entity.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class GameWorldViewMock : public GameWorldView
   {
   private:
      std::vector<std::string> m_commandsLog;

   public:
      void onEntityAdded(Entity& entity)
      {
         m_commandsLog.push_back("entity added");
      }

      void onEntityRemoved(Entity& entity)
      {
      }

      int getReceivedCommandsCount() const
      {
         return m_commandsLog.size();
      }

      const std::string& getLoggedCommand(unsigned int idx) const
      {
         return m_commandsLog.at(idx);
      }
   };

   // -------------------------------------------------------------------------

   class EntityMock : public Entity
   {
   };

} // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(GameWorld, populatingWorldWithEntities)
{
   GameWorld world;
   GameWorldViewMock observer;
   world.attachObserver(observer);

   world.addEntity(new EntityMock());

   CPPUNIT_ASSERT_EQUAL(1, observer.getReceivedCommandsCount());
   CPPUNIT_ASSERT_EQUAL(std::string("entity added"), observer.getLoggedCommand(0));
}

///////////////////////////////////////////////////////////////////////////////
