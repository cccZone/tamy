#pragma once

#include "core-AI\Agent.h"
#include "core-AI\State.h"
#include <string>
#include <cassert>
#include "ExplosiveCharge.h"


///////////////////////////////////////////////////////////////////////////////

class ExplosiveCharge;

///////////////////////////////////////////////////////////////////////////////

class DemolitionMan : public Agent<DemolitionMan>
{
private:
   class Idle : public State<DemolitionMan>
   {
   public:
      Idle(DemolitionMan& agent) : State(agent) {}

      void onEnter() {m_agent.setAction("");}
      void onUpdate() {}
      void onExit() {}
   };

   class DoingHisJob : public State<DemolitionMan>
   {
   public:
      DoingHisJob(DemolitionMan& agent) : State(agent) {}

      void onEnter() {m_agent.setAction("arming an explosive charge");}
      void onUpdate() 
      {
         if (m_agent.hasChargeToDisarm())
         {
            m_agent.armCharge();
         }
         else
         {
            m_agent.doNothing();
         }
      }
      void onExit() {}
   };
   friend class DoingHisJob;

private:
   std::string m_action;
   ExplosiveCharge* m_chargeToArm;
   Idle* m_idle;
   DoingHisJob* m_work;

public:
   DemolitionMan() 
         : Agent("demolitionMan"), m_chargeToArm(NULL)
   {
      m_idle = new Idle(*this);
      m_work = new DoingHisJob(*this);
   }

   ~DemolitionMan()
   {
      delete m_idle;
      m_idle = NULL;

      delete m_work;
      m_work = NULL;
   }

   void arm(ExplosiveCharge& charge)
   {
      m_chargeToArm = &charge;
      fsm().setState(*m_work);
   }

   void doNothing()
   {
      fsm().setState(*m_idle);
   }

   void setAction(const std::string& action) {m_action = action;}
   const std::string& getAction() const {return m_action;}

private:
   void armCharge()
   {
      assert(m_chargeToArm != NULL);
      m_chargeToArm->arm();
      m_chargeToArm = NULL;
   }

   bool hasChargeToDisarm() const 
   {
      return m_chargeToArm != NULL;
   }
};

///////////////////////////////////////////////////////////////////////////////
