#pragma once

/// @file   TamyEditor\ColorPropertyEditor.h
/// @brief  color property editor

#include "core\Property.h"
#include "QPropertyEditor.h"


///////////////////////////////////////////////////////////////////////////////

class PropertiesEditor;
struct Color;

///////////////////////////////////////////////////////////////////////////////

/**
 * Editor widget that will edit color properties.
 */
class ColorPropertyEditor : public QPropertyEditor
{
   Q_OBJECT

private:
   TEditableProperty< Color >*   m_property;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   ColorPropertyEditor( TEditableProperty< Color >* property );
   ~ColorPropertyEditor();

public slots:
   void rValChanged(double val);
   void gValChanged(double val);
   void bValChanged(double val);
   void aValChanged(double val);

private:
   void setupUi();
};

///////////////////////////////////////////////////////////////////////////////
