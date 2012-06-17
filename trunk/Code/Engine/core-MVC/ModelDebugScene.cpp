#include "core-MVC/ModelDebugScene.h"
#include "core-MVC/Entity.h"
#include "core/IDebugDraw.h"


///////////////////////////////////////////////////////////////////////////////

ModelDebugScene::ModelDebugScene( Model& debugScene )
   : m_debugScene( debugScene )
{
}

///////////////////////////////////////////////////////////////////////////////

ModelDebugScene::~ModelDebugScene()
{
   resetContents();
}

///////////////////////////////////////////////////////////////////////////////

void ModelDebugScene::onEntityAdded( Entity& entity )
{
   IDebugDrawable* drawable = dynamic_cast< IDebugDrawable* >( &entity );
   if ( drawable )
   {
      m_entities.insert( drawable );
      drawable->onInitializeDebugRepresentation( m_debugScene );
   }
}

///////////////////////////////////////////////////////////////////////////////

void ModelDebugScene::onEntityRemoved( Entity& entity )
{
   IDebugDrawable* drawable = dynamic_cast< IDebugDrawable* >( &entity );
   if ( drawable )
   {
      m_entities.erase( drawable );
      drawable->onDeinitializeDebugRepresentation( m_debugScene );
   }
}

///////////////////////////////////////////////////////////////////////////////

void ModelDebugScene::onEntityChanged( Entity& entity )
{
   // do nothing
}

///////////////////////////////////////////////////////////////////////////////

void ModelDebugScene::resetContents()
{
   for ( std::set< IDebugDrawable* >::const_iterator it = m_entities.begin(); it != m_entities.end(); ++it )
   {
      IDebugDrawable* drawable = *it;
      drawable->onDeinitializeDebugRepresentation( m_debugScene );
   }
   m_entities.clear();
}

///////////////////////////////////////////////////////////////////////////////
