#ifndef _TYPE_ID_H
#define _TYPE_ID_H

/// @file   core\TypeID.h
/// @brief  utility for deducing names of types.

#include <string>


///////////////////////////////////////////////////////////////////////////////

/**
 * Utility for deducing names of types. 
 * It strips the name of the 'struct'/'class' qualifiers.
 */
template< typename T >
class TypeID
{
private:
   std::string    m_name;
   unsigned int   m_hash;

public:
   /**
    * Constructor for regular types.
    *
    * @param T     type we want to deduce
    */
   TypeID();

   /**
    * Returns the name of the type.
    */
   inline const std::string& name() const;

   /**
    * Returns the hash value for the type.
    */
   inline unsigned int hash() const;

private:
   std::string getSingleTypeName( const std::string& typeName) const;
};

///////////////////////////////////////////////////////////////////////////////

#include "core\TypeID.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _TYPE_ID_H
