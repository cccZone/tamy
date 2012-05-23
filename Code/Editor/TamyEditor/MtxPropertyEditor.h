#pragma once

/// @file   TamyEditor\MtxPropertyEditor.h
/// @brief  matrix property editor

#include "core\ReflectionProperty.h"
#include "QPropertyEditor.h"
#include "core\Matrix.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Editor widget that will edit matrix properties.
 */
class MtxPropertyEditor : public QPropertyEditor
{
   Q_OBJECT

private:
   TEditableReflectionProperty< Matrix >*    m_property;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   MtxPropertyEditor( TEditableReflectionProperty< Matrix >* property );
   ~MtxPropertyEditor();

public slots:
   void yawValChanged(double val);
   void pitchValChanged(double val);
   void rollValChanged(double val);

   void xValChanged(double val);
   void yValChanged(double val);
   void zValChanged(double val);

private:
   void setupUi();

};

///////////////////////////////////////////////////////////////////////////////
