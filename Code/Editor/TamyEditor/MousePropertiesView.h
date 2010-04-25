#pragma once

/// @file   TamyEditor\MousePropertiesView.h
/// @brief  view that edits properties using mouse input

#include "core\PropertiesView.h"


///////////////////////////////////////////////////////////////////////////////

struct Point;

///////////////////////////////////////////////////////////////////////////////

class MousePropertiesView : public TPropertiesView<MousePropertiesView>
{
public:
   class Observer
   {
   public:
      virtual ~Observer() {}

      virtual void update(const Point& mousePos) = 0;
   };

private:
   typedef std::vector<Observer*> Observers;
   Observers m_observers;

public:
   MousePropertiesView();

   void update(const Point& mousePos);

   void attach(Observer& observer);
   void detach(Observer& observer);
};

///////////////////////////////////////////////////////////////////////////////
