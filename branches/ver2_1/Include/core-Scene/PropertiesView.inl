#ifndef _PROPERTIES_VIEW_H
#error "This file can only be included from PropertiesView.h"
#else

#include "core-Scene\PropertyEditor.h"
#include "core-Scene\Properties.h"


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

#endif // _PROPERTIES_VIEW_H