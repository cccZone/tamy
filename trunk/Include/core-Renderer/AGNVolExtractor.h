#pragma once



///////////////////////////////////////////////////////////////////////////////

class AbstractGraphicalNode;
struct AABoundingBox;

///////////////////////////////////////////////////////////////////////////////

class AGNVolExtractor
{
public:
   void operator()(AbstractGraphicalNode* elem, AABoundingBox& output) const;
};

///////////////////////////////////////////////////////////////////////////////
