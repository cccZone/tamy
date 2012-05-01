#include "core-Renderer/GeometrySockets.h"
#include "core-Renderer/GeometryShaderNode.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GSFloatInput );
   PROPERTY( std::string, m_name );
   PROPERTY( GSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( GSFloatOutput );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< GeometryShaderNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GSMatrixInput );
   PROPERTY( std::string, m_name );
   PROPERTY( GSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( GSMatrixOutput );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< GeometryShaderNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GSTextureInput );
   PROPERTY( std::string, m_name );
   PROPERTY( GSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( GSTextureOutput );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< GeometryShaderNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GSVec4Input );
   PROPERTY( std::string, m_name );
   PROPERTY( GSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( GSVec4Output );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< GeometryShaderNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GSBoolInput );
   PROPERTY( std::string, m_name );
   PROPERTY( GSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( GSBoolOutput );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< GeometryShaderNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////
