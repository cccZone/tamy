#pragma once

#include "core-Renderer\Material.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

namespace RegularTests
{
   class RenderingAttributeStub : public RenderingAttribute
   {
   private:
      bool m_transparent;

   public:
      RenderingAttributeStub(bool transparent = false)
         : RenderingAttribute("Stub"),
         m_transparent(transparent)
      {}

      RenderingAttributeStub(const std::string& name)
         : RenderingAttribute(name),
         m_transparent(false)
      {}

      bool isTransparent() const {return m_transparent;}
   };
}

///////////////////////////////////////////////////////////////////////////////

namespace RenderingProcessorTests
{
   class RenderingAttributeStub : public RenderingAttribute
   {
   private:
      std::vector<std::string>& m_log;

   public:
      RenderingAttributeStub(const std::string& name,
                             std::vector<std::string>& log)
         : RenderingAttribute(name),
         m_log(log)
      {}


      bool isTransparent() const {return false;}
   };
}

///////////////////////////////////////////////////////////////////////////////

namespace MaterialTests
{
   class RenderingAttributeStub : public RenderingAttribute
   {
   private:
      unsigned int m_passesCount;
      std::vector<std::string>& m_log;

   public:
      RenderingAttributeStub(const std::string& name,
                             RenderingTargetsPolicy& policy,
                             unsigned int passesCount,
                             std::vector<std::string>& log)
         : RenderingAttribute(name),
         m_passesCount(passesCount),
         m_log(log)
      {}


      bool isTransparent() const {return false;}
   };
}

///////////////////////////////////////////////////////////////////////////////
