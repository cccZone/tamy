#pragma once

/// @file   core-Scene\PropertyEditor.h
/// @brief  property editor interface


///////////////////////////////////////////////////////////////////////////////

/**
 * This is a generic property editor interface. It will allow to store editors
 * of various types in a uniform manner in all sorts of collections.
 */
class PropertyEditor
{
public:
   virtual ~PropertyEditor() {}
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This specialized interface of property editor is what you should derive all
 * your editors from!
 * It's specialized to work with a specific type of view, allowing to treat it
 * as a factory of stuff required etc.
 */
template <typename ViewType>
class TPropertyEditor : public PropertyEditor
{
public:
   virtual ~TPropertyEditor() {}

   /**
    * This method will be called once the editor is created.
    *
    * @param view    view that manages the editor.
    */
   virtual void initialize(ViewType& view) = 0;

   /**
    * This method will be called when the editor is about to be destroyed.
    *
    * @param view    view that manages the editor.
    */
   virtual void deinitialize(ViewType& view) = 0;
};

///////////////////////////////////////////////////////////////////////////////
