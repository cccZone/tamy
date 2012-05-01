/// @file   TamyEditor\FloatPropertyEditor.h
/// @brief  floating point value property editor
#pragma once

#include "core\ReflectionProperty.h"
#include "QPropertyEditor.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Editor widget that will edit float value properties.
 */
class FloatPropertyEditor : public QPropertyEditor
{
   Q_OBJECT

private:
   TEditableReflectionProperty< float >*    m_property;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   FloatPropertyEditor( TEditableReflectionProperty< float >* property );
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
   TEditableReflectionProperty< double >*    m_property;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   DoublePropertyEditor( TEditableReflectionProperty< double >* property );
   ~DoublePropertyEditor();

public slots:
   void valChanged(double val);

private:
   void setupUi();
};

///////////////////////////////////////////////////////////////////////////////
