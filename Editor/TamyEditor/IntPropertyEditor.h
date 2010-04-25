#ifndef _INT_PROPERTY_EDITOR_H
#define _INT_PROPERTY_EDITOR_H

/// @file   TamyEditor\IntPropertyEditor.h
/// @brief  integer value property editor

#include "core-Scene\Property.h"
#include "QPropertyEditor.h"


///////////////////////////////////////////////////////////////////////////////

class PropertiesEditor;

///////////////////////////////////////////////////////////////////////////////

/**
 * Editor widget that will edit integer value properties.
 */
template <typename INT_TYPE>
class IntPropertyEditor : public QPropertyEditor
{
private:
   TProperty<INT_TYPE>& m_property;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   IntPropertyEditor(TProperty<INT_TYPE>& property);

public slots:
   void valChanged(int val);

private:
   void setupUi();
};

///////////////////////////////////////////////////////////////////////////////

#include "IntPropertyEditor.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _INT_PROPERTY_EDITOR_H
