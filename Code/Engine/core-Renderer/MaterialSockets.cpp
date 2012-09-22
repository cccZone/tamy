#include "core-Renderer/MaterialSockets.h"
#include "core-Renderer/MaterialNode.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MSFloatInput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( MSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( MSFloatOutput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< MaterialNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MSMatrixInput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( MSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( MSMatrixOutput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< MaterialNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MSTextureInput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( MSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( MSTextureOutput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< MaterialNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MSVec4Input );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( MSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( MSVec4Output );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< MaterialNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MSBoolInput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( MSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( MSBoolOutput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< MaterialNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MSIntInput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( MSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( MSIntOutput );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< MaterialNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////
