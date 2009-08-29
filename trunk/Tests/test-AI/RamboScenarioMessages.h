#pragma once

#include "core-AI\Message.h"
#include "Rambo.h"
#include "Enemy.h"


///////////////////////////////////////////////////////////////////////////////

class ReduceHealth : public Message,
                     public TMessage<Rambo>,
                     public TMessage<Enemy>
{
public:
   Message* clone() {return new ReduceHealth(*this);}

   void perform(Rambo& rambo, Messagable& sender)
   {
      rambo.receiveWound(1);
   }

   void perform(Enemy& enemy, Messagable& sender)
   {
      enemy.reduceHealth(1);
      enemy.attack(sender);
   }
};

///////////////////////////////////////////////////////////////////////////////

class Shots : public Message,
              public TMessage<Rambo>
{
   Message* clone() {return new Shots(*this);}

   void perform(Rambo& rambo, Messagable& sender)
   {
      Enemy& enemySpotted = dynamic_cast<Enemy&>(sender);
      rambo.seesEnemy(enemySpotted);
   }
};

///////////////////////////////////////////////////////////////////////////////
