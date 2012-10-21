#ifndef _LINE_SEGMENTS_H
#error "This file can only be included from LineSegments.h"
#else


///////////////////////////////////////////////////////////////////////////////

LineSegment::LineSegment()
   : start( Quad_0 )
   , end( Quad_0 )
{
}

///////////////////////////////////////////////////////////////////////////////

LineSegment::LineSegment( const Vector& _start, const Vector& _end, const Color& _color )
   : start( _start )
   , end( _end )
   , color( _color )
{
}

///////////////////////////////////////////////////////////////////////////////

bool LineSegment::operator==( const LineSegment& rhs ) const
{
   Vector startDiff;
   startDiff.setSub( start, rhs.start );
   Vector endDiff;
   endDiff.setSub( end, rhs.end );


   return ( ( startDiff.lengthSq() < Float_1e_3 ) && (endDiff.lengthSq() < Float_1e_3 ) );
}

///////////////////////////////////////////////////////////////////////////////

bool LineSegment::operator!=(const LineSegment& rhs) const
{
   return !(*this == rhs);
}

///////////////////////////////////////////////////////////////////////////////

#endif // _LINE_SEGMENTS_H
