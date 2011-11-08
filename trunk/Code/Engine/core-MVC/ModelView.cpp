#include "core-MVC\ModelView.h"
#include "core-MVC\Model.h"


///////////////////////////////////////////////////////////////////////////////

ModelView::ModelView()
{
}

///////////////////////////////////////////////////////////////////////////////

ModelView::~ModelView()
{
   while( m_models.empty() == false )
   {
      Model* model = *m_models.begin();
      model->detach( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

void ModelView::onAttachedToModel( Model& model  )
{
   // check if the model isn't already on our list
   unsigned int count = m_models.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      if ( m_models[i] == &model )
      {
         // yup - we're already observing this one
         return;
      }
   }
   m_models.push_back( &model );
}

///////////////////////////////////////////////////////////////////////////////

void ModelView::onDetachedFromModel( Model& model )
{
   unsigned int count = m_models.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      if ( m_models[i] == &model )
      {
         m_models.erase( m_models.begin() + i );
         resetContents();
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

bool ModelView::isAttachedToModel() const
{
   return !m_models.empty();
}

///////////////////////////////////////////////////////////////////////////////
