#ifndef _PROPERTIES_VIEW_H
#error "This file can only be included from PropertiesView.h"
#else

#include "core\PropertyEditor.h"
#include "core\Properties.h"


///////////////////////////////////////////////////////////////////////////////

template <typename Derived>
TPropertiesView<Derived>::~TPropertiesView() 
{
   reset();
}

///////////////////////////////////////////////////////////////////////////////

template <typename Derived>
void TPropertiesView<Derived>::set(Properties& properties)
{
   onSet(properties);

   Derived* thisDerived = dynamic_cast<Derived*> (this);

   const Properties::Names& names = properties.getNames();
   for (Properties::Names::const_iterator it = names.begin();
      it != names.end(); ++it)
   {
      Property& property = properties.get(*it);

      PropertyEditor* editor = create(property);
      if (editor == NULL)
      {
         continue;
      }

      TPropertyEditor<Derived>* editorWithProperType = dynamic_cast<TPropertyEditor<Derived>*> (editor);
      if (editorWithProperType == NULL)
      {
         delete editor;
         throw std::runtime_error("Editors must be specialized to a specific view type");
      }

      editorWithProperType->initialize(*thisDerived);
      m_editors.push_back(editor);
   }
}

///////////////////////////////////////////////////////////////////////////////

template <typename Derived>
void TPropertiesView<Derived>::reset()
{
   Derived* thisDerived = dynamic_cast<Derived*> (this);

   unsigned int count = m_editors.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      PropertyEditor* editor = m_editors[i];
      TPropertyEditor<Derived>* editorWithProperType = dynamic_cast<TPropertyEditor<Derived>*> (editor);

      if (thisDerived != NULL)
      {
         // deinitialize only if the derived view still exists
         // (which it may not if it's actually undergoing the destruction process)
         editorWithProperType->deinitialize(*thisDerived);
      }

      delete editor;
   }
   m_editors.clear();
}

///////////////////////////////////////////////////////////////////////////////

template <typename Derived>
unsigned int TPropertiesView<Derived>::getEditorsCount() const
{
   return m_editors.size();
}

///////////////////////////////////////////////////////////////////////////////

template <typename Derived>
PropertyEditor* TPropertiesView<Derived>::create( Property& property )
{
   // first try to find a solid creator for it...
   PropertyEditor* editor = createSolid( property );

   if ( !editor )
   {
      // ...if that's not possible - check if the class matches to 
      // any of the registered abstract types
      editor = createAbstract( property );
   }

   return editor;
}

///////////////////////////////////////////////////////////////////////////////

template <typename Derived>
PropertyEditor* TPropertiesView< Derived >::createSolid( Property& property )
{
   const Class& checkedClassType = property.getVirtualClass();

   for ( CreatorsVec::iterator it = m_solidCreators.begin();  
         it != m_solidCreators.end(); ++it )
   {
      Class& refClassType = (*it)->classType;
      if ( refClassType.isExactlyA( checkedClassType ) )
      {
         PropertyEditor* editor = (*((*it)->creator))( &property );
         return editor;
      }
   }
   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

template <typename Derived>
PropertyEditor* TPropertiesView< Derived >::createAbstract( Property& property )
{
   const Class& checkedClassType = property.getVirtualClass();

   for ( CreatorsVec::iterator it = m_abstractCreators.begin();  
         it != m_abstractCreators.end(); ++it )
   {
      Class& refClassType = (*it)->classType;
      if ( refClassType.isA(checkedClassType) )
      {
         PropertyEditor* editor = (*((*it)->creator))( &property );
         return editor;
      }
   }

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _PROPERTIES_VIEW_H