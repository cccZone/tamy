#pragma once


///////////////////////////////////////////////////////////////////////////////

/**
 * The interface modifies the real time value so that it suits our needs
 * (slows it down, curves it around etc.)
 */
class TimeFunction
{
public:
   virtual ~TimeFunction() {}

   virtual float operator()(const float& timeElapsed) const = 0;
};

///////////////////////////////////////////////////////////////////////////////
