#pragma once

#include "core-Renderer\MaterialOperationImplementation.h"
#include <string>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class MaterialOperationImplementationMock : public MaterialOperationImplementation
{
private:
   std::string m_args[2];
   std::vector<std::string> m_operation;

public:

   void clear()
   {
      m_operation.clear();
   }

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
      m_operation.push_back("disable");
   }

   void selectArg1(unsigned char stage)
   {
      m_operation.push_back("selectArg1");
   }

   void selectArg2(unsigned char stage)
   {
      m_operation.push_back("selectArg2");
   }

   void multiply(unsigned char stage) 
   {
      m_operation.push_back("multiply");
   }

   void multiply2X(unsigned char stage)
   {
      m_operation.push_back("multiply2X");
   }

   void multiply4X(unsigned char stage)
   {
      m_operation.push_back("multiply4X");
   }

   void add(unsigned char stage)
   {
      m_operation.push_back("add");
   }

   void addSigned(unsigned char stage)
   {
      m_operation.push_back("addSigned");
   }

   void addSigned2X(unsigned char stage)
   {
      m_operation.push_back("addSigned2X");
   }

   void addSmooth(unsigned char stage) 
   {
      m_operation.push_back("addSmooth");
   }

   void subtract(unsigned char stage)
   {
      m_operation.push_back("subtract");
   }

   void multiplyAdd(unsigned char stage)
   {
      m_operation.push_back("multiplyAdd");
   }

   std::string getOperation(unsigned int idx = 0)
   {
      return m_operation.at(idx);
   }

   int getOperationsCount() const
   {
      return m_operation.size();
   }
};

///////////////////////////////////////////////////////////////////////////////
