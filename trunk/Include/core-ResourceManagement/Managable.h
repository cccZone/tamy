#pragma once


///////////////////////////////////////////////////////////////////////////////

class Managable
{
public:
   virtual ~Managable() {}
};

///////////////////////////////////////////////////////////////////////////////

template<typename T>
class TManagable : public Managable
{
private:
   T* m_object;
   bool m_destroy;

public:
   TManagable(T* object) : m_object(object), m_destroy(true) {}
   TManagable(T& object) : m_object(&object), m_destroy(false) {}

   ~TManagable() 
   {
      if (m_destroy)
      {
         delete m_object; 
      }
      m_object = NULL;
   }

   operator T*()
   {
      return m_object;
   }

   T& operator*()
   {
      return *m_object;
   }

   const T& operator*() const
   {
      return *m_object;
   }
};

///////////////////////////////////////////////////////////////////////////////
