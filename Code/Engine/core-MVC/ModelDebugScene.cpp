#include "core-MVC/ModelDebugScene.h"
#include "core-MVC/Entity.h"
#include "core/IDebugDraw.h"


///////////////////////////////////////////////////////////////////////////////

ModelDebugScene::ModelDebugScene( DebugScene& debugScene )
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
      m_debugScene.add( *drawable );
   }
}

///////////////////////////////////////////////////////////////////////////////

void ModelDebugScene::onEntityRemoved( Entity& entity )
{
   IDebugDrawable* drawable = dynamic_cast< IDebugDrawable* >( &entity );
   if ( drawable )
   {
      m_entities.erase( drawable );
      m_debugScene.remove( *drawable );
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
      m_debugScene.remove( **it );
   }
   m_entities.clear();
}

///////////////////////////////////////////////////////////////////////////////
