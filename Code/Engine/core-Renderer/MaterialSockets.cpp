#include "core-Renderer/MaterialSockets.h"
#include "core-Renderer/MaterialNode.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MSFloatInput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( MSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( MSFloatOutput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< MaterialNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MSMatrixInput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( MSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( MSMatrixOutput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< MaterialNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MSTextureInput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( MSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( MSTextureOutput, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< MaterialNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MSVec4Input, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( MSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( MSVec4Output, Object );
   PROPERTY( std::string, m_name );
   PROPERTY( std::vector< MaterialNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MSBoolInput, Object );
PROPERTY( std::string, m_name );
PROPERTY( MSOutput*, m_connectedOutput );
END_OBJECT();

BEGIN_OBJECT( MSBoolOutput, Object );
PROPERTY( std::string, m_name );
PROPERTY( std::vector< MaterialNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////
