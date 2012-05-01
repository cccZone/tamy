#ifndef REFLECTION_PROPERTIES_VIEW_H
#error "This file can only be included from PropertiesView.h"
#else

#include "core\ReflectionPropertyEditor.h"
#include "core\ReflectionProperties.h"


///////////////////////////////////////////////////////////////////////////////

template< typename Derived >
TReflectionPropertiesView< Derived >::TReflectionPropertiesView()
   : m_properties( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename Derived >
TReflectionPropertiesView< Derived >::~TReflectionPropertiesView() 
{
   reset();

   // delete the creators
   unsigned int count = m_creators.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete m_creators[ i ];
   }
   m_creators.clear();
}

///////////////////////////////////////////////////////////////////////////////

template< typename Derived >
void TReflectionPropertiesView< Derived >::set( ReflectionProperties* properties )
{
   reset();

   m_properties = properties;
   onSet( *properties );

   Derived* thisDerived = dynamic_cast<Derived*> (this);

   const ReflectionProperties::Names& names = properties->getNames();
   for ( ReflectionProperties::Names::const_iterator it = names.begin(); it != names.end(); ++it )
   {
      ReflectionProperty& property = properties->get( *it );
      if ( !property.canBeEdited() )
      {
         continue;
      }
      ReflectionPropertyEditor* editor = create(property);
      if ( editor == NULL )
      {
         continue;
      }

      TReflectionPropertyEditor< Derived >* editorWithProperType = dynamic_cast< TReflectionPropertyEditor< Derived >* >( editor );
      if ( editorWithProperType == NULL )
      {
         delete editor;
         throw std::runtime_error("Editors must be specialized to a specific view type");
      }

      editorWithProperType->initialize( *thisDerived );
      m_editors.push_back( editor );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename Derived >
void TReflectionPropertiesView< Derived >::reset()
{
   Derived* thisDerived = dynamic_cast< Derived* >( this );

   // delete the editors
   unsigned int count = m_editors.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      ReflectionPropertyEditor* editor = m_editors[i];
      TReflectionPropertyEditor< Derived >* editorWithProperType = dynamic_cast< TReflectionPropertyEditor< Derived >* > (editor);

      if ( thisDerived != NULL )
      {
         // deinitialize only if the derived view still exists
         // (which it may not if it's actually undergoing the destruction process)
         editorWithProperType->deinitialize( *thisDerived );
      }

      delete editor;
   }
   m_editors.clear();

   // delete the properties instance
   delete m_properties;
   m_properties = NULL;
}

///////////////////////////////////////////////////////////////////////////////

template< typename Derived >
unsigned int TReflectionPropertiesView< Derived >::getEditorsCount() const
{
   return m_editors.size();
}

///////////////////////////////////////////////////////////////////////////////

template< typename Derived >
ReflectionPropertyEditor& TReflectionPropertiesView< Derived >::getEditor( uint idx ) const
{
   return *m_editors[idx];
}

///////////////////////////////////////////////////////////////////////////////

template< typename Derived >
ReflectionPropertyEditor* TReflectionPropertiesView<Derived>::create( ReflectionProperty& property )
{
   const ReflectionType& checkedClassType = property.getVirtualClass();

   for ( CreatorsVec::iterator it = m_creators.begin();  it != m_creators.end(); ++it )
   {
      Creator& creator = **it;
      if ( creator.doesMatch( checkedClassType ) )
      {
         ReflectionPropertyEditor* editor = creator( &property );
         return editor;
      }
   }
   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

#endif // REFLECTION_PROPERTIES_VIEW_H