#include "core-Renderer\MaterialOperation.h"


///////////////////////////////////////////////////////////////////////////////

MaterialOperation::MaterialOperation(MatOpCode opCode, 
                                     SourceCode arg1, 
                                     SourceCode arg2)
      : m_opCode(opCode),
      m_arg1(arg1),
      m_arg2(arg2)
{
}

///////////////////////////////////////////////////////////////////////////////

MaterialOperation::MaterialOperation(const MaterialOperation& rhs)
      : m_opCode(rhs.m_opCode),
      m_arg1(rhs.m_arg1),
      m_arg2(rhs.m_arg2)
{
}

///////////////////////////////////////////////////////////////////////////////

bool MaterialOperation::operator==(const MaterialOperation& rhs) const
{
   if (m_opCode != rhs.m_opCode) return false;
   if (m_arg1 != rhs.m_arg1) return false;
   if (m_arg2 != rhs.m_arg2) return false;

   return true;
}

///////////////////////////////////////////////////////////////////////////////

bool MaterialOperation::operator!=(const MaterialOperation& rhs) const
{
   return !(*this == rhs);
}

///////////////////////////////////////////////////////////////////////////////
