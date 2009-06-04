#pragma once

#include "core-AI\Agent.h"
#include <string>



///////////////////////////////////////////////////////////////////////////////

class Grunt : public Agent<DemolitionMan>
{
private:
   std::string m_action;

public:
   Grunt() : Agent("grunt", true) {}

   void setAction(const std::string& action) {m_action = action;}
   const std::string& getAction() const {return m_action;}
};

///////////////////////////////////////////////////////////////////////////////
