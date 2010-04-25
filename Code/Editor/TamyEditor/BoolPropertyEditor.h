#pragma once

/// @file   TamyEditor\ColorPropertyEditor.h
/// @brief  bool property editor

#include "core\Property.h"
#include "QPropertyEditor.h"


///////////////////////////////////////////////////////////////////////////////

class PropertiesEditor;

///////////////////////////////////////////////////////////////////////////////

/**
 * Editor widget that will edit boolean properties.
 */
class BoolPropertyEditor : public QPropertyEditor
{
   Q_OBJECT

private:
   TProperty<bool>& m_property;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   BoolPropertyEditor(TProperty<bool>& property);

public slots:
   void valChanged(int val);

private:
   void setupUi();
};

///////////////////////////////////////////////////////////////////////////////
