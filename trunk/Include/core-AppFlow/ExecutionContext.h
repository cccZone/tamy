#pragma once


///////////////////////////////////////////////////////////////////////////////

class Application;
struct Point;

///////////////////////////////////////////////////////////////////////////////

/**
 * This interface provides the means for an Application instance
 * to communicate back with the ApplicationManager, without exposing
 * AppManager's methods (such as deinitialize and initialize) to the app.
 */
class ExecutionContext
{
public:
   virtual ~ExecutionContext() {}

   /**
    * This way we can send signals to the app manager
    * to inform it that we want to switch over to a different app,
    * or that it finished whatever it was doing and can be closed
    */
   virtual void signal(const Application& app, int signalId) = 0;

   virtual bool isKeyPressed(unsigned char keyCode) const = 0;

   virtual const Point& getMousePos() const = 0;

   virtual void relativeMouseMovement(bool enable) = 0;
};

///////////////////////////////////////////////////////////////////////////////
