#include "core-Renderer\CyclicRenderingTarget.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

CyclicRenderingTarget::CyclicRenderingTarget(const std::string& name)
      : TextureRenderingTarget(name)
{
}

///////////////////////////////////////////////////////////////////////////////

CyclicRenderingTarget::~CyclicRenderingTarget()
{
   for (std::list<TextureRenderingTarget*>::iterator it = m_targets.begin();
        it != m_targets.end(); ++it)
   {
      delete *it;
   }
   m_targets.clear();
}

///////////////////////////////////////////////////////////////////////////////

void CyclicRenderingTarget::add(TextureRenderingTarget* target)
{
   if (target == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a RenderingTarget instance");
   }

   m_targets.push_back(target);
}

///////////////////////////////////////////////////////////////////////////////

void CyclicRenderingTarget::switchBuffer()
{
   TextureRenderingTarget* currTarget = m_targets.front();
   m_targets.pop_front();
   m_targets.push_back(currTarget);
}

///////////////////////////////////////////////////////////////////////////////

void CyclicRenderingTarget::use(unsigned char idx)
{

   m_targets.back()->use(idx);
}

///////////////////////////////////////////////////////////////////////////////

TextureImpl& CyclicRenderingTarget::getImpl()
{
   return m_targets.back()->getImpl();
}

///////////////////////////////////////////////////////////////////////////////
