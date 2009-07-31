#pragma once

#include "core-Renderer\Renderable.h"
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

class RenderableMock : public Renderable
{
private:
   std::vector<std::string>& m_log;

public:
   RenderableMock(std::vector<std::string>& log): m_log(log) {}

   void render() {m_log.push_back("Renderable::render");}
};

///////////////////////////////////////////////////////////////////////////////
