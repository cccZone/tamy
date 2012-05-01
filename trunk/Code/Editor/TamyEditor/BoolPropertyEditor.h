/// @file   TamyEditor\ColorPropertyEditor.h
/// @brief  bool property editor
#pragma once


#include "core\ReflectionProperty.h"
#include "QPropertyEditor.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Editor widget that will edit boolean properties.
 */
class BoolPropertyEditor : public QPropertyEditor
{
   Q_OBJECT

private:
   TEditableReflectionProperty< bool >*    m_val;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   BoolPropertyEditor( TEditableReflectionProperty< bool >* property );
   ~BoolPropertyEditor();

public slots:
   void valChanged(int val);

private:
   void setupUi();
};

///////////////////////////////////////////////////////////////////////////////
