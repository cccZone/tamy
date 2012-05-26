#include "core-Renderer/RenderingPipelineSockets.h"
#include "core-Renderer/RenderingPipelineNode.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBoolInput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( RPOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( RPBoolOutput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPFloatInput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( RPOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( RPFloatOutput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPIntInput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( RPOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( RPIntOutput );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPMatrixInput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( RPOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( RPMatrixOutput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPStringInput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( RPOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( RPStringOutput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPTextureInput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( RPOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( RPTextureOutput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPVec4Input );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( RPOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( RPVec4Output );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPVoidInput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( RPOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( RPVoidOutput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////
