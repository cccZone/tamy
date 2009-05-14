#pragma once


///////////////////////////////////////////////////////////////////////////////

class MaterialOperationImplementation
{
public:
   virtual ~MaterialOperationImplementation() {}

   // ------------- source settings -------------
   virtual void setLightReflectingProperties(unsigned char stage, unsigned char argIdx) = 0;

   virtual void setTexture(unsigned char stage, unsigned char argIdx) = 0;

   virtual void setPreviousStageResults(unsigned char stage, unsigned char argIdx) = 0;

   virtual void disableArgument(unsigned char stage, unsigned char argIdx) = 0;

   // ---------------- operations ---------------
   virtual void disableStage(unsigned char stage) = 0;

   virtual void selectArg1(unsigned char stage) = 0;

   virtual void selectArg2(unsigned char stage) = 0;

   virtual void multiply(unsigned char stage) = 0;

   virtual void multiply2X(unsigned char stage) = 0;

   virtual void multiply4X(unsigned char stage) = 0;

   virtual void add(unsigned char stage) = 0;

   virtual void addSigned(unsigned char stage) = 0;

   virtual void addSigned2X(unsigned char stage) = 0;

   virtual void addSmooth(unsigned char stage) = 0;

   virtual void subtract(unsigned char stage) = 0;

   virtual void multiplyAdd(unsigned char stage) = 0;

};

///////////////////////////////////////////////////////////////////////////////
