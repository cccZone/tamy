#pragma once


///////////////////////////////////////////////////////////////////////////////

class RenderingTarget
{
public:

   /**
    * This method sets the target as the rendering target.
    *
    * @param idx - since there can be multiple rendering targets
    *              set as once, this param specifies the index which
    *              we assign to this particular one at the time
    */
   virtual void use(unsigned char idx) = 0;
};

///////////////////////////////////////////////////////////////////////////////

class NullRenderingTarget : public RenderingTarget
{
public:
   void use(unsigned char idx) {}
};

///////////////////////////////////////////////////////////////////////////////

