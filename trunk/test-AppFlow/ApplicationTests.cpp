#include "core-TestFramework\TestFramework.h"
#include "ApplicationManagerMock.h"
#include "ApplicationMock.h"
#include "core-AppFlow\Application.h"
#include <windows.h>


///////////////////////////////////////////////////////////////////////////////

TEST(ApplicationManager, noEntryApplicationDefined)
{
   ApplicationManagerMock manager;
   ApplicationMock app("app");

   manager.addApplication(app);

   CPPUNIT_ASSERT_EQUAL(false, app.isInitialized());

   CPPUNIT_ASSERT_EQUAL(false, manager.step());
   CPPUNIT_ASSERT_EQUAL(false, app.isInitialized());

};

///////////////////////////////////////////////////////////////////////////////

TEST(ApplicationManager, runningSingleApplication)
{
   ApplicationManagerMock manager;
   ApplicationMock app("app");

   manager.addApplication(app);
   manager.setEntryApplication("app");

   CPPUNIT_ASSERT_EQUAL(false, app.isInitialized());

   CPPUNIT_ASSERT_EQUAL(true, manager.step());
   CPPUNIT_ASSERT_EQUAL(true, app.isInitialized());

   app.finish();
   CPPUNIT_ASSERT_EQUAL(true, app.isInitialized());

   CPPUNIT_ASSERT_EQUAL(false, manager.step());
   CPPUNIT_ASSERT_EQUAL(false, app.isInitialized());
};

///////////////////////////////////////////////////////////////////////////////

TEST(ApplicationManager, appReceivesUpdated)
{
   ApplicationManagerMock manager;
   ApplicationMock app("app");

   manager.addApplication(app);
   manager.setEntryApplication("app");

   CPPUNIT_ASSERT_EQUAL(0.f, app.getTimeElapsed());

   manager.step(); // initialization step
   CPPUNIT_ASSERT_EQUAL(0.f, app.getTimeElapsed());

   manager.step();
   CPPUNIT_ASSERT_EQUAL(1.f, app.getTimeElapsed());

   app.finish();

   manager.step();
   CPPUNIT_ASSERT_EQUAL(0.f, app.getTimeElapsed());
};

///////////////////////////////////////////////////////////////////////////////

TEST(ApplicationManager, connectingTwoApplications)
{
   ApplicationManagerMock manager;
   ApplicationMock menu("menu");
   ApplicationMock game("game");

   manager.addApplication(menu);
   manager.addApplication(game);

   manager.setEntryApplication("menu");
   manager.connect("menu", "game", 0);
   manager.connect("game", "menu", Application::ON_EXIT);

   // no app is initialized yet
   CPPUNIT_ASSERT_EQUAL(false, menu.isInitialized());
   CPPUNIT_ASSERT_EQUAL(false, game.isInitialized());

   // first step and menu gets initialized
   CPPUNIT_ASSERT_EQUAL(true, manager.step());
   CPPUNIT_ASSERT_EQUAL(true, menu.isInitialized());
   CPPUNIT_ASSERT_EQUAL(0.f, menu.getTimeElapsed());
   CPPUNIT_ASSERT_EQUAL(false, game.isInitialized());
   CPPUNIT_ASSERT_EQUAL(0.f, game.getTimeElapsed());

   // menu starts getting updated, game still uninitialized
   CPPUNIT_ASSERT_EQUAL(true, manager.step());
   CPPUNIT_ASSERT_EQUAL(true, menu.isInitialized());
   CPPUNIT_ASSERT_EQUAL(1.f, menu.getTimeElapsed());
   CPPUNIT_ASSERT_EQUAL(false, game.isInitialized());
   CPPUNIT_ASSERT_EQUAL(0.f, game.getTimeElapsed());

   // switching over to game:
   //   - menu doesn't get deinitialized, but it stops receivig updates (enters hibernation)
   //   - game gets initialized
   menu.sendSignal(0);
   CPPUNIT_ASSERT_EQUAL(true, manager.step());
   CPPUNIT_ASSERT_EQUAL(true, menu.isInitialized());
   CPPUNIT_ASSERT_EQUAL(0.f, menu.getTimeElapsed());
   CPPUNIT_ASSERT_EQUAL(true, game.isInitialized());
   CPPUNIT_ASSERT_EQUAL(0.f, game.getTimeElapsed());

   // ...playing the game...
   CPPUNIT_ASSERT_EQUAL(true, manager.step());
   CPPUNIT_ASSERT_EQUAL(true, menu.isInitialized());
   CPPUNIT_ASSERT_EQUAL(0.f, menu.getTimeElapsed());
   CPPUNIT_ASSERT_EQUAL(true, game.isInitialized());
   CPPUNIT_ASSERT_EQUAL(1.f, game.getTimeElapsed());

   // finished the game - game gets deinitialized...
   game.finish();
   CPPUNIT_ASSERT_EQUAL(true, manager.step());
   CPPUNIT_ASSERT_EQUAL(true, menu.isInitialized());
   CPPUNIT_ASSERT_EQUAL(0.f, menu.getTimeElapsed());
   CPPUNIT_ASSERT_EQUAL(false, game.isInitialized());
   CPPUNIT_ASSERT_EQUAL(0.f, game.getTimeElapsed());

   // .. and menu gains focus with the next update
   CPPUNIT_ASSERT_EQUAL(true, manager.step());
   CPPUNIT_ASSERT_EQUAL(true, menu.isInitialized());
   CPPUNIT_ASSERT_EQUAL(1.f, menu.getTimeElapsed());
   CPPUNIT_ASSERT_EQUAL(false, game.isInitialized());

   // close the menu
   menu.finish();
   CPPUNIT_ASSERT_EQUAL(false, manager.step());
   CPPUNIT_ASSERT_EQUAL(false, menu.isInitialized());
   CPPUNIT_ASSERT_EQUAL(0.f, menu.getTimeElapsed());
   CPPUNIT_ASSERT_EQUAL(false, game.isInitialized());
};

///////////////////////////////////////////////////////////////////////////////

TEST(ApplicationManager, reinitializingFinishedApp)
{
   ApplicationManagerMock manager;
   ApplicationMock menu("menu");
   ApplicationMock game("game");

   manager.addApplication(menu);
   manager.addApplication(game);

   manager.setEntryApplication("menu");
   manager.connect("menu", "game", 0);
   manager.connect("game", "menu", Application::ON_EXIT);

   // no app is initialized yet
   CPPUNIT_ASSERT_EQUAL(false, menu.isInitialized());
   CPPUNIT_ASSERT_EQUAL(false, game.isInitialized());

   // first step and menu gets initialized
   CPPUNIT_ASSERT_EQUAL(true, manager.step());
   CPPUNIT_ASSERT_EQUAL(true, menu.isInitialized());
   CPPUNIT_ASSERT_EQUAL(0.f, menu.getTimeElapsed());
   CPPUNIT_ASSERT_EQUAL(false, game.isInitialized());
   CPPUNIT_ASSERT_EQUAL(0.f, game.getTimeElapsed());

   // menu starts getting updated, game still uninitialized
   CPPUNIT_ASSERT_EQUAL(true, manager.step());
   CPPUNIT_ASSERT_EQUAL(true, menu.isInitialized());
   CPPUNIT_ASSERT_EQUAL(1.f, menu.getTimeElapsed());
   CPPUNIT_ASSERT_EQUAL(false, game.isInitialized());
   CPPUNIT_ASSERT_EQUAL(0.f, game.getTimeElapsed());

   // switching over to game:
   //   - menu doesn't get deinitialized, but it stops receivig updates (enters hibernation)
   //   - game gets initialized
   menu.sendSignal(0);
   CPPUNIT_ASSERT_EQUAL(true, manager.step());
   CPPUNIT_ASSERT_EQUAL(true, menu.isInitialized());
   CPPUNIT_ASSERT_EQUAL(0.f, menu.getTimeElapsed());
   CPPUNIT_ASSERT_EQUAL(true, game.isInitialized());
   CPPUNIT_ASSERT_EQUAL(0.f, game.getTimeElapsed());

   // ...playing the game...
   CPPUNIT_ASSERT_EQUAL(true, manager.step());
   CPPUNIT_ASSERT_EQUAL(true, menu.isInitialized());
   CPPUNIT_ASSERT_EQUAL(0.f, menu.getTimeElapsed());
   CPPUNIT_ASSERT_EQUAL(true, game.isInitialized());
   CPPUNIT_ASSERT_EQUAL(1.f, game.getTimeElapsed());

   // finished the game - game gets deinitialized...
   game.finish();
   CPPUNIT_ASSERT_EQUAL(true, manager.step());
   CPPUNIT_ASSERT_EQUAL(true, menu.isInitialized());
   CPPUNIT_ASSERT_EQUAL(0.f, menu.getTimeElapsed());
   CPPUNIT_ASSERT_EQUAL(false, game.isInitialized());
   CPPUNIT_ASSERT_EQUAL(0.f, game.getTimeElapsed());

   // .. and menu gains focus with the next update
   CPPUNIT_ASSERT_EQUAL(true, manager.step());
   CPPUNIT_ASSERT_EQUAL(true, menu.isInitialized());
   CPPUNIT_ASSERT_EQUAL(1.f, menu.getTimeElapsed());
   CPPUNIT_ASSERT_EQUAL(false, game.isInitialized());


   // switching over to game again...
   menu.sendSignal(0);
   CPPUNIT_ASSERT_EQUAL(true, manager.step());
   CPPUNIT_ASSERT_EQUAL(true, menu.isInitialized());
   CPPUNIT_ASSERT_EQUAL(0.f, menu.getTimeElapsed());
   CPPUNIT_ASSERT_EQUAL(true, game.isInitialized());
   CPPUNIT_ASSERT_EQUAL(0.f, game.getTimeElapsed());

   // ...playing the game...
   CPPUNIT_ASSERT_EQUAL(true, manager.step());
   CPPUNIT_ASSERT_EQUAL(true, menu.isInitialized());
   CPPUNIT_ASSERT_EQUAL(0.f, menu.getTimeElapsed());
   CPPUNIT_ASSERT_EQUAL(true, game.isInitialized());
   CPPUNIT_ASSERT_EQUAL(1.f, game.getTimeElapsed());

   // ... and finishing the game once more
   game.finish();
   CPPUNIT_ASSERT_EQUAL(true, manager.step());
   CPPUNIT_ASSERT_EQUAL(true, menu.isInitialized());
   CPPUNIT_ASSERT_EQUAL(0.f, menu.getTimeElapsed());
   CPPUNIT_ASSERT_EQUAL(false, game.isInitialized());
   CPPUNIT_ASSERT_EQUAL(0.f, game.getTimeElapsed());

   // close the menu
   menu.finish();
   CPPUNIT_ASSERT_EQUAL(false, manager.step());
   CPPUNIT_ASSERT_EQUAL(false, menu.isInitialized());
   CPPUNIT_ASSERT_EQUAL(0.f, menu.getTimeElapsed());
   CPPUNIT_ASSERT_EQUAL(false, game.isInitialized());
}

///////////////////////////////////////////////////////////////////////////////

TEST(ApplicationManager, userInput)
{
   ApplicationManagerMock manager;
   ApplicationMock app("app");
   manager.addApplication(app);
   manager.setEntryApplication("app");

   manager.keyDown(VK_UP);
   manager.step();
   CPPUNIT_ASSERT_EQUAL(true, app.isKeyPressed(VK_UP));
   CPPUNIT_ASSERT_EQUAL(false, app.isKeyPressed(VK_DOWN));
   CPPUNIT_ASSERT_EQUAL(false, app.isKeyPressed(VK_LEFT));

   manager.keyDown(VK_LEFT);
   manager.keyDown(VK_DOWN);
   manager.step();
   CPPUNIT_ASSERT_EQUAL(false, app.isKeyPressed(VK_UP));
   CPPUNIT_ASSERT_EQUAL(true, app.isKeyPressed(VK_DOWN));
   CPPUNIT_ASSERT_EQUAL(true, app.isKeyPressed(VK_LEFT));

   manager.mouseMove(Point(500, 600));
   manager.step();
   CPPUNIT_ASSERT_EQUAL(Point(500, 600), app.getMousePos());

   manager.mouseMove(Point(505, 610));
   manager.step();
   CPPUNIT_ASSERT_EQUAL(Point(505, 610), app.getMousePos());
}

///////////////////////////////////////////////////////////////////////////////

TEST(ApplicationManager, signalingOtherApplication)
{
   ApplicationManagerMock manager;
   ApplicationMock menu("menu");
   ApplicationMock game("game");

   manager.addApplication(menu);
   manager.addApplication(game);

   manager.setEntryApplication("menu");
   manager.connect("menu", "game", 0);
   manager.connect("game", "menu", Application::ON_EXIT);


   // first step and menu gets initialized
   CPPUNIT_ASSERT_EQUAL(true, manager.step());
   CPPUNIT_ASSERT_EQUAL(true, menu.isInitialized());
   CPPUNIT_ASSERT_EQUAL(0.f, menu.getTimeElapsed());
   CPPUNIT_ASSERT_EQUAL(false, game.isInitialized());
   CPPUNIT_ASSERT_EQUAL(0.f, game.getTimeElapsed());

   CPPUNIT_ASSERT_THROW(menu.sendSignal("game", 5), std::logic_error);

   // switching over to game:
   //   - menu doesn't get deinitialized, but it stops receivig updates (enters hibernation)
   //   - game gets initialized
   menu.sendSignal(0);
   CPPUNIT_ASSERT_EQUAL(true, manager.step());
   CPPUNIT_ASSERT_EQUAL(true, menu.isInitialized());
   CPPUNIT_ASSERT_EQUAL(0.f, menu.getTimeElapsed());
   CPPUNIT_ASSERT_EQUAL(true, game.isInitialized());
   CPPUNIT_ASSERT_EQUAL(0.f, game.getTimeElapsed());

   // sending signal from a hibernated app to the active app
   menu.sendSignal("game", 5);
   CPPUNIT_ASSERT_EQUAL(true, manager.step());
   CPPUNIT_ASSERT_EQUAL(-1, menu.getReceivedSignal());
   CPPUNIT_ASSERT_EQUAL(5, game.getReceivedSignal());

   // sending signal from the active app to a hibernated app
   menu.sendSignal("menu", 7);
   CPPUNIT_ASSERT_EQUAL(true, manager.step());
   CPPUNIT_ASSERT_EQUAL(7, menu.getReceivedSignal());
   CPPUNIT_ASSERT_EQUAL(-1, game.getReceivedSignal());
}

///////////////////////////////////////////////////////////////////////////////
