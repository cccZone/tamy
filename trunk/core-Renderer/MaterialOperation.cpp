#include "core-Renderer\MaterialOperation.h"
#include "core-Renderer\MaterialOperationImplementation.h"


///////////////////////////////////////////////////////////////////////////////

MaterialOperation::MaterialOperation(MaterialOperationImplementation& impl, 
                                     MatOpCode opCode, 
                                     SourceCode arg1, 
                                     SourceCode arg2)
      : m_impl(impl),
      m_opCode(opCode),
      m_arg1(arg1),
      m_arg2(arg2)
{
}

///////////////////////////////////////////////////////////////////////////////

MaterialOperation::MaterialOperation(const MaterialOperation& rhs)
      : m_impl(rhs.m_impl),
      m_opCode(rhs.m_opCode),
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
   if (&m_impl != &(rhs.m_impl)) return false;

   return true;
}

///////////////////////////////////////////////////////////////////////////////

bool MaterialOperation::operator!=(const MaterialOperation& rhs) const
{
   return !(*this == rhs);
}

///////////////////////////////////////////////////////////////////////////////

void MaterialOperation::setForRendering(unsigned char stageIdx)
{
   setSource(stageIdx, 0, m_arg1);
   setSource(stageIdx, 1, m_arg2);

   switch(m_opCode)
   {
   case MOP_DISABLE:
      {
         m_impl.disableStage(stageIdx);
         break;
      }

   case MOP_SELECT_ARG1:
      {
         m_impl.selectArg1(stageIdx);
         break;
      }

   case MOP_SELECT_ARG2:
      {
         m_impl.selectArg2(stageIdx);
         break;
      }

   case MOP_MULTIPLY:
      {
         m_impl.multiply(stageIdx);
         break;
      }

   case MOP_MULTIPLY_2X:
      {
         m_impl.multiply2X(stageIdx);
         break;
      }

   case MOP_MULTIPLY_4X:
      {
         m_impl.multiply4X(stageIdx);
         break;
      }

   case MOP_ADD:
      {
         m_impl.add(stageIdx);
         break;
      }

   case MOP_ADD_SIGNED:
      {
         m_impl.addSigned(stageIdx);
         break;
      }

   case MOP_ADD_SIGNED_2X:
      {
         m_impl.addSigned2X(stageIdx);
         break;
      }

   case MOP_ADD_SMOOTH:
      {
         m_impl.addSmooth(stageIdx);
         break;
      }

   case MOP_SUBTRACT:
      {
         m_impl.subtract(stageIdx);
         break;
      }

   case MOP_MULTIPLY_ADD:
      {
         m_impl.multiplyAdd(stageIdx);
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void MaterialOperation::setSource(unsigned char stageIdx, unsigned char argIdx, SourceCode arg)
{
   switch(arg)
   {
   case SC_LRP:
      {
         m_impl.setLightReflectingProperties(stageIdx, argIdx);
         break;
      }

   case SC_TEXTURE:
      {
         m_impl.setTexture(stageIdx, argIdx);
         break;
      }

   case SC_PREV:
      {
         m_impl.setPreviousStageResults(stageIdx, argIdx);
         break;
      }

   case SC_NONE: /// fallthrough
   default:
      {
         m_impl.disableArgument(stageIdx, argIdx);
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
