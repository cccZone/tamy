#include "core-Renderer/GeometrySockets.h"
#include "core-Renderer/GeometryShaderNode.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GSFloatInput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( GSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( GSFloatOutput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< GeometryShaderNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GSMatrixInput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( GSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( GSMatrixOutput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< GeometryShaderNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GSTextureInput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( GSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( GSTextureOutput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< GeometryShaderNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GSVec4Input );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( GSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( GSVec4Output );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< GeometryShaderNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GSBoolInput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( GSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( GSBoolOutput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< GeometryShaderNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////
