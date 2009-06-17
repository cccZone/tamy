#pragma once

#include "core-AppFlow\Application.h"
#include "core\IntervalOperation.h"
#include "core\dostream.h"


///////////////////////////////////////////////////////////////////////////////

class CompositeSceneManager;
class VisualSceneManager;

///////////////////////////////////////////////////////////////////////////////

class QueryNodesAction
{
private:
   const Renderer& m_renderer;
   const VisualSceneManager& m_sceneMgr;
   ExecutionContext& m_context;
   dostream m_dbg;

public:
   QueryNodesAction(const Renderer& renderer,
                    const VisualSceneManager& sceneMgr, 
                    ExecutionContext& context)
      : m_renderer(renderer),
      m_sceneMgr(sceneMgr),
      m_context(context)
   {
   }

   void operator()();
};

///////////////////////////////////////////////////////////////////////////////

class PickingDemo : public Application
{
private:
   Renderer* m_renderer;
   ResourceManager* m_resourceManager;

   CompositeSceneManager* m_sceneManager;
   VisualSceneManager * m_visualSceneManager;

   bool m_rotating;
   IntervalOperation<QueryNodesAction>* m_action;

public:
   PickingDemo();

   void initialize(Renderer& renderer, ResourceManager& resourceManager);

   void deinitialize();

   void update(float timeElapsed);

   void notify(const std::string& senderApp, int signalCode) {}
};

///////////////////////////////////////////////////////////////////////////////
