#ifndef _RENDERER_OBJECT_H
#error "This file can only be included from RendererObject.inl"
#else

#include <string>


///////////////////////////////////////////////////////////////////////////////

template <typename Impl, typename NullImpl>
TRendererObject<Impl, NullImpl>::TRendererObject() 
: m_impl(new NullImpl()) 
{}

///////////////////////////////////////////////////////////////////////////////

template <typename Impl, typename NullImpl>
TRendererObject<Impl, NullImpl>::~TRendererObject() 
{
   delete m_impl; m_impl = NULL;
}

///////////////////////////////////////////////////////////////////////////////

template <typename Impl, typename NullImpl>
void TRendererObject<Impl, NullImpl>::setImplementation(RendererObjectImpl* impl)
{
   delete m_impl;

   if (impl == NULL)
   {
      m_impl = new NullImpl();
   }
   else
   {
      Impl* typedImpl = dynamic_cast<Impl*> (impl);
      if (typedImpl == NULL)
      {
         throw std::runtime_error("Invalid renderer object implementation type");
      }

      m_impl = typedImpl;
   }
}

///////////////////////////////////////////////////////////////////////////////

template <typename Impl, typename NullImpl>
Impl& TRendererObject<Impl, NullImpl>::impl() 
{
   return *m_impl;
}

///////////////////////////////////////////////////////////////////////////////

template <typename Impl, typename NullImpl>
const Impl& TRendererObject<Impl, NullImpl>::impl() const
{
   return *m_impl;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _RENDERER_OBJECT_H
