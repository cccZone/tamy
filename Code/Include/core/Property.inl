#ifndef _PROPERTY_H
#error "This file can only be included from Property.h"
#else



///////////////////////////////////////////////////////////////////////////////

template< typename T >
TProperty< T >::TProperty() 
: m_val(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
TProperty< T >::TProperty( T* val, 
                           const std::string& name, 
                           const std::string& label ) 
: m_val(val)
, m_name(name)
, m_label(label)
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
const std::string& TProperty< T >::getName() const 
{
   return m_name;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
const std::string& TProperty< T >::getLabel() const 
{
   return m_label;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TProperty< T >::set( const T& val ) 
{
   *m_val = val;
   notifyAboutChange();
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
const T& TProperty< T >::get() const 
{
   return *m_val;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
T& TProperty< T >::get() 
{
   return *m_val;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TProperty< T >::serialize(Serializer& serializer) 
{ 
   serializer << *m_val; 
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void* TProperty< T >::edit()
{
   return m_val;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
Class TProperty< T >::getVirtualClass() const
{
   return Class( getClassesRegistry().getClassByType< T >() );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
Class TProperty< T >::getRTTIClass()
{
   return Class( getClassesRegistry().getClassByType< T >() );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template <typename T>
TProperty<T*>::TProperty() 
: m_val(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
TProperty< T* >::TProperty( T** val, 
                            const std::string& name, 
                            const std::string& label ) 
: m_val(val)
, m_name(name)
, m_label(label)
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
const std::string& TProperty< T* >::getName() const 
{
   return m_name;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
const std::string& TProperty< T* >::getLabel() const 
{
   return m_label;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TProperty< T* >::set(const T* val) 
{
   *m_val = val;
   notifyAboutChange();
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
const T* TProperty< T* >::get() const 
{
   return *m_val;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
T* TProperty< T* >::get() 
{
   return *m_val;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TProperty< T* >::serialize(Serializer& serializer) 
{ 
   Serializable* newValPtr = dynamic_cast<Serializable*> (*m_val);
   serializer << &newValPtr; 

   if ( newValPtr )
   {
      *m_val = reinterpret_cast<T*> (newValPtr);
   }
   else
   {
      *m_val = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void* TProperty< T* >::edit()
{
   return *m_val;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
Object** TProperty< T* >::editPtr()
{
   return reinterpret_cast< Object** >( m_val );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
Class TProperty< T* >::getVirtualClass() const
{
   if ( *m_val )
   {
      return (*m_val)->getVirtualClass();
   }
   else
   {
      return Class( getClassesRegistry().getClassByType< T >() );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
Class TProperty< T* >::getRTTIClass()
{
   return Class( getClassesRegistry().getClassByType< T >() );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename T >
Class TProperty< std::vector< T* > >::s_class;

template< typename T >
typename TProperty< std::vector< T* > >::RTTITypeInit TProperty< std::vector< T* > >::s_typeRegistrationTool;

///////////////////////////////////////////////////////////////////////////////

template< typename T >
TProperty< std::vector< T* > >::RTTITypeInit::RTTITypeInit()
{
   s_class = Class( getClassesRegistry().defineClass< std::vector< T* > >() );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
TProperty< std::vector< T* > >::TProperty( std::vector< T* >* val, 
                                         const std::string& name, 
                                         const std::string& label ) 
: m_val(val)
, m_name(name)
, m_label(label)
{
   s_typeRegistrationTool.realize();
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
const std::string& TProperty< std::vector< T* > >::getName() const 
{
   return m_name;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
const std::string& TProperty< std::vector< T* > >::getLabel() const 
{
   return m_label;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
unsigned int TProperty< std::vector< T* > >::size() const
{
   return m_val->size();
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
Object* TProperty< std::vector< T* > >::get(unsigned int idx)
{
   return m_val->at(idx);
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TProperty< std::vector< T* > >::serialize(Serializer& serializer) 
{ 
   serializer << *m_val;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void* TProperty< std::vector< T* > >::edit()
{
   return m_val;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
Class TProperty< std::vector< T* > >::getVirtualClass() const
{
   return s_class;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
Class TProperty< std::vector< T* > >::getRTTIClass()
{
   return s_class;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _PROPERTY_H
