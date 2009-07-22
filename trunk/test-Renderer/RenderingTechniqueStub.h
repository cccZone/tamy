#pragma once

#include "core-Renderer\RenderingTechnique.h"
#include "RenderingTargetsPolicyMock.h"
#include <list>


///////////////////////////////////////////////////////////////////////////////

namespace RegularTests
{
   class RenderingTechniqueStub : public RenderingTechnique
   {
   private:
      static RenderingTargetsPolicyMock s_policy;
      bool m_transparent;

   public:
      RenderingTechniqueStub(bool transparent = false)
         : RenderingTechnique("Stub", s_policy),
         m_transparent(transparent)
      {}

      RenderingTechniqueStub(const std::string& name)
         : RenderingTechnique(name, s_policy),
         m_transparent(false)
      {}

      bool isTransparent() const {return m_transparent;}

   protected:
      unsigned int beginRendering() {return 1;}

      void endRendering() {}

      void beginPass(const unsigned int& passIdx) {}

      void endPass(const unsigned int& passIdx) {}
   };
}

///////////////////////////////////////////////////////////////////////////////

namespace RenderingProcessorTests
{
   class RenderingTechniqueStub : public RenderingTechnique
   {
   private:
      static RegularTests::RenderingTargetsPolicyMock s_policy;
      std::vector<std::string>& m_log;

   public:
      RenderingTechniqueStub(const std::string& name,
                             std::vector<std::string>& log)
         : RenderingTechnique(name, s_policy),
         m_log(log)
      {}


      bool isTransparent() const {return false;}

   protected:
      unsigned int beginRendering() {m_log.push_back(std::string("Set technique ") + getName()); return 1;}

      void endRendering() {}

      void beginPass(const unsigned int& passIdx) {}

      void endPass(const unsigned int& passIdx) {}
   };
}

///////////////////////////////////////////////////////////////////////////////

namespace RenderingTechniqueTests
{
   class RenderingTechniqueStub : public RenderingTechnique
   {
   private:
      unsigned int m_passesCount;
      std::vector<std::string>& m_log;

   public:
      RenderingTechniqueStub(const std::string& name,
                             RenderingTargetsPolicy& policy,
                             unsigned int passesCount,
                             std::vector<std::string>& log)
         : RenderingTechnique(name, policy),
         m_passesCount(passesCount),
         m_log(log)
      {}


      bool isTransparent() const {return false;}

   protected:
      unsigned int beginRendering() 
      {
         m_log.push_back(std::string("RenderingTechnique::beginRendering")); 
         return m_passesCount;
      }

      void endRendering() 
      {
         m_log.push_back(std::string("RenderingTechnique::endRendering")); 
      }

      void beginPass(const unsigned int& passIdx) 
      {
         char tmpStr[128];
         sprintf_s(tmpStr, 128, "RenderingTechnique::beginPass - %d", passIdx);
         m_log.push_back(tmpStr); 
      }

      void endPass(const unsigned int& passIdx) 
      {
         char tmpStr[128];
         sprintf_s(tmpStr, 128, "RenderingTechnique::endPass - %d", passIdx);
         m_log.push_back(tmpStr);
      }
   };
}

///////////////////////////////////////////////////////////////////////////////
