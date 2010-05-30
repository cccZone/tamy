#ifndef _RENDERER_H
#error "This file can only be included from Renderer.h"
#else

///////////////////////////////////////////////////////////////////////////////

template <typename I>
void Renderer::implement(RendererObject& object)
{
   RendererObjectImpl* impl = create(object);
   if (impl == NULL)
   {
      throw std::invalid_argument("Implementation for this object is not defined");
   }

   object.setImplementation(impl);
   impl->initialize(*this);
}

///////////////////////////////////////////////////////////////////////////////

AttributeSorter& Renderer::getAttributeSorter() const
{
   return *m_sorter;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _RENDERER_H
