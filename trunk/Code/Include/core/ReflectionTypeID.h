/// @file   core\ReflectionTypeID.h
/// @brief  utility for deducing names of types.
#ifndef REFLECTION_TYPE_ID_H
#define REFLECTION_TYPE_ID_H

#include "core\StringUtils.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

/**
 * Utility for deducing names of types.
 */
template< typename T >
struct ReflectionTypeID
{
   std::string    m_name;
   unsigned int   m_id;

   /**
    * Constructor for regular types.
    *
    * @param T     type we want to deduce
    */
   ReflectionTypeID();
};

///////////////////////////////////////////////////////////////////////////////

#ifndef IMPORT_RTTI_REGISTRATIONS
   #define DEFINE_TYPE_ID( Type )
#else
   #define DEFINE_TYPE_ID( Type )                                             \
   template<>                                                                 \
   ReflectionTypeID< Type >::ReflectionTypeID()                               \
   {                                                                          \
      m_name = #Type;                                                         \
      m_id = StringUtils::calculateHash( m_name );                            \
   }                                                                          \
   template<>                                                                 \
   ReflectionTypeID< std::vector< Type > >::ReflectionTypeID()                \
   {                                                                          \
      m_name = "__Vector__"#Type;                                             \
      m_id = StringUtils::calculateHash( m_name );                            \
   }                                                                          \
   template<>                                                                 \
   ReflectionTypeID< std::vector< Type* > >::ReflectionTypeID()               \
   {                                                                          \
      m_name = "__VectorPtr__"#Type;                                          \
      m_id = StringUtils::calculateHash( m_name );                            \
   }
#endif
///////////////////////////////////////////////////////////////////////////////

#include "core\ReflectionTypeID.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // REFLECTION_TYPE_ID_H
