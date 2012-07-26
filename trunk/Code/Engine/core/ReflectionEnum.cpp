#include "core/ReflectionEnum.h"


///////////////////////////////////////////////////////////////////////////////

ReflectionEnum::ReflectionEnum( const std::string& typeName )
   : ReflectionType( typeName )
{
}

///////////////////////////////////////////////////////////////////////////////

bool ReflectionEnum::isA( const ReflectionType& referenceType ) const
{
   ReflectionTypeID< ReflectionEnum > id;
   return referenceType.m_id == id.m_id;
}

///////////////////////////////////////////////////////////////////////////////