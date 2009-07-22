#pragma once


///////////////////////////////////////////////////////////////////////////////

class RenderingTargetsPolicy;

///////////////////////////////////////////////////////////////////////////////

class RenderingMechanism
{
private:
   RenderingTargetsPolicy* m_ownRenderTargetsPolicy;

public:
   virtual ~RenderingMechanism();

   virtual void render() = 0;

   inline RenderingTargetsPolicy& getRenderingTargetPolicy() {return *m_ownRenderTargetsPolicy;}

protected:
   RenderingMechanism(RenderingTargetsPolicy* policy);
};

///////////////////////////////////////////////////////////////////////////////
