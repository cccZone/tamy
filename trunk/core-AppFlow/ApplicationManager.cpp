#include "core-AppFlow\ApplicationManager.h"
#include "core-AppFlow\Application.h"
#include <stdexcept>
#include <windows.h>


///////////////////////////////////////////////////////////////////////////////

ApplicationManager::ApplicationManager()
{
   ZeroMemory(m_keyBuffer, 256 * sizeof(unsigned char));
}

///////////////////////////////////////////////////////////////////////////////

void ApplicationManager::addApplication(Application& app)
{
   AppsMap::iterator it = m_apps.find(app.getName());
   if (it != m_apps.end())
   {
      throw std::invalid_argument(std::string("Application '") + app.getName() +
                                  std::string("' already registered"));
   }

   m_apps.insert(std::make_pair(app.getName(), ApplicationNode(app)));
   app.setContext(*this);
}

///////////////////////////////////////////////////////////////////////////////

void ApplicationManager::setEntryApplication(const std::string& appName)
{
   AppsMap::iterator it = m_apps.find(appName);
   if (it == m_apps.end())
   {
      throw std::invalid_argument(std::string("Application '") + appName +
                                  std::string("' not registered"));
   }

   it->second.state = AS_SCHEDULED;
}

///////////////////////////////////////////////////////////////////////////////

void ApplicationManager::connect(const std::string& originator, 
                                 const std::string& targetApp, 
                                 int signal)
{
   AppsMap::iterator originatorIt = m_apps.find(originator);
   if (originatorIt == m_apps.end())
   {
      throw std::invalid_argument(std::string("Application '") + originator +
                                  std::string("' not registered"));
   }

   AppsMap::iterator targetAppIt = m_apps.find(targetApp);
   if (targetAppIt == m_apps.end())
   {
      throw std::invalid_argument(std::string("Application '") + targetApp +
                                  std::string("' not registered"));
   }

   ApplicationNode& originatorNode = originatorIt->second;
   std::map<int, std::string>::iterator connectionIt = originatorNode.connections.find(signal);
   if (connectionIt != originatorNode.connections.end())
   {
      throw std::invalid_argument(std::string("Signal already defined for '") + originator +
                                  std::string("'"));
   }

   originatorNode.connections.insert(std::make_pair(signal, targetApp));
}

///////////////////////////////////////////////////////////////////////////////

bool ApplicationManager::step()
{
   switch(onStep())
   {
   case APC_SYSTEM:      return true;
   case APC_APPLICATION: break;
   case APC_EXIT:        return false;
   }

   int numRunning = 0;

   for (AppsMap::iterator appIt = m_apps.begin(); 
        appIt != m_apps.end(); ++appIt)
   {
      ApplicationNode& currNode = appIt->second;

      switch(currNode.state)
      {
      case AS_UNINITIALIZED:
         {
            // do nothing
            break;
         }

      case AS_SCHEDULED:
         {
            currNode.app.initialize(getRenderer(), getResourceManager());
            currNode.state = AS_RUNNING;
            break;
         }

      case AS_RUNNING:
         {
            currNode.app.update(getTimeElapsed());
            break;
         }

      case AS_FINISHED:
         {
            currNode.app.deinitialize();
            currNode.state = AS_UNINITIALIZED;
            break;
         }
      }

      if (currNode.state == AS_RUNNING) { numRunning++; }     
      if (numRunning > 1)
      {
         throw std::runtime_error("Only one application can be running at a time");
      }
   }

   checkUserInput(m_keyBuffer, m_mousePos);

   return (numRunning == 1);
}

///////////////////////////////////////////////////////////////////////////////

void ApplicationManager::signal(const Application& app, int signalId)
{
   // locate the app in the registry
   AppsMap::iterator appIt = m_apps.find(app.getName());
   if (appIt == m_apps.end())
   {
      throw std::runtime_error(std::string("Application '") + app.getName() +
                               std::string("' not registered"));
   }

   ApplicationNode& appNode = appIt->second;

   // if the app wants to be closed - change its state
   if (signalId == Application::ON_EXIT) {appNode.state = AS_FINISHED;}

   // check if there is a connection for this app on this signal
   std::map<int, std::string>::iterator connectionIt = appNode.connections.find(signalId);
   if (connectionIt == appNode.connections.end()) {return;}

   AppsMap::iterator targetAppIt = m_apps.find(connectionIt->second);
   if (targetAppIt == m_apps.end())
   {
      throw std::runtime_error(std::string("Application '") + connectionIt->second +
                               std::string("' not registered"));
   }

   // switch over to the other application
   if (appNode.state != AS_FINISHED)
   {
      appNode.state = AS_UNINITIALIZED;
   }
   targetAppIt->second.state = AS_SCHEDULED;
}

///////////////////////////////////////////////////////////////////////////////

bool ApplicationManager::isKeyPressed(unsigned char keyCode) const
{
   return (bool)(m_keyBuffer[keyCode] & 0xF0); 
}

///////////////////////////////////////////////////////////////////////////////

void ApplicationManager::relativeMouseMovement(bool enable)
{
   switchMouseMovementMode(enable);
   checkUserInput(m_keyBuffer, m_mousePos);
}

///////////////////////////////////////////////////////////////////////////////
