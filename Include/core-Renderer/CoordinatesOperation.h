#pragma once


///////////////////////////////////////////////////////////////////////////////

enum CoordsOpCode
{
   CC_WRAP,
   CC_MIRROR,
   CC_CLAMP,
   CC_BORDER,
   CC_MIRRORONCE
};

///////////////////////////////////////////////////////////////////////////////

class CoordinatesOperation
{
private:
   CoordsOpCode m_opCode;

public:
   CoordinatesOperation(CoordsOpCode opCode);
   CoordinatesOperation(const CoordinatesOperation& rhs);
   ~CoordinatesOperation();

   bool operator==(const CoordinatesOperation& rhs) const;
   bool operator!=(const CoordinatesOperation& rhs) const;

   inline const CoordsOpCode& getCode() const {return m_opCode;}
};

///////////////////////////////////////////////////////////////////////////////
