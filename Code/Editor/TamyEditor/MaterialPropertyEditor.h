#pragma once

/// @file   TamyEditor\MaterialPropertyEditor.h
/// @brief  material property editor

#include "core\Property.h"
#include "core-Renderer\Material.h"
#include "QPropertyEditor.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Editor widget that will edit color properties.
 */
class MaterialPropertyEditor : public QPropertyEditor
{
   Q_OBJECT

private:
   TEditableProperty< Material >*   m_property;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   MaterialPropertyEditor( TEditableProperty< Material >* property );
   ~MaterialPropertyEditor();

public slots:
   void ambientChanged( const QColor& color );
   void diffuseChanged( const QColor& color );
   void specularChanged( const QColor& color );
   void emissiveChanged( const QColor& color );
   void powerChanged( double val );

private:
   void setupUi();
};

///////////////////////////////////////////////////////////////////////////////
