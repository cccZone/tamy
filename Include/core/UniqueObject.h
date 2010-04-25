#pragma once

/// @file   core\UniqueObject.h
/// @brief  class each instance of which has a unique id assigned

///////////////////////////////////////////////////////////////////////////////

/**
 * Each instance of this class will have a unique ID assigned.
 */
class UniqueObject
{
private:
   static unsigned int m_nextIndex;

   unsigned int m_index;

public:
   virtual ~UniqueObject() {}

   /** 
    * Returns teh unique id assigned to each instance.
    *
    * @return     unique object id
    */
   unsigned int getIndex() const {return m_index;}
   bool operator<(const UniqueObject& rhs) const {return m_index < rhs.m_index;}

   /** 
    * Operator returns true if both objects have the same id.
    */
   bool operator==(const UniqueObject& rhs) const {return m_index == rhs.m_index;}
   bool operator!=(const UniqueObject& rhs) const {return m_index != rhs.m_index;}

protected:
   UniqueObject();
   UniqueObject(const UniqueObject& rhs);
};

///////////////////////////////////////////////////////////////////////////////
