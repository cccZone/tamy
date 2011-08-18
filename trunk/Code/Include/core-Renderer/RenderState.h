/// @file   core-Renderer/RenderState.h
/// @brief  render state interface
#pragma once


///////////////////////////////////////////////////////////////////////////////

class Renderer;

///////////////////////////////////////////////////////////////////////////////

/**
 * Changes the rendering state of the graphics device.
 */
class RenderState
{
private:
   unsigned long     m_rendereStateId;

public:
   virtual ~RenderState() {}

   /**
    * Compares two states together.
    *
    * @param rhs     other state
    */
   bool operator==( const RenderState& rhs ) const { return m_rendereStateId == rhs.m_rendereStateId; }

   /**
    * Compares two states together.
    *
    * @param rhs     other state
    */
   bool operator<( const RenderState& rhs ) const { return m_rendereStateId < rhs.m_rendereStateId; }

   /**
    * Called before the geometry rendering - sets the render state on the device.
    *
    * @param renderer
    */
   virtual void onPreRender( Renderer& renderer ) = 0;

   /**
    * Called after the geometry rendering- resets the render state.
    *
    * @param renderer
    */
   virtual void onPostRender( Renderer& renderer ) = 0;

protected:
   /**
    * Constructor.
    */
   RenderState() : m_rendereStateId( 0 ) {}

   /**
    * Sets the render state id.
    *
    * @param id
    */
   inline void setRenderStateId( unsigned long id ) { m_rendereStateId = id; }
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
      return *lhs < *rhs;
   }
};

///////////////////////////////////////////////////////////////////////////////
