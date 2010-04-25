#pragma once

/// @file   TamyEditor\ColorPropertyEditor.h
/// @brief  color property editor

#include "core-Scene\Property.h"
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
   TProperty<Color>& m_property;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   ColorPropertyEditor(TProperty<Color>& property);

public slots:
   void rValChanged(double val);
   void gValChanged(double val);
   void bValChanged(double val);
   void aValChanged(double val);

private:
   void setupUi();
};

///////////////////////////////////////////////////////////////////////////////
