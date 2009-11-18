#ifndef _PROPERTIES_VIEW_H
#error "This file can only be included from PropertiesView.h"
#else

#include "core-Scene\PropertyEditor.h"
#include "core-Scene\Properties.h"


///////////////////////////////////////////////////////////////////////////////

template <typename Derrived>
TPropertiesView<Derrived>::~TPropertiesView() 
{
   reset();
}

///////////////////////////////////////////////////////////////////////////////

template <typename Derrived>
void TPropertiesView<Derrived>::set(Properties& properties)
{
   Derrived* thisDerrived = dynamic_cast<Derrived*> (this);

   const Properties::Names& names = properties.getNames();
   for (Properties::Names::const_iterator it = names.begin();
      it != names.end(); ++it)
   {
      Property& property = properties.get(*it);

      PropertyEditor* editor = create(property);
      TPropertyEditor<Derrived>* editorWithProperType = dynamic_cast<TPropertyEditor<Derrived>*> (editor);
      if (editorWithProperType == NULL)
      {
         delete editor;
         throw std::runtime_error("Editors must be specialized to a specific view type");
      }

      editorWithProperType->initialize(*thisDerrived);
      m_editors.push_back(editor);
   }
}

///////////////////////////////////////////////////////////////////////////////

template <typename Derrived>
void TPropertiesView<Derrived>::reset()
{
   Derrived* thisDerrived = dynamic_cast<Derrived*> (this);

   unsigned int count = m_editors.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      PropertyEditor* editor = m_editors[i];
      TPropertyEditor<Derrived>* editorWithProperType = dynamic_cast<TPropertyEditor<Derrived>*> (editor);
      editorWithProperType->deinitialize(*thisDerrived);

      delete editor;
   }
   m_editors.clear();
}

///////////////////////////////////////////////////////////////////////////////

template <typename Derrived>
unsigned int TPropertiesView<Derrived>::getEditorsCount() const
{
   return m_editors.size();
}

///////////////////////////////////////////////////////////////////////////////

#endif // _PROPERTIES_VIEW_H