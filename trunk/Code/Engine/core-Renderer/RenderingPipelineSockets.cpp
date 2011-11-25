#include "core-Renderer/RenderingPipelineSockets.h"
#include "core-Renderer/RenderingPipelineNode.h"


///////////////////////////////////////////////////////////////////////////////
BEGIN_OBJECT( RPBoolInput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( RPOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( RPBoolOutput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPFloatInput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( RPOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( RPFloatOutput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPIntInput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( RPOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( RPIntOutput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPMatrixInput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( RPOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( RPMatrixOutput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPStringInput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( RPOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( RPStringOutput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPTextureInput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( RPOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( RPTextureOutput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPVec4Input, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( RPOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( RPVec4Output, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPVoidInput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( RPOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( RPVoidOutput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////
