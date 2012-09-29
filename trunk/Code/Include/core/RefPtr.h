/// @file   core/RefPtr.h
/// @brief  reference counting pointer
#ifndef _REF_PTR_H
#define _REF_PTR_H

#include <string>


///////////////////////////////////////////////////////////////////////////////

class ReflectionObject;

///////////////////////////////////////////////////////////////////////////////

/**
 * An object of this class manages the lifetime of the specified
 * ReflectionObject by increasing and decreasing the number of references
 * to it.
 */
class RefPtr
{
protected:
   ReflectionObject*       m_obj;

public:
   /**
    * Constructor.
    *
    * @param obj
    */
   RefPtr( ReflectionObject* obj = NULL );

   /**
    * Copy constructor.
    */
   RefPtr( const RefPtr& rhs );
   virtual ~RefPtr();

   // -------------------------------------------------------------------------
   // Operators
   // -------------------------------------------------------------------------
   RefPtr& operator=( const RefPtr& rhs );
   RefPtr& operator=( ReflectionObject* obj );
   bool operator==( const RefPtr& rhs ) const;
   bool operator==( const ReflectionObject* obj ) const;
   bool operator!=( const RefPtr& rhs ) const;
   bool operator!=( const ReflectionObject* obj ) const;
};

///////////////////////////////////////////////////////////////////////////////

template< typename T >
class TRefPtr : public RefPtr
{
public:
   /**
    * Constructor.
    *
    * @param obj
    */
   TRefPtr( T* obj = NULL );

   /**
    * Copy constructor.
    */
   TRefPtr( const TRefPtr& rhs );
   virtual ~TRefPtr();

   // -------------------------------------------------------------------------
   // Access operators
   // -------------------------------------------------------------------------
   T* operator->()               { return (T*)m_obj; }
   const T* operator->() const   { return (T*)m_obj; }
};

///////////////////////////////////////////////////////////////////////////////

#include "core/RefPtr.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _REF_PTR_H
