/// @file   core-Renderer/GeometryEntity.h
/// @brief  entity that allows to embed a geometry shader
#pragma once

#include "core-Renderer/Geometry.h"
#include "core/Observer.h"


///////////////////////////////////////////////////////////////////////////////

class GeometryShader;
class RuntimeDataBuffer;
class GeometryShaderNode;
class GNVertexShader;
enum GraphBuilderOperation;
enum GraphBuilderNodeOperation;

///////////////////////////////////////////////////////////////////////////////

/**
 * Entity that allows to embed a geometry shader.
 */
class GeometryEntity :  public Geometry, 
                        public Observer< GeometryShader, GraphBuilderOperation >,
                        public Observer< GeometryShaderNode, GraphBuilderNodeOperation >
{
   DECLARE_CLASS()

private:
   // static data
   GeometryShader*                        m_geometryShader;

   // runtime data
   RuntimeDataBuffer*                     m_dataBuf;
   GNVertexShader*                        m_vsNode;
   std::vector< GeometryShaderNode* >     m_nodesQueue;

public:
   /**
    * Constructor.
    *
    * @param name
    */
   GeometryEntity( const std::string& name = "" );

   /**
    * Copy constructor.
    *
    * @param rhs
    */
   GeometryEntity( const GeometryEntity& rhs );
   ~GeometryEntity();

   /**
    * Returns the runtime data buffer.
    */
   inline RuntimeDataBuffer& data() const { return *m_dataBuf; }

   /**
    * Sets a new geometry shader this instance should use to render stuff.
    *
    * @param shader
    */
   inline void setGeometryShader( GeometryShader* shader ) { m_geometryShader = shader; }

   // -------------------------------------------------------------------------
   // Geometry implementation
   // -------------------------------------------------------------------------
   RCBindVertexShader* onPreRender( Renderer& renderer );
   void onPostRender( Renderer& renderer );

   // -------------------------------------------------------------------------
   // Observer implementation
   // -------------------------------------------------------------------------
   void update( GeometryShader& subject );
   void update( GeometryShader& subject, const GraphBuilderOperation& msg );
   void update( GeometryShaderNode& subject );
   void update( GeometryShaderNode& subject, const GraphBuilderNodeOperation& msg );

protected:
   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();
   void onPrePropertyChanged( ReflectionProperty& property );
   void onPropertyChanged( ReflectionProperty& property );

   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
   Entity* cloneSelf() const;

private:
   void initializeGeometryShader();
   void deinitializeGeometryShader();
   void attachListeners();
   void detachListeners();
};

///////////////////////////////////////////////////////////////////////////////
