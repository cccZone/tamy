/// @file   core-Renderer/RenderState.h
/// @brief  render state interface
#pragma once


///////////////////////////////////////////////////////////////////////////////

class Renderer;
class RuntimeDataBuffer;

///////////////////////////////////////////////////////////////////////////////

/**
 * Changes the rendering state of the graphics device.
 */
class RenderState
{
public:
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
    * @param data       runtime data buffer
    */
   virtual void onPreRender( Renderer& renderer, RuntimeDataBuffer& data ) const = 0;

   /**
    * Called after the geometry rendering- resets the render state.
    *
    * @param renderer
    * @param data       runtime data buffer
    */
   virtual void onPostRender( Renderer& renderer, RuntimeDataBuffer& data ) const = 0;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Type specific render state.
 */
template< typename T >
class TRenderState : public RenderState
{
public:
   // -------------------------------------------------------------------------
   // RenderStat implementation
   // -------------------------------------------------------------------------
   inline bool equals( const RenderState& rhs ) const
   {
      const T* typedRhs = dynamic_cast< const T* >( &rhs );
      if ( typedRhs == NULL )
      {
         return false;
      }

      return onEquals( static_cast< const T& >( rhs ) );
   }

   inline bool less( const RenderState& rhs ) const
   {
      const T* typedRhs = dynamic_cast< const T* >( &rhs );
      if ( typedRhs == NULL )
      {
         return this < &rhs;
      }

      return onLess( static_cast< const T& >( rhs ) );
   }
protected:
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
