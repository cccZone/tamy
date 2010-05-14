#ifndef _COMPONENT_H
#error "This file can only be included from Component.h"
#else


///////////////////////////////////////////////////////////////////////////////

template< typename Manager >
void Component< Manager >::addService( void* service )
{
   if ( service )
   {
      m_hostedServices.insert( service );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename Manager >
void Component< Manager >::removeService( void* service )
{
   if ( service )
   {
      m_hostedServices.erase( service );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename Manager >
bool Component< Manager >::isHost( void* service ) const
{
   return m_hostedServices.find( service ) != m_hostedServices.end();
}

///////////////////////////////////////////////////////////////////////////////

#endif // _COMPONENT_H
