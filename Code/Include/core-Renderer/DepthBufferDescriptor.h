/// @file   core-Renderer/DepthBufferDescriptor.h
/// @brief  a descriptor of a depth buffer used by the rendering pipeline
#pragma once

#include "core/ReflectionObject.h"
#include "core/RuntimeData.h"


///////////////////////////////////////////////////////////////////////////////

class DepthBuffer;
class Renderer;

///////////////////////////////////////////////////////////////////////////////

/**
 * A descriptor of a depth buffer used by the rendering pipeline.
 */
class DepthBufferDescriptor : public ReflectionObject
{
   DECLARE_CLASS()

private:
   std::string                   m_id;
   unsigned int                  m_width;
   unsigned int                  m_height;

   // runtime data
   TRuntimeVar< DepthBuffer* >  m_depthBuffer;

public:
   /**
    * Constructor.
    */
   DepthBufferDescriptor();
   ~DepthBufferDescriptor();

   /**
    * Returns the render buffer's id.
    */
   inline const std::string& getID() const { return m_id; }

   /**
    * Changes the buffer's ID.
    *
    * @param id
    */
   inline void setID( const std::string& id ) { m_id = id; }

   /**
    * Returns buffer's dimensions.
    *
    * @param outWidth
    * @param outHeight
    */
   inline void getSize( unsigned int& outWidth, unsigned int& outHeight ) { outWidth = m_width; outHeight = m_height; }

   /**
    * Changes buffer's dimensions.
    *
    * @param width
    * @param height
    */
   inline void setSize( unsigned int width, unsigned int height ) { m_width = width; m_height = height; }

   /**
    * Initializes the descriptor.
    *
    * @param runtimeData      runtimeDataBuffer
    */
   void initialize( RuntimeDataBuffer& runtimeData ) const;

   /**
    * Deinitializes the descriptor.
    *
    * @param runtimeData      runtimeDataBuffer
    */
   void deinitialize( RuntimeDataBuffer& runtimeData ) const;

   /**
    * Returns a runtime implementation of the depth buffer dedicated to the specific data buffer.
    *
    * @param runtimeData      runtimeDataBuffer
    */
   inline DepthBuffer* getBuffer( RuntimeDataBuffer& runtimeData ) { return runtimeData[m_depthBuffer]; }
};

///////////////////////////////////////////////////////////////////////////////
