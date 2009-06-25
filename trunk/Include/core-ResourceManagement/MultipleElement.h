#pragma once

#include "core-ResourceManagement\MaterialXmlElement.h"
#include <list>
#include <string>


///////////////////////////////////////////////////////////////////////////////

class MultipleElement : public MaterialXmlElement
{
private:
   std::string m_tag;
   MaterialXmlElement* m_elem;

public:
   MultipleElement(MaterialsParser& mainParser, 
                   const std::string& tag, 
                   MaterialXmlElement* elem);
   ~MultipleElement();

   bool parse(TiXmlElement& parent);
};

///////////////////////////////////////////////////////////////////////////////
