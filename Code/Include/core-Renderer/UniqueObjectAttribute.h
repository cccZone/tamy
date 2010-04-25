#pragma once

/// @file   core-Renderer\UniqueObjectAttribute.h
/// @brief  attribute sorter attribute that sorts unique objects

#include "core-Renderer\EffectAttribute.h"
#include "core\UniqueObject.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Attribute sorter attribute that sorts by the unique id assigned to each
 * unique object.
 */
class UniqueObjectAttribute : public EffectAttribute
{
private:
   UniqueObject& m_object;

public:
   UniqueObjectAttribute(UniqueObject& object)
   : m_object(object)
   {}

   inline bool operator<(const UniqueObjectAttribute& rhs) const
   {
      return (m_object < rhs.m_object);
   }
};

///////////////////////////////////////////////////////////////////////////////
