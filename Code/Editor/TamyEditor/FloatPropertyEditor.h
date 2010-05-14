#ifndef _FLOAT_PROPERTY_EDITOR_H
#define _FLOAT_PROPERTY_EDITOR_H

/// @file   TamyEditor\FloatPropertyEditor.h
/// @brief  floating point value property editor

#include "core\Property.h"
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
   FLOAT_TYPE& m_property;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   FloatPropertyEditor( FLOAT_TYPE& property, const std::string& label );

public slots:
   void valChanged(double val);

private:
   void setupUi();
};

///////////////////////////////////////////////////////////////////////////////

#include "FloatPropertyEditor.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _FLOAT_PROPERTY_EDITOR_H
