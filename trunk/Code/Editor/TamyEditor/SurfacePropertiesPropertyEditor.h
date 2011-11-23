/// @file   TamyEditor\SurfacePropertiesPropertyEditor.h
/// @brief  material property editor
#pragma once


#include "core\Property.h"
#include "core-Renderer\SurfaceProperties.h"
#include "QPropertyEditor.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Editor widget that will edit color properties.
 */
class SurfacePropertiesPropertyEditor : public QPropertyEditor
{
   Q_OBJECT

private:
   TEditableProperty< SurfaceProperties >*   m_property;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   SurfacePropertiesPropertyEditor( TEditableProperty< SurfaceProperties >* property );
   ~SurfacePropertiesPropertyEditor();

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
