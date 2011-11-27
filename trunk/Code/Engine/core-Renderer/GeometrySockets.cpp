#include "core-Renderer/GeometrySockets.h"
#include "core-Renderer/GeometryShaderNode.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GSFloatInput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( GSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( GSFloatOutput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< GeometryShaderNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GSMatrixInput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( GSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( GSMatrixOutput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< GeometryShaderNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GSTextureInput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( GSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( GSTextureOutput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< GeometryShaderNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GSVec4Input, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( GSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( GSVec4Output, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< GeometryShaderNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GSBoolInput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( GSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( GSBoolOutput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< GeometryShaderNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////
