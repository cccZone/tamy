#ifndef REFLECTION_PROPERTY_H
#error "This file can only be included from ReflectionProperty.h"
#else

#include "core\ReflectionType.h"


///////////////////////////////////////////////////////////////////////////////

void ReflectionProperty::setParams( const std::string& name, const std::string& label, bool canBeEdited, bool canBeSaved )
{
   m_name = name;
   m_label = label;
   m_canBeEdited = canBeEdited;
   m_canBeSaved = canBeSaved;
}

///////////////////////////////////////////////////////////////////////////////

const std::string& ReflectionProperty::getName() const
{
   return m_name;
}

///////////////////////////////////////////////////////////////////////////////

const std::string& ReflectionProperty::getLabel() const
{
   return ReflectionProperty::m_label;
}

///////////////////////////////////////////////////////////////////////////////

bool ReflectionProperty::canBeEdited() const
{
   return m_canBeEdited;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename T >
TReflectionProperty< T >::TReflectionProperty( ReflectionObject* hostObject, T* val ) 
   : ReflectionProperty( hostObject )
   , m_val( val )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TReflectionProperty< T >::set( void* val )
{
   notifyBeforeChange();
   *m_val = *( reinterpret_cast< T* >( val ) );
   notifyAfterChange();
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void* TReflectionProperty< T >::edit()
{
   return m_val;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
const ReflectionType& TReflectionProperty< T >::getVirtualClass() const
{
   const ReflectionType* type = ReflectionTypesRegistry::getInstance().find< T >();
   ASSERT_MSG( type != NULL, "This property type doesn't exist" );

   return *type;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
const ReflectionType& TReflectionProperty< T >::getPropertyClass() const
{
   const ReflectionType* type = ReflectionTypesRegistry::getInstance().find< T >();
   ASSERT_MSG( type != NULL, "This property type doesn't exist" );

   return *type;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
const ReflectionType& TReflectionProperty< T >::getRTTIClass()
{
   const ReflectionType* type = ReflectionTypesRegistry::getInstance().find< T >();
   ASSERT_MSG( type != NULL, "This property type doesn't exist" );

   return *type;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename T >
TReflectionProperty< T* >::TReflectionProperty( ReflectionObject* hostObject, T** val ) 
   : ReflectionProperty( hostObject )
   , m_val(val)
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TReflectionProperty< T* >::set( void* val )
{
   notifyBeforeChange();
   *m_val = *( reinterpret_cast< T** >( val ) );
   notifyAfterChange();
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void* TReflectionProperty< T* >::edit()
{
   return m_val;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
const ReflectionType& TReflectionProperty< T* >::getVirtualClass() const
{
   if ( *m_val )
   {
      return (*m_val)->getVirtualRTTI();
   }
   else
   {
      const ReflectionType* type = ReflectionTypesRegistry::getInstance().find< T >();
      ASSERT_MSG( type != NULL, "This property type doesn't exist" );

      return *type;
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
const ReflectionType& TReflectionProperty< T* >::getPropertyClass() const
{
   const ReflectionType* type = ReflectionTypesRegistry::getInstance().find< T >();
   ASSERT_MSG( type != NULL, "This property type doesn't exist" );

   return *type;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
const ReflectionType& TReflectionProperty< T* >::getRTTIClass()
{
   const ReflectionType* type = ReflectionTypesRegistry::getInstance().find< T >();
   ASSERT_MSG( type != NULL, "This property type doesn't exist" );

   return *type;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename T >
TReflectionProperty< std::vector< T* > >::TReflectionProperty( ReflectionObject* hostObject, std::vector< T* >* val ) 
   : ReflectionPropertyArray( hostObject )
   , m_val( val )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TReflectionProperty< std::vector< T* > >::set( void* val )
{
   notifyBeforeChange();
   notifyAfterChange();
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void* TReflectionProperty< std::vector< T* > >::edit()
{
   return m_val;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
unsigned int TReflectionProperty< std::vector< T* > >::size() const
{
   return m_val->size();
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
ReflectionObject* TReflectionProperty< std::vector< T* > >::getElement( uint idx )
{
   return ( *m_val )[idx];
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
const ReflectionType& TReflectionProperty< std::vector< T* > >::getVirtualClass() const
{
   return getRTTIClass();
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
const ReflectionType& TReflectionProperty< std::vector< T* > >::getPropertyClass() const
{
   const ReflectionType* type = ReflectionTypesRegistry::getInstance().find< T >();
   ASSERT_MSG( type != NULL, "This property type doesn't exist" );

   return *type;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
const ReflectionType& TReflectionProperty< std::vector< T* > >::getRTTIClass()
{
   const ReflectionType* type = ReflectionTypesRegistry::getInstance().find< T >();
   ASSERT_MSG( type != NULL, "This property type doesn't exist" );

   return *type;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename T >
TEditableReflectionProperty< T >::TEditableReflectionProperty( ReflectionProperty* property )
   : m_property( property )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
TEditableReflectionProperty< T >::~TEditableReflectionProperty()
{
   delete m_property;
   m_property = NULL;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
const std::string& TEditableReflectionProperty< T >::getLabel() const
{
   return m_property->getLabel();
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TEditableReflectionProperty< T >::set( const T& val ) 
{
   m_property->set( ( void* )( &val ) );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
const T& TEditableReflectionProperty< T >::get() const 
{
   void* val = m_property->edit();
   ASSERT_MSG( val != NULL, "Non-pointer properties must be initialized before thay can be edited" );

   T* typedVal = reinterpret_cast< T* >( val );

   return *typedVal;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
T& TEditableReflectionProperty< T >::get() 
{
   void* val = m_property->edit();
   ASSERT_MSG( val != NULL, "Non-pointer properties must be initialized before thay can be edited" );

   T* typedVal = reinterpret_cast< T* >( val );

   return *typedVal;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
const ReflectionType& TEditableReflectionProperty< T >::getType() const
{
   return m_property->getPropertyClass();
}

///////////////////////////////////////////////////////////////////////////////

#endif // REFLECTION_PROPERTY_H