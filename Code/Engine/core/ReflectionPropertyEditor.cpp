#include "core\ReflectionPropertyEditor.h"
#include "core\ReflectionProperty.h"


///////////////////////////////////////////////////////////////////////////////

ReflectionPropertyEditorComposite::ReflectionPropertyEditorComposite( ReflectionPropertyArray* arrayProperty )
   : m_arrayProperty( arrayProperty )
{
}

///////////////////////////////////////////////////////////////////////////////

ReflectionPropertyEditorComposite::~ReflectionPropertyEditorComposite()
{
   uint count = m_editors.size();
   for ( uint i = 0; i < count; ++i )
   {
      delete m_editors[i];
   }
   m_editors.clear();

   delete m_arrayProperty;
   m_arrayProperty = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void ReflectionPropertyEditorComposite::initialize( ReflectionObjectEditor* parentEditor )
{
   unsigned int count = m_editors.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      ReflectionObjectEditor* editor = m_editors[i];
      editor->initialize( this );
   }
}

///////////////////////////////////////////////////////////////////////////////

void ReflectionPropertyEditorComposite::deinitialize( ReflectionObjectEditor* parentEditor )
{
   unsigned int count = m_editors.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      ReflectionObjectEditor* editor = m_editors[i];

      // deinitialize only if the derived view still exists
      // (which it may not if it's actually undergoing the destruction process)
      editor->deinitialize( this );
   }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

ReflectionObjectEditor::ReflectionObjectEditor( ReflectionObject* editedObject )
   : m_editedObject( editedObject )
{
}

///////////////////////////////////////////////////////////////////////////////

ReflectionObjectEditor::~ReflectionObjectEditor()
{
   uint count = m_editors.size();
   for ( uint i = 0; i < count; ++i )
   {
      delete m_editors[i];
   }
   m_editors.clear();

   // don't delete the edited object - after all we were just editing it, it's still out there,
   // doing its fine job - leave it to  it ;)
   m_editedObject = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void ReflectionObjectEditor::initialize( ReflectionPropertyEditorComposite* parentComposite )
{
   unsigned int count = m_editors.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      ReflectionPropertyEditor* editor = m_editors[i];
      editor->initialize( this );
   }
}

///////////////////////////////////////////////////////////////////////////////

void ReflectionObjectEditor::deinitialize( ReflectionPropertyEditorComposite* parentComposite )
{
   unsigned int count = m_editors.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      ReflectionPropertyEditor* editor = m_editors[i];
      editor->deinitialize( this );
   }
}

///////////////////////////////////////////////////////////////////////////////
