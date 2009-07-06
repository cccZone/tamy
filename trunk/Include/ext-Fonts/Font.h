#pragma once

#include <string>


///////////////////////////////////////////////////////////////////////////////

class AbstractGraphicalEntity;

///////////////////////////////////////////////////////////////////////////////

class Font
{
private:
   std::string m_name;

public:
   virtual ~Font() {}

   const std::string& getName() const {return m_name;}

   virtual AbstractGraphicalEntity& getChar(unsigned char c) = 0;

   virtual float getCharWidth(unsigned char c) const = 0;

protected:
   Font(const std::string& name) : m_name(name) {}
};

///////////////////////////////////////////////////////////////////////////////
