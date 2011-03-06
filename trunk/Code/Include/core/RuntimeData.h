/// @file   core\RuntimeData.h
/// @brief  runtime data buffer
#ifndef _RUNTIME_DATA_H
#define _RUNTIME_DATA_H

#include <map>


///////////////////////////////////////////////////////////////////////////////

class RuntimeDataBuffer;

///////////////////////////////////////////////////////////////////////////////

class IRuntimeVar 
{
protected:
   static unsigned int  s_nextId;

public: 
   virtual ~IRuntimeVar() {}
};

///////////////////////////////////////////////////////////////////////////////

template< typename T >
class TRuntimeVar : public IRuntimeVar
{
private:
   unsigned int         m_id;

public:
   TRuntimeVar();

private:

   /**
      * Returns the variable id.
      */
   inline unsigned int getId() const;

   /**
      * Returns the size of the described type.
      */
   inline unsigned int getTypeSize() const;

   friend class RuntimeDataBuffer;
};

///////////////////////////////////////////////////////////////////////////////

class RuntimeDataBuffer
{
private:
   typedef std::map< unsigned int, unsigned long >     VarsLayout;

private:
   const unsigned long        INITIAL_SIZE;

   VarsLayout                 m_varsLayout;
   char*                      m_buffer;
   unsigned long              m_size;

public:
   RuntimeDataBuffer();
   ~RuntimeDataBuffer();

   /**
    * Registers a new runtime variable.
    *
    * @param var           variable to register
    * @param defaultVal    variable's default value
    */
   template< typename T >
   void registerVar( const typename TRuntimeVar< T >& var, const T& defaultVal = T(0) );

   /**
    * An access operator.
    *
    * @param var     variable data of which we want to access
    */
   template< typename T >
   T& operator[]( const typename TRuntimeVar< T >& var );

   /**
    * An access operator ( const version ).
    *
    * @param var     variable data of which we want to access
    */
   template< typename T >
   const T& operator[]( const typename TRuntimeVar< T >& var ) const;
};

///////////////////////////////////////////////////////////////////////////////

#include "core\RuntimeData.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _RUNTIME_DATA_H
