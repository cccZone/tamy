#ifndef _RENDERER_OBJECT_H
#define _RENDERER_OBJECT_H

/// @file   core-Renderer\RendererObject.h
/// @brief  A base for all renderer objects

#include "core\Class.h"


///////////////////////////////////////////////////////////////////////////////

class RendererObjectImpl;

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a base interface for all objects that depend on particular
 * renderer implementations.
 * Such objects will be created using a bridge pattern - meaning that 
 * they are gonna have a generic family classes for abstraction
 * of the problem, and renderer implementation dependent implementations
 * that realize the abstraction task on the underlying graphics library
 * (DX, OpenGL, etc).
 */
class RendererObject : public RTTIObject
{
public:
   virtual ~RendererObject() {}

   /**
    * This method sets a new implementation for the object.
    *
    * @param impl    object's implementation
    */
   virtual void setImplementation(RendererObjectImpl* impl) = 0;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This intermediate interface is what all actual renderer objects should
 * implement. 
 * It handles the implementation management details, allowing to simply
 * refer to the implementation using the 'impl()' method.
 *
 * Generic parameters specify the exact type of implementation expected
 * (Textures will work with TextureImpl interface, Geometry - with GeometryImpl
 * and so on).
 * Since some implementation must exist - the second generic parameter (NullImpl)
 * specifies the name of a class that is considered a null implementation 
 * of the RendererObjectImplementation type this class is supposed to work with.
 *
 * By default it's equal to the class name we specify as the first parameter.
 * What this means is that the class should be instantiable!!! (can't be abstract
 * and has to have a non-parametrized public constructor).
 */
template <typename Impl, typename NullImpl = Impl>
class TRendererObject : public RendererObject
{
private:
   Impl* m_impl;

public:
   virtual ~TRendererObject();

   // -------------------------------------------------------------------------
   // RendererObject implementation
   // -------------------------------------------------------------------------
   void setImplementation(RendererObjectImpl* impl);

protected:
   /**
    * Constructor.
    */
   TRendererObject();

   /**
    * This method gives access to the implementation.
    * For the internal use of this interface implementations.
    *
    * @return  renderer object's implementation
    */
   Impl& impl();

   /**
    * Const version of 'impl' method.
    *
    * @return  const renderer object's implementation
    */
   const Impl& impl() const;
};

///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer\RendererObject.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _RENDERER_OBJECT_H
