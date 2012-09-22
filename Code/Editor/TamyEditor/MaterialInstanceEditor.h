/// @file   TamyEditor/MaterialInstanceEditor.h
/// @brief  material instance editor
#pragma once

#include "ui_materialinstanceeditor.h"
#include "ResourceEditor.h"
#include <QListWidget>
#include "core/FilePath.h"


///////////////////////////////////////////////////////////////////////////////

class MaterialInstance;
class QPropertiesView;
class QVBoxLayout;
class ResourceDropArea;
class ColorFrame;
class QDoubleSpinBox;
enum MaterialTextures;

///////////////////////////////////////////////////////////////////////////////

/**
 * Material instance editor.
 */
class MaterialInstanceEditor : public ResourceEditor
{
   Q_OBJECT

private:
   Ui::MaterialInstanceEditor                m_ui;

   MaterialInstance&                         m_materialInstance;

   ResourceDropArea*                         m_materialRendererPath;
   ColorFrame*                               m_ambientColor;
   ColorFrame*                               m_diffuseColor;
   ColorFrame*                               m_specularColor;
   ColorFrame*                               m_emissiveColor;
   QDoubleSpinBox*                           m_specularPower;

public:
   /**
    * Constructor.
    *
    * @param materialLayout      material layout to be edited
    */
   MaterialInstanceEditor( MaterialInstance& materialInstance );
   ~MaterialInstanceEditor();

   // -------------------------------------------------------------------------
   // ResourceEditor implementation
   // -------------------------------------------------------------------------
   void onInitialize();
   void onDeinitialize( bool saveProgress );

public slots:
   void save();
   void onMaterialRendererChanged( const FilePath& materialRendererPath );
   void onAmbientColorChanged( const QColor& color );
   void onDiffuseColorChanged( const QColor& color );
   void onEmissiveColorChanged( const QColor& color );
   void onSpecularColorChanged( const QColor& color );
   void onSpecularPowerChanged( double specularPower );
   void onTextureChanged( MaterialTextures textureUsage, const FilePath& texturePath );
};

///////////////////////////////////////////////////////////////////////////////
