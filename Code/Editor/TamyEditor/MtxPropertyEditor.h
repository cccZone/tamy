#pragma once

/// @file   TamyEditor\MtxPropertyEditor.h
/// @brief  matrix property editor

#include "core\Property.h"
#include "QPropertyEditor.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * Editor widget that will edit matrix properties.
 */
class MtxPropertyEditor : public QPropertyEditor
{
   Q_OBJECT

private:
   TEditableProperty< D3DXMATRIX >*    m_property;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   MtxPropertyEditor( TEditableProperty< D3DXMATRIX >* property );
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
