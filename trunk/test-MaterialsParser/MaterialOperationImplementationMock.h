#pragma once

#include "core-Renderer\MaterialOperationImplementation.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

class MaterialOperationImplementationMock : public MaterialOperationImplementation
{
private:
   std::string m_args[2];
   std::string m_operation;

public:

   // ------------------------------------------------

   void setLightReflectingProperties(unsigned char stage, unsigned char argIdx)
   {
      m_args[argIdx] = "lrp";
   }

   void setTexture(unsigned char stage, unsigned char argIdx)
   {
      m_args[argIdx] = "tex";
   }

   void setPreviousStageResults(unsigned char stage, unsigned char argIdx)
   {
      m_args[argIdx] = "prev";
   }

   void disableArgument(unsigned char stage, unsigned char argIdx)
   {
      m_args[argIdx] = "NULL";
   }

   std::string getSource(unsigned char argIdx)
   {
      return m_args[argIdx];
   }

   // ------------------------------------------------

   void disableStage(unsigned char stage) 
   {
      m_operation = "disable";
   }

   void selectArg1(unsigned char stage)
   {
      m_operation = "selectArg1";
   }

   void selectArg2(unsigned char stage)
   {
      m_operation = "selectArg2";
   }

   void multiply(unsigned char stage) 
   {
      m_operation = "multiply";
   }

   void multiply2X(unsigned char stage)
   {
      m_operation = "multiply2X";
   }

   void multiply4X(unsigned char stage)
   {
      m_operation = "multiply4X";
   }

   void add(unsigned char stage)
   {
      m_operation = "add";
   }

   void addSigned(unsigned char stage)
   {
      m_operation = "addSigned";
   }

   void addSigned2X(unsigned char stage)
   {
      m_operation = "addSigned2X";
   }

   void addSmooth(unsigned char stage) 
   {
      m_operation = "addSmooth";
   }

   void subtract(unsigned char stage)
   {
      m_operation = "subtract";
   }

   void multiplyAdd(unsigned char stage)
   {
      m_operation = "multiplyAdd";
   }

   std::string getOperation()
   {
      return m_operation;
   }
};

///////////////////////////////////////////////////////////////////////////////
