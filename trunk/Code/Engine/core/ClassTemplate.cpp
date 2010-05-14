#include "core\ClassTemplate.h"


///////////////////////////////////////////////////////////////////////////////

ClassTemplate::ClassTemplate( unsigned int handle, const std::string& className, ClassCreator* creator )
: m_name( className )
, m_handle( handle )
, m_creator( creator )
{
}

///////////////////////////////////////////////////////////////////////////////

ClassTemplate::ClassTemplate( const ClassTemplate& rhs )
: m_name( rhs.m_name )
, m_handle( rhs.m_handle )
, m_parents( rhs.m_parents )
, m_creator( rhs.m_creator->clone() )
{
}

///////////////////////////////////////////////////////////////////////////////

ClassTemplate::~ClassTemplate()
{
   delete m_creator;
   m_creator = NULL;
}

///////////////////////////////////////////////////////////////////////////////

bool ClassTemplate::operator==(const ClassTemplate& rhs) const
{
   return m_handle == rhs.m_handle;
}

///////////////////////////////////////////////////////////////////////////////

bool ClassTemplate::operator!=(const ClassTemplate& rhs) const
{
   return !(*this == rhs);
}

///////////////////////////////////////////////////////////////////////////////

bool ClassTemplate::operator<(const ClassTemplate& rhs) const
{
   return m_handle < rhs.m_handle;
}

///////////////////////////////////////////////////////////////////////////////

void ClassTemplate::addParent( const std::string& classType )
{
   m_parents.push_back( classType );
}

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<( std::ostream& out, const ClassTemplate& rhs )
{
   out << rhs.m_name;
   return out;
}

///////////////////////////////////////////////////////////////////////////////
