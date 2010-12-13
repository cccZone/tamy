/// @file   TamyEditor\FloatPropertyEditor.h
/// @brief  floating point value property editor
#pragma once

#include "core\Property.h"
#include "QPropertyEditor.h"


///////////////////////////////////////////////////////////////////////////////

class PropertiesEditor;

///////////////////////////////////////////////////////////////////////////////

/**
 * Editor widget that will edit float value properties.
 */
class FloatPropertyEditor : public QPropertyEditor
{
   Q_OBJECT

private:
   TEditableProperty< float >*    m_property;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   FloatPropertyEditor( TEditableProperty< float >* property );
   ~FloatPropertyEditor();

public slots:
   void valChanged(double val);

private:
   void setupUi();
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Editor widget that will edit double value properties.
 */
class DoublePropertyEditor : public QPropertyEditor
{
   Q_OBJECT

private:
   TEditableProperty< double >*    m_property;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   DoublePropertyEditor( TEditableProperty< double >* property );
   ~DoublePropertyEditor();

public slots:
   void valChanged(double val);

private:
   void setupUi();
};

///////////////////////////////////////////////////////////////////////////////
