#include "core-TestFramework\TestFramework.h"
#include "UserInputControllerMock.h"
#include "core-TestFramework\MatrixWriter.h"
#include <windows.h>


///////////////////////////////////////////////////////////////////////////////

TEST(UserInputController, keyboardInput)
{
   UserInputControllerMock controller;

   controller.keyDown(VK_UP);
   controller.update(0);
   CPPUNIT_ASSERT_EQUAL(true, controller.isKeyPressed(VK_UP));
   CPPUNIT_ASSERT_EQUAL(false, controller.isKeyPressed(VK_DOWN));
   CPPUNIT_ASSERT_EQUAL(false, controller.isKeyPressed(VK_LEFT));

   controller.keyDown(VK_LEFT);
   controller.keyDown(VK_DOWN);
   controller.update(0);
   CPPUNIT_ASSERT_EQUAL(false, controller.isKeyPressed(VK_UP));
   CPPUNIT_ASSERT_EQUAL(true, controller.isKeyPressed(VK_DOWN));
   CPPUNIT_ASSERT_EQUAL(true, controller.isKeyPressed(VK_LEFT));
}

///////////////////////////////////////////////////////////////////////////////

TEST(UserInputController, regularMouseMovement)
{
   UserInputControllerMock controller;

   controller.mouseMove(Point(500, 600));
   controller.update(1);
   CPPUNIT_ASSERT_EQUAL(Point(500, 600), controller.getMousePos());
   CPPUNIT_ASSERT_EQUAL(500.0f, controller.getMouseSpeed().v[0]);
   CPPUNIT_ASSERT_EQUAL(600.0f, controller.getMouseSpeed().v[1]);
   CPPUNIT_ASSERT_EQUAL(Point(500, 600), controller.getCursorPosition());

   controller.mouseMove(Point(505, 610));
   controller.update(1);
   CPPUNIT_ASSERT_EQUAL(Point(505, 610), controller.getMousePos());
   CPPUNIT_ASSERT_EQUAL(5.0f, controller.getMouseSpeed().v[0]);
   CPPUNIT_ASSERT_EQUAL(10.0f, controller.getMouseSpeed().v[1]);
   CPPUNIT_ASSERT_EQUAL(Point(505, 610), controller.getCursorPosition());
}

///////////////////////////////////////////////////////////////////////////////

TEST(UserInputController, absoluteMouseMovement)
{
   UserInputControllerMock controller;
   controller.setBounds(150, 100);

   controller.mouseMove(Point(150, 100));
   controller.update(1);
   CPPUNIT_ASSERT_EQUAL(Point(150, 100), controller.getMousePos());
   CPPUNIT_ASSERT_EQUAL(150.0f, controller.getMouseSpeed().v[0]);
   CPPUNIT_ASSERT_EQUAL(100.0f, controller.getMouseSpeed().v[1]);
   CPPUNIT_ASSERT_EQUAL(Point(150, 100), controller.getCursorPosition());

   // mouse can't go outside the screen bounds
   controller.mouseMove(Point(2, 2));
   controller.update(1);
   CPPUNIT_ASSERT_EQUAL(Point(2, 2), controller.getMousePos());
   CPPUNIT_ASSERT_EQUAL(-148.0f, controller.getMouseSpeed().v[0]);
   CPPUNIT_ASSERT_EQUAL(-98.0f, controller.getMouseSpeed().v[1]);
   CPPUNIT_ASSERT_EQUAL(Point(2, 2), controller.getCursorPosition());
}

///////////////////////////////////////////////////////////////////////////////

TEST(UserInputController, relativeMouseMovement)
{
   UserInputControllerMock controller;

   controller.mouseMove(Point(50, 50));
   controller.update(1);
   CPPUNIT_ASSERT_EQUAL(Point(50, 50), controller.getMousePos());
   CPPUNIT_ASSERT_EQUAL(50.0f, controller.getMouseSpeed().v[0]);
   CPPUNIT_ASSERT_EQUAL(50.0f, controller.getMouseSpeed().v[1]);
   CPPUNIT_ASSERT_EQUAL(Point(50, 50), controller.getCursorPosition());

   controller.setRelativeMouseMovement( true );
   controller.update(1);
   CPPUNIT_ASSERT_EQUAL(Point(50, 50), controller.getMousePos());
   CPPUNIT_ASSERT_EQUAL(0.0f, controller.getMouseSpeed().v[0]);
   CPPUNIT_ASSERT_EQUAL(0.0f, controller.getMouseSpeed().v[1]);
   CPPUNIT_ASSERT_EQUAL(Point(50, 50), controller.getCursorPosition());

   controller.mouseMove(Point(52, 54));
   controller.update(1);
   CPPUNIT_ASSERT_EQUAL(Point(52, 54), controller.getMousePos());
   CPPUNIT_ASSERT_EQUAL(2.0f, controller.getMouseSpeed().v[0]);
   CPPUNIT_ASSERT_EQUAL(4.0f, controller.getMouseSpeed().v[1]);
   CPPUNIT_ASSERT_EQUAL(Point(50, 50), controller.getCursorPosition());
}

///////////////////////////////////////////////////////////////////////////////
