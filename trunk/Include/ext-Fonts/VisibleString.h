#pragma once

#include "core\Node.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

class Font;
struct Color;
class Material;
class RenderingTechniqueReplacer;

///////////////////////////////////////////////////////////////////////////////

class VisibleString : public Node
{
private:
   Font& m_font;
   Material* m_material;
   RenderingTechniqueReplacer* m_matReplacer;

public:
   VisibleString(Font& font);
   ~VisibleString();

   void setText(const char* text);

   void setColor(const Color& color);

   /**
    * Returns the instance of material used by this string
    */
   Material& getMaterial();

protected:
   void onAccept(NodeVisitor& visitor);
};

///////////////////////////////////////////////////////////////////////////////
