#ifndef _FLOAT_PROPERTY_EDITOR_H
#define _FLOAT_PROPERTY_EDITOR_H

/// @file   TamyEditor\FloatPropertyEditor.h
/// @brief  floating point value property editor

#include "core-Scene\Property.h"
#include "QPropertyEditor.h"


///////////////////////////////////////////////////////////////////////////////

class PropertiesEditor;

///////////////////////////////////////////////////////////////////////////////

/**
 * Editor widget that will edit floating point value properties.
 */
template <typename FLOAT_TYPE>
class FloatPropertyEditor : public QPropertyEditor
{
private:
   TProperty<FLOAT_TYPE>& m_property;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   FloatPropertyEditor(TProperty<FLOAT_TYPE>& property);

public slots:
   void valChanged(double val);

private:
   void setupUi();
};

///////////////////////////////////////////////////////////////////////////////

#include "FloatPropertyEditor.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _FLOAT_PROPERTY_EDITOR_H
