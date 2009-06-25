#include "core-ResourceManagement\CompositeElement.h"
#include "core-ResourceManagement\MaterialsParser.h"


///////////////////////////////////////////////////////////////////////////////

CompositeElement::CompositeElement(MaterialsParser& mainParser)
      : MaterialXmlElement(mainParser)
{
}

///////////////////////////////////////////////////////////////////////////////

CompositeElement::~CompositeElement()
{
   for (std::list<MaterialXmlElement*>::iterator it = m_elems.begin();
      it != m_elems.end(); ++it)
   {
      delete *it;
   }
   m_elems.clear();
}

///////////////////////////////////////////////////////////////////////////////

void CompositeElement::addElement(MaterialXmlElement* elem)
{
   m_elems.push_back(elem);
}

///////////////////////////////////////////////////////////////////////////////

bool CompositeElement::parse(TiXmlElement& parent)
{
   for (std::list<MaterialXmlElement*>::iterator it = m_elems.begin();
        it != m_elems.end(); ++it)
   {
      if ((*it)->parse(parent) == false)
      {
         break;
      }
   }

   return true;
}

///////////////////////////////////////////////////////////////////////////////
