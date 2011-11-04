/// @file   core\Casts.h
/// @brief  RTTI casts definitions
#pragma once


///////////////////////////////////////////////////////////////////////////////

/**
 * Casts an object from one type to another. The types need to share a common hierarchy.
 */
template< typename T >
T* DynamicCast( Object* obj ) 
{
   if ( !obj )
   {
      return NULL;
   }

   if ( obj->getVirtualClass().isA( T::getRTTIClass() ) )
   {
      T* castObj = static_cast< T* >( obj );
      return castObj;
   }
   else
   {
      return NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////
