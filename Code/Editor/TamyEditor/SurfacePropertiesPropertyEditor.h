/// @file   TamyEditor\SurfacePropertiesPropertyEditor.h
/// @brief  material property editor
#pragma once


#include "core\ReflectionProperty.h"
#include "core-Renderer\SurfaceProperties.h"
#include "QPropertyEditor.h"


///////////////////////////////////////////////////////////////////////////////

class ColorFrame;
class QDoubleSpinBox;

///////////////////////////////////////////////////////////////////////////////

/**
 * Editor widget that will edit color properties.
 */
class SurfacePropertiesPropertyEditor : public QPropertyEditor
{
   Q_OBJECT

private:
   TEditableReflectionProperty< SurfaceProperties >*     m_property;

   ColorFrame*                                           m_ambientVal;
   ColorFrame*                                           m_diffuseVal;
   ColorFrame*                                           m_emissiveVal;
   ColorFrame*                                           m_specularVal;
   QDoubleSpinBox*                                       m_powerVal;

public:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   SurfacePropertiesPropertyEditor( TEditableReflectionProperty< SurfaceProperties >* property );
   ~SurfacePropertiesPropertyEditor();

   // -------------------------------------------------------------------------
   // ReflectionPropertyEditor implementation
   // -------------------------------------------------------------------------
   void onPropertyChanged();

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
