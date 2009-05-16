#include "core-ResourceManagement\MultipleElement.h"
#include "core-ResourceManagement\MaterialsParser.h"


///////////////////////////////////////////////////////////////////////////////

MultipleElement::MultipleElement(MaterialsParser& mainParser,
                                 const std::string& tag, 
                                 MaterialXmlElement* elem)
      : MaterialXmlElement(mainParser),
      m_tag(tag),
      m_elem(elem)
{
}

///////////////////////////////////////////////////////////////////////////////

MultipleElement::~MultipleElement()
{
   delete m_elem;
   m_elem = NULL;
}


///////////////////////////////////////////////////////////////////////////////

void MultipleElement::parse(TiXmlElement& parent)
{
   for (TiXmlElement* element = parent.FirstChildElement(m_tag.c_str()); 
        element != NULL;
        element = element->NextSiblingElement(m_tag.c_str()))
   {
      m_elem->parse(*element);
   }
}

///////////////////////////////////////////////////////////////////////////////
