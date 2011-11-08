/// @file   core\Casts.h
/// @brief  RTTI casts definitions
#pragma once


///////////////////////////////////////////////////////////////////////////////

class RTTIObject;

///////////////////////////////////////////////////////////////////////////////

/**
 * Casts an object from one type to another. The types need to share a common hierarchy.
 */
template< typename T >
T* DynamicCast( RTTIObject* obj ) 
{
   if ( !obj )
   {
      return NULL;
   }

   if ( obj->isA< T >() )
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
