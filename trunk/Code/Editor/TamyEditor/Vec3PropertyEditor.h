#pragma once

/// @file   TamyEditor\Vec3PropertyEditor.h
/// @brief  3D vector property editor

#include "core\ReflectionProperty.h"
#include "QPropertyEditor.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * Editor widget that will edit 3D vector properties.
 */
class Vec3PropertyEditor : public QPropertyEditor
{
   Q_OBJECT

private:
   TEditableReflectionProperty< D3DXVECTOR3 >* m_property;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   Vec3PropertyEditor( TEditableReflectionProperty< D3DXVECTOR3 >* property );
   ~Vec3PropertyEditor();

public slots:
   void xValChanged(double val);
   void yValChanged(double val);
   void zValChanged(double val);

private:
   void setupUi();
};

///////////////////////////////////////////////////////////////////////////////
