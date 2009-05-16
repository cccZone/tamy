#pragma once

#include <string>


///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents a texture of a graphicla object
 */
class Texture
{
private:
   std::string m_name;

public:
   Texture(const std::string& name);
   virtual ~Texture() {}

   const std::string& getName() const {return m_name;}

   bool operator==(const Texture& rhs) const;
   bool operator!=(const Texture& rhs) const;

   /**
    * This method is engine implementation specific
    * and is supposed to set the texture
    * as an active one with which the objects will be rendered
    */
   virtual void setForRendering(unsigned char stageIdx) = 0;
};

///////////////////////////////////////////////////////////////////////////////
