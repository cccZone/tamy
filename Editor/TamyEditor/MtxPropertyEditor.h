#pragma once

/// @file   TamyEditor\MtxPropertyEditor.h
/// @brief  matrix property editor

#include "core-Scene\Property.h"
#include "QPropertyEditor.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class PropertiesEditor;

///////////////////////////////////////////////////////////////////////////////

/**
 * Editor widget that will edit matrix properties.
 */
class MtxPropertyEditor : public QPropertyEditor
{
   Q_OBJECT

private:
   TProperty<D3DXMATRIX>& m_property;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   MtxPropertyEditor(TProperty<D3DXMATRIX>& property);

public slots:
   void m11ValChanged(double val);
   void m12ValChanged(double val);
   void m13ValChanged(double val);
   void m14ValChanged(double val);

   void m21ValChanged(double val);
   void m22ValChanged(double val);
   void m23ValChanged(double val);
   void m24ValChanged(double val);

   void m31ValChanged(double val);
   void m32ValChanged(double val);
   void m33ValChanged(double val);
   void m34ValChanged(double val);

   void m41ValChanged(double val);
   void m42ValChanged(double val);
   void m43ValChanged(double val);
   void m44ValChanged(double val);

private:
   void setupUi();
};

///////////////////////////////////////////////////////////////////////////////
