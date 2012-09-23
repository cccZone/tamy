#include "core-MVC/ModelEntity.h"
#include "core-MVC/Model.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( ModelEntity )
   PARENT( SpatialEntity )
   PROPERTY_EDIT( "EmbeddedModel", Model*, m_model )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

ModelEntity::ModelEntity( const std::string& name )
   : SpatialEntity( name )
   , m_model( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

ModelEntity::ModelEntity( const ModelEntity& rhs )
   : SpatialEntity( rhs )
   , m_model( rhs.m_model )
{
}

///////////////////////////////////////////////////////////////////////////////

ModelEntity::ModelEntity( Model& model, const std::string& name )
   : SpatialEntity( name )
   , m_model( &model )
{
   onModelChanged();
}

///////////////////////////////////////////////////////////////////////////////

ModelEntity::~ModelEntity()
{
   // can't delete that - it's a resource - ResourcesManager deals with it
   m_model = NULL;

   // model has changed - process the change
   onModelChanged();
}

///////////////////////////////////////////////////////////////////////////////

void ModelEntity::onObjectLoaded()
{
   __super::onObjectLoaded();

   onModelChanged();
}

///////////////////////////////////////////////////////////////////////////////

void ModelEntity::onPropertyChanged( ReflectionProperty& property )
{
   __super::onPropertyChanged( property );

   onModelChanged();
}

///////////////////////////////////////////////////////////////////////////////

Entity* ModelEntity::cloneSelf() const
{
   ModelEntity* entity = new ModelEntity( *this );
   return entity;
}

///////////////////////////////////////////////////////////////////////////////

void ModelEntity::onModelChanged()
{
   // remove existing entities
   unsigned int count = m_embeddedEntities.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      remove( *m_embeddedEntities[i], true );
   }
   m_embeddedEntities.clear();

   // if the model is set, clone the entities it contains and attach them
   // as the children of this entity
   if ( m_model )
   {
      // clone the entities structure stored in the embedded model
      m_model->clone( m_embeddedEntities );

      // add new children to the entity
      unsigned int count = m_embeddedEntities.size();
      for ( unsigned int i = 0; i < count; ++i )
      {
         add( m_embeddedEntities[i], false );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
