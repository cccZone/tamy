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
   TEditableProperty< bool >*    m_val;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   BoolPropertyEditor( TEditableProperty< bool >* property );
   ~BoolPropertyEditor();

public slots:
   void valChanged(int val);

private:
   void setupUi();
};

///////////////////////////////////////////////////////////////////////////////
