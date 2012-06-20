/// @file   core\ReflectionPropertyEditor.h
/// @brief  property editor interface
#ifndef _REFLECTION_PROPERTY_EDITOR_H
#define _REFLECTION_PROPERTY_EDITOR_H

#include <vector>
#include "core\types.h"
#include "core\ReflectionObjectChangeListener.h"


///////////////////////////////////////////////////////////////////////////////

class ReflectionPropertyArray;
class ReflectionPropertyEditorComposite;
class ReflectionObjectEditor;
class ReflectionObject;

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a generic property editor interface. It will allow to store editors
 * of various types in a uniform manner in all sorts of collections.
 */
class ReflectionPropertyEditor
{
public:
   uint            m_propertyId;   

public:
   virtual ~ReflectionPropertyEditor() {}

   /**
    * This method will be called once the editor is created.
    *
    * @param parentEditor    object editor that manages this editor.
    */
   virtual void initialize( ReflectionObjectEditor* parentEditor ) = 0;

   /**
    * This method will be called when the editor is about to be destroyed.
    *
    * @param parentEditor    object editor that manages this editor.
    */
   virtual void deinitialize( ReflectionObjectEditor* parentEditor ) = 0;

   /**
    * Called when the property this editor edits has changed.
    */
   virtual void onPropertyChanged() = 0;

protected:
   /**
    * Constructor.
    */
   ReflectionPropertyEditor() : m_propertyId( 0 ) {}
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a special type of editor that will be created whenever an array-property
 * is encountered.
 * It holds an array of object editors, which in turn hold other property editors.
 */
class ReflectionPropertyEditorComposite : public ReflectionPropertyEditor
{
private:
   ReflectionPropertyArray*                     m_arrayProperty;
   std::vector< ReflectionObjectEditor* >		   m_editors;

public:
   ReflectionPropertyEditorComposite( ReflectionPropertyArray* arrayProperty );
   virtual ~ReflectionPropertyEditorComposite();

   /**
    * Adds a new editor to the composite.
    */
   inline void addObjectEditor( ReflectionObjectEditor* editor ) { m_editors.push_back( editor ); }

   /**
    * Returns the number of managed editors.
    */
   inline uint getEditorsCount() const { return m_editors.size(); }

   /**
    * Returns an editor for the specified element.
    *
    * CAUTION: some instances may be null, because some elements of the array
    * may not have a proper editor defined.
    *
    * @param idx		index of an element we we want to retrieve an editor of
    */
   inline ReflectionObjectEditor* getEditor( int idx ) { return m_editors[idx]; }

   // -------------------------------------------------------------------------
   // ReflectionPropertyEditor implementation
   // -------------------------------------------------------------------------
   void initialize( ReflectionObjectEditor* parentEditor );
   void deinitialize( ReflectionObjectEditor* parentEditor );
   void onPropertyChanged();
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This editor node holds the actual property editors.
 */
class ReflectionObjectEditor : public ReflectionObjectChangeListener
{
private:
   ReflectionObject*                               m_editedObject;
   std::vector< ReflectionPropertyEditor* >		   m_editors;

public:
   ReflectionObjectEditor( ReflectionObject* m_editedObject );
   virtual ~ReflectionObjectEditor();

   /**
    * Returns a reference to the edited object.
    */
   inline ReflectionObject& getEditedObject() { return *m_editedObject; }

   /**
    * Adds a new editor to the composite.
    */
   inline void addPropertyEditor( ReflectionPropertyEditor* editor ) { m_editors.push_back( editor ); }

   /**
    * Returns the number of property editors registered with this object editor.
    */
   inline uint getPropertiesCount() const { return m_editors.size(); }

   /**
    * Returns the specified property editor.
    */
   inline ReflectionPropertyEditor* getPropertyEditor( uint idx ) const { return m_editors[idx]; }

   /**
    * This method will be called once the editor is created.
    *
    * @param parent    composite that manages the editor.
    */
   virtual void initialize( ReflectionPropertyEditorComposite* parentComposite );

   /**
    * This method will be called when the editor is about to be destroyed.
    *
    * @param parent    composite that manages the editor.
    */
   virtual void deinitialize( ReflectionPropertyEditorComposite* parentComposite );

   // -------------------------------------------------------------------------
   // ReflectionObjectChangeListener implementation
   // -------------------------------------------------------------------------
   void onPropertyChanged( ReflectionProperty& property );
};

///////////////////////////////////////////////////////////////////////////////

#include "core\ReflectionPropertyEditor.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _REFLECTION_PROPERTY_EDITOR_H
