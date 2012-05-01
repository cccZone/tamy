#include "core-Renderer/RenderingPipelineSockets.h"
#include "core-Renderer/RenderingPipelineNode.h"


///////////////////////////////////////////////////////////////////////////////
BEGIN_OBJECT( RPBoolInput );
   PROPERTY( std::string, m_name );
   PROPERTY( RPOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( RPBoolOutput );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPFloatInput );
   PROPERTY( std::string, m_name );
   PROPERTY( RPOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( RPFloatOutput );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPIntInput );
   PROPERTY( std::string, m_name );
   PROPERTY( RPOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( RPIntOutput );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPMatrixInput );
   PROPERTY( std::string, m_name );
   PROPERTY( RPOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( RPMatrixOutput );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPStringInput );
   PROPERTY( std::string, m_name );
   PROPERTY( RPOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( RPStringOutput );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPTextureInput );
   PROPERTY( std::string, m_name );
   PROPERTY( RPOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( RPTextureOutput );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPVec4Input );
   PROPERTY( std::string, m_name );
   PROPERTY( RPOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( RPVec4Output );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPVoidInput );
   PROPERTY( std::string, m_name );
   PROPERTY( RPOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( RPVoidOutput );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////
