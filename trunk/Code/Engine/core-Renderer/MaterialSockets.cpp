#include "core-Renderer/MaterialSockets.h"
#include "core-Renderer/MaterialNode.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MSFloatInput );
   PROPERTY( std::string, m_name );
   PROPERTY( MSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( MSFloatOutput );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< MaterialNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MSMatrixInput );
   PROPERTY( std::string, m_name );
   PROPERTY( MSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( MSMatrixOutput );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< MaterialNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MSTextureInput );
   PROPERTY( std::string, m_name );
   PROPERTY( MSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( MSTextureOutput );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< MaterialNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MSVec4Input );
   PROPERTY( std::string, m_name );
   PROPERTY( MSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( MSVec4Output );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< MaterialNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MSBoolInput );
PROPERTY( std::string, m_name );
PROPERTY( MSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( MSBoolOutput );
PROPERTY( std::string, m_name );
PROPERTY( std::vector< MaterialNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////
