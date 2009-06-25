#pragma once

#include "tinyxml\tinyxml.h"


///////////////////////////////////////////////////////////////////////////////

class MaterialsParser;

///////////////////////////////////////////////////////////////////////////////

class MaterialXmlElement
{
protected:
   MaterialsParser& m_mainParser;

public:
   virtual ~MaterialXmlElement() {}

   virtual bool parse(TiXmlElement& parent) = 0;

protected:
   MaterialXmlElement(MaterialsParser& mainParser) 
      : m_mainParser(mainParser) 
   {}
};

///////////////////////////////////////////////////////////////////////////////
