#pragma once

#include "core-AI\Agent.h"
#include "core-AI\State.h"
#include <string>
#include "Enemy.h"


///////////////////////////////////////////////////////////////////////////////

class MessageDispatcher;

///////////////////////////////////////////////////////////////////////////////

class Rambo : public Agent<Rambo>
{
private:
   class LookForEnemy : public State<Rambo>
   {
   public:
      LookForEnemy(Rambo& agent) : State(agent) {}

      void onEnter()
      {
         m_agent.setAction("looking for an ass to kick");
      }

      void onUpdate() 
      {
         if (m_agent.isEnemyInSight())
         {
            m_agent.kill();
         }
         else if (m_agent.getHealth() < m_agent.getMaxHealth())
         {
            m_agent.heal();
         }
      }

      void onExit() {}
   };

   class Kill : public State<Rambo>
   {
   public:
      Kill(Rambo& agent) 
            : State(agent)
      {}

      void onEnter()
      {
         m_agent.setAction("kickin' ass");
      }

      void onUpdate() 
      {
         if (m_agent.isEnemyInSight())
         {
            m_agent.engageEnemy();
         }
         else
         {
           m_agent.lookForEnemy();
         }
      }

      void onExit() {}
   };

   class Heal : public State<Rambo>
   {

   public:
      Heal(Rambo& agent) : State(agent) {}

      void onEnter()
      {
         m_agent.setAction("healing");
      }

      void onUpdate() 
      {
         m_agent.setHealth(m_agent.getHealth() + 1);

         if (m_agent.getHealth() == m_agent.getMaxHealth())
         {
            m_agent.lookForEnemy();
         }
      }

      void onExit() {}
   };

private:
   int m_health;
   std::string m_action;
   Enemy* m_enemySpotted;

   State<Rambo>* m_lookForEnemy;
   State<Rambo>* m_kill;
   State<Rambo>* m_heal;

public:
   Rambo() 
      : Agent("rambo"), m_health(10), m_enemySpotted(NULL)
   {
      m_lookForEnemy = new LookForEnemy(*this);
      m_kill = new Kill(*this);
      m_heal = new Heal(*this);

      lookForEnemy();
   }

   ~Rambo()
   {
      delete m_lookForEnemy;
      m_lookForEnemy = NULL;

      delete m_kill;
      m_kill = NULL;

      delete m_heal;
      m_heal = NULL;
   }

   int getMaxHealth() const {return 10;}
   int getHealth() const {return m_health;}

   void heal() 
   {
      fsm().setState(*m_heal);
   }

   void lookForEnemy() 
   {
      fsm().setState(*m_lookForEnemy);
   }

   void kill() 
   {
      fsm().setState(*m_kill);
   }

   void seesEnemy(Enemy& agent)
   {
      m_enemySpotted = &agent;
   }

   void receiveWound(int healtPointsTaken)
   {
      setHealth(m_health - healtPointsTaken);
   }

   const std::string& getAction() const {return m_action;}

private:
   void setAction(const std::string& action) {m_action = action;}

   void setHealth(int val)
   {
      m_health = val;
      if (m_health < 0) {m_health = 0;}
      else if (m_health > getMaxHealth()) {m_health = getMaxHealth();}
   }

   bool isEnemyInSight() 
   {
      if ((m_enemySpotted != NULL) && (m_enemySpotted->getHealth() == 0))
      {
         m_enemySpotted = NULL;
      }

      return m_enemySpotted != NULL;
   }

   void engageEnemy();
};

///////////////////////////////////////////////////////////////////////////////
