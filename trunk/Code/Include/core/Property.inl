#ifndef _PROPERTY_H
#error "This file can only be included from Property.h"
#else


///////////////////////////////////////////////////////////////////////////////

template <typename T>
TProperty<T>::TProperty() 
: m_val(NULL)
{
   REALIZE_TEMPLATE_RTTI();
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
TProperty<T>::TProperty(T* val, 
                        const std::string& name, 
                        const std::string& label) 
: m_val(val)
, m_name(name)
, m_label(label)
{
   REALIZE_TEMPLATE_RTTI();
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
const std::string& TProperty<T>::getName() const 
{
   return m_name;
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
const std::string& TProperty<T>::getLabel() const 
{
   return m_label;
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
void TProperty<T>::set(const T& val) 
{
   *m_val = val;
   notifyAboutChange();
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
const T& TProperty<T>::get() const 
{
   return *m_val;
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
T& TProperty<T>::get() 
{
   return *m_val;
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
void TProperty<T>::serialize(Serializer& serializer) 
{ 
   serializer << *m_val; 
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template <typename T>
TProperty<T*>::TProperty() 
: m_val(NULL)
{
   REALIZE_TEMPLATE_RTTI();
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
TProperty<T*>::TProperty(T** val, 
                         const std::string& name, 
                         const std::string& label) 
: m_val(val)
, m_name(name)
, m_label(label)
{
   REALIZE_TEMPLATE_RTTI();
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
const std::string& TProperty<T*>::getName() const 
{
   return m_name;
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
const std::string& TProperty<T*>::getLabel() const 
{
   return m_label;
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
void TProperty<T*>::set(const T* val) 
{
   *m_val = val;
   notifyAboutChange();
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
const T* TProperty<T*>::get() const 
{
   return *m_val;
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
T* TProperty<T*>::get() 
{
   return *m_val;
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
void TProperty<T*>::serialize(Serializer& serializer) 
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
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template <typename T>
TProperty< std::vector<T*> >::TProperty(std::vector<T*>* val, 
                         const std::string& name, 
                         const std::string& label) 
: m_val(val)
, m_name(name)
, m_label(label)
{}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
const std::string& TProperty< std::vector<T*> >::getName() const 
{
   return m_name;
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
const std::string& TProperty< std::vector<T*> >::getLabel() const 
{
   return m_label;
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
unsigned int TProperty< std::vector<T*> >::size() const
{
   return m_val->size();
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
Object* TProperty< std::vector<T*> >::get(unsigned int idx)
{
   return m_val->at(idx);
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
void TProperty< std::vector<T*> >::serialize(Serializer& serializer) 
{ 
   serializer << *m_val;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _PROPERTY_H
