#pragma once

#include "core-ResourceManagement\MaterialXmlElement.h"
#include <list>


///////////////////////////////////////////////////////////////////////////////

class CompositeElement : public MaterialXmlElement
{
private:
   std::list<MaterialXmlElement*> m_elems;

public:
   CompositeElement(MaterialsParser& mainParser);

   ~CompositeElement();

   void addElement(MaterialXmlElement* elem);

   bool parse(TiXmlElement& parent);
};

///////////////////////////////////////////////////////////////////////////////
