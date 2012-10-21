/// @file   core-Renderer/RenderState.h
/// @brief  render state interface
#pragma once

#include "core-MVC/Entity.h"
#include "core/ReflectionType.h"


///////////////////////////////////////////////////////////////////////////////

class Renderer;
class RuntimeDataBuffer;

///////////////////////////////////////////////////////////////////////////////

/**
 * Changes the rendering state of the graphics device.
 */
class RenderState : public Entity
{
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    *
    * @param name
    */
   RenderState( const std::string& name = "" );

   /**
    * Copy constructor.
    */
   RenderState( const RenderState& rhs );
   virtual ~RenderState() {}

   /**
    * Compares two states together.
    *
    * @param rhs     other state
    */
   virtual bool equals( const RenderState& rhs ) const = 0;

   /**
    * Compares two states together.
    *
    * @param rhs     other state
    */
   virtual bool less( const RenderState& rhs ) const = 0;

   /**
    * Called before the geometry rendering - sets the render state on the device.
    *
    * @param renderer
    */
   virtual void onPreRender( Renderer& renderer ) const = 0;

   /**
    * Called after the geometry rendering- resets the render state.
    *
    * @param renderer
    */
   virtual void onPostRender( Renderer& renderer ) const = 0;

   /**
    * Returns the reflection type of the encapsulated render state.
    */
   virtual const ReflectionType& getType() const = 0;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Type specific render state.
 */
template< typename T >
class TRenderState : public RenderState
{
   DECLARE_ALLOCATOR( TRenderState, AM_DEFAULT );

public:
   /**
    * Constructor.
    *
    * @param name
    */
   TRenderState( const std::string& name = "" ) : RenderState( name ) {}

   /**
    * Copy constructor.
    */
   TRenderState( const TRenderState& rhs ) : RenderState( rhs ) {}
   virtual ~TRenderState() {}

   // -------------------------------------------------------------------------
   // RenderStat implementation
   // -------------------------------------------------------------------------
   inline bool equals( const RenderState& rhs ) const
   {
      const ReflectionType& thisType = T::getStaticRTTI();
      const ReflectionType& rhsType = rhs.getType();
      if ( !rhsType.isA( thisType ) )
      {
         return false;
      }

      return onEquals( static_cast< const T& >( rhs ) );
   }

   inline bool less( const RenderState& rhs ) const
   {
      const ReflectionType& thisType = T::getStaticRTTI();
      const ReflectionType& rhsType = rhs.getType();
      if ( !rhsType.isA( thisType ) )
      {
         return this < &rhs;
      }

      return onLess( static_cast< const T& >( rhs ) );
   }

protected:
   const ReflectionType& getType() const
   {
      return T::getStaticRTTI();
   }

   virtual bool onEquals( const T& rhs ) const = 0;
   virtual bool onLess( const T& rhs ) const = 0;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Compares two states.
 */
class StateComparator
{
public:
   inline bool operator()( const RenderState* lhs, const RenderState* rhs ) const
   {
      return lhs->less( *rhs );
   }
};

///////////////////////////////////////////////////////////////////////////////
