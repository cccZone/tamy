#pragma once

#include "core-Renderer\TextureRenderingTarget.h"
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

namespace RendereringPipelineTests
{
   
   template<>
   class TTextureImplDestructor<int>
   {
   public:
      void destroy(int* tex) {}
   };

   class TextureRenderingTargetMock : public TextureRenderingTarget
   {
   private:
      std::string m_id;
      std::vector<std::string>& m_seqLog;
      TextureImpl* m_impl;

   public:
      TextureRenderingTargetMock(const std::string& id,
                                 std::vector<std::string>& seqLog) 
         : TextureRenderingTarget(id), 
         m_id(id), 
         m_seqLog(seqLog),
         m_impl(new TTextureImpl<int>())
      {}

      ~TextureRenderingTargetMock() 
      {
         delete m_impl;
         m_impl = NULL;
      }

      void use(unsigned char idx) 
      {
         char tmpStr[1024];
         sprintf_s(tmpStr, 1024, "%s - %d", m_id.c_str(), idx);
         m_seqLog.push_back(tmpStr);
      }

      TextureImpl& getImpl() {return *m_impl;}
   };
}

///////////////////////////////////////////////////////////////////////////////
