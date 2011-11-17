/// @file   TamyEditor/TextureEditor.h
/// @brief  texture resource editor
#pragma once

#include "ResourceEditor.h"
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

class Texture;
class DropArea;

///////////////////////////////////////////////////////////////////////////////

class TextureEditor : public ResourceEditor
{
   Q_OBJECT 

private:
   Texture&                      m_texture;

   DropArea*                     m_image;
   std::vector< std::string >    m_imagePaths;

public:
   /**
    * Constructor.
    *
    * @param texture      texture to be edited
    */
   TextureEditor( Texture& texture );
   ~TextureEditor();

public slots:
   void onTextureSet();
   void saveTexture();

protected:
   // -------------------------------------------------------------------------
   // ResourceEditor implementation
   // -------------------------------------------------------------------------
   void onInitialize();
   void onDeinitialize( bool saveProgress );

private:
   void refreshImage();
};

///////////////////////////////////////////////////////////////////////////////
