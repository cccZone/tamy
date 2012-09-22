/// @file   TamyEditor/MaterialTexturePanel.h
/// @brief  a panel where you can set a texture used by the material instance
#pragma once

#include <QFrame>
#include "core/FilePath.h"
#include "core-Renderer/MaterialInstance.h"
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

class ResourceDropArea;
class DropArea;

///////////////////////////////////////////////////////////////////////////////

/**
 * A panel where you can set a texture used by the material instance.
 */
class MaterialTexturePanel : public QFrame
{
   Q_OBJECT

private:
   MaterialInstance&             m_materialInstance;
   MaterialTextures              m_textureUsage;

   ResourceDropArea*             m_texturePath;
   DropArea*                     m_imageDropArea;
   std::vector< std::string >    m_imagesPaths;

public:
   /**
    * Constructor.
    *
    * @param parentWidget
    * @param usage
    * @param materialInstance
    */
   MaterialTexturePanel( QWidget* parentWidget, MaterialTextures usage, MaterialInstance& materialInstance );

signals:
   /**
    * Signal emitted when the user assigns a different texture.
    */
   void textureChanged( MaterialTextures, const FilePath& );

public slots:
   void onTexturePathChanged( const FilePath& texturePath );
   void onImageSet(); 

private:
   void refreshImage();
};

///////////////////////////////////////////////////////////////////////////////
