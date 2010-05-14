#pragma once

/// @file   TamyEditor\Vec3PropertyEditor.h
/// @brief  3D vector property editor

#include "core\Property.h"
#include "QPropertyEditor.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class PropertiesEditor;

///////////////////////////////////////////////////////////////////////////////

/**
 * Editor widget that will edit 3D vector properties.
 */
class Vec3PropertyEditor : public QPropertyEditor
{
   Q_OBJECT

private:
   D3DXVECTOR3& m_property;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   Vec3PropertyEditor( D3DXVECTOR3& property, const std::string& label );

public slots:
   void xValChanged(double val);
   void yValChanged(double val);
   void zValChanged(double val);

private:
   void setupUi();
};

///////////////////////////////////////////////////////////////////////////////
