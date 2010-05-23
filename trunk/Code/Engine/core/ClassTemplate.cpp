#include "core\ClassTemplate.h"


///////////////////////////////////////////////////////////////////////////////

ClassTemplate::ClassTemplate( unsigned int handle, const std::string& className )
: m_name( className )
, m_handle( handle )
, m_creator( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

ClassTemplate::ClassTemplate( const ClassTemplate& rhs )
: m_name( rhs.m_name )
, m_handle( rhs.m_handle )
, m_parents( rhs.m_parents )
, m_creator( NULL )
{
   if ( rhs.m_creator )
   {
      m_creator = rhs.m_creator->clone();
   }
   else
   {
      m_creator = NULL; 
   }
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

void ClassTemplate::setCreator( ClassCreator* creator )
{
   delete m_creator;
   m_creator = creator;
}

///////////////////////////////////////////////////////////////////////////////

void* ClassTemplate::instantiate() const 
{ 
   if ( m_creator )
   { 
      return m_creator->create(); 
   } 
   else
   {
      return NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<( std::ostream& out, const ClassTemplate& rhs )
{
   out << rhs.m_name;
   return out;
}

///////////////////////////////////////////////////////////////////////////////
