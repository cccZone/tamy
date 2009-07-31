#pragma once


///////////////////////////////////////////////////////////////////////////////

enum MatOpCode
{
   MOP_DISABLE,
   MOP_SELECT_ARG1,
   MOP_SELECT_ARG2,
   MOP_MULTIPLY,
   MOP_MULTIPLY_2X,
   MOP_MULTIPLY_4X,
   MOP_ADD,
   MOP_ADD_SIGNED,
   MOP_ADD_SIGNED_2X,
   MOP_ADD_SMOOTH,
   MOP_SUBTRACT,
   MOP_MULTIPLY_ADD
};

///////////////////////////////////////////////////////////////////////////////

enum SourceCode
{
   SC_NONE,
   SC_LRP,
   SC_TEXTURE,
   SC_PREV
};

///////////////////////////////////////////////////////////////////////////////

class MaterialOperationImplementation;

///////////////////////////////////////////////////////////////////////////////

class MaterialOperation
{
private:
   MatOpCode m_opCode;
   SourceCode m_arg1;
   SourceCode m_arg2;

public:
   MaterialOperation(MatOpCode opCode, 
                     SourceCode arg1, 
                     SourceCode arg2);
   MaterialOperation(const MaterialOperation& rhs);

   bool operator==(const MaterialOperation& rhs) const;
   bool operator!=(const MaterialOperation& rhs) const;

   MatOpCode getOperationCode() const {return m_opCode;}
   SourceCode getArg1() const {return m_arg1;}
   SourceCode getArg2() const {return m_arg2;}

private:
   void setSource(unsigned char stageIdx, unsigned char argIdx, SourceCode arg);
};

///////////////////////////////////////////////////////////////////////////////
