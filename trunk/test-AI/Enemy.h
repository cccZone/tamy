#pragma once

#include "core-AI\Agent.h"
#include "core-AI\MessageDispatcher.h"


///////////////////////////////////////////////////////////////////////////////

class Enemy : public Agent<Enemy>
{
private:
   int m_health;

public:
   Enemy() : Agent("enemy"), m_health(2) 
   {}

   int getHealth() const {return m_health;}

   void reduceHealth(int amount)
   {
      m_health -= amount;
      if (m_health < 0) {m_health = 0;}
   }

   void revive() {m_health = 2;}

   void startShooting(Messagable& enemy);
   void attack(Messagable& enemy);
};

///////////////////////////////////////////////////////////////////////////////
