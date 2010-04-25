#pragma once

#include <iostream>


///////////////////////////////////////////////////////////////////////////////

struct Point
{
   int x;
   int y;
   Point(int _x, int _y) : x(_x), y(_y) {}
   Point() : x(0), y(0) {}

   bool operator==(const Point& rhs) const
   {
      return (x == rhs.x) && (y == rhs.y);
   }

   bool operator!=(const Point& rhs) const
   {
      return !(*this == rhs);
   }

   Point operator-(const Point& rhs) const 
   {
      Point newPt(x - rhs.x, y - rhs.y);
      return newPt;
   }

   Point operator+(const Point& rhs) const 
   {
      Point newPt(x + rhs.x, y + rhs.y);
      return newPt;
   }

   friend std::ostream& operator<<(std::ostream& stream, const Point& rhs)
   {
      stream << "[" << rhs.x << ", " << rhs.y << "]";
      return stream;
   }
};

///////////////////////////////////////////////////////////////////////////////
