#include "core-MVC\ModelView.h"
#include "core-MVC\Model.h"


///////////////////////////////////////////////////////////////////////////////

ModelView::ModelView()
{
}

///////////////////////////////////////////////////////////////////////////////

ModelView::~ModelView()
{
   std::set< Model* > modelsCache = m_models;
   m_models.clear();

   for ( std::set< Model* >::iterator it = modelsCache.begin();
         it != modelsCache.end(); ++it )
   {
      (*it)->detach( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

void ModelView::onAttachedToModel( Model& model  )
{
   m_models.insert( &model );
}

///////////////////////////////////////////////////////////////////////////////

void ModelView::onDetachedFromModel( Model& model )
{
   std::set< Model* >::iterator it = m_models.find( &model );
   if ( it != m_models.end() )
   {
      m_models.erase( it );
      resetContents();
   }
}

///////////////////////////////////////////////////////////////////////////////

bool ModelView::isAttachedToModel() const
{
   return !m_models.empty();
}

///////////////////////////////////////////////////////////////////////////////
