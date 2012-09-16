#include "MenuUtils.h"
#include "TreeWidgetDescFactory.h"
#include <QIcon>
#include <QMenu>
#include <QAction>


///////////////////////////////////////////////////////////////////////////////

void MenuUtils::itemsListMenu( TreeWidgetDescFactory* itemsFactory, MenuActionsFactory& actionsFactory, QMenu* hostMenu )
{
   if ( !itemsFactory || itemsFactory->typesCount() <= 0)
   {
      return;
   }


   QString  desc;
   QString  group;
   QIcon    icon;

   typedef std::map< QString, QMenu* > GroupMenusMap;
   GroupMenusMap   groups;

   unsigned int factoriesCount = itemsFactory->typesCount();
   for ( unsigned int itemTypeIdx = 0; itemTypeIdx < factoriesCount; ++itemTypeIdx )
   {
      itemsFactory->getDesc( itemTypeIdx, desc, group, icon );

      // find a proper group for this item ( or create a new one )
      QMenu* groupMenu = NULL;
      if ( group.isEmpty() )
      {
         // if no group name was specified, simply aggregate the entries directly under the host menu
         groupMenu = hostMenu;
      }
      else
      {
         GroupMenusMap::iterator groupIt = groups.find( group );
         if ( groupIt != groups.end() )
         {
            groupMenu = groupIt->second;
         }
         else
         {
            groupMenu = new QMenu( group, hostMenu );
            groups.insert( std::make_pair( group, groupMenu ) );
         }
      }

      QAction* addAction = actionsFactory.create( icon, desc, itemTypeIdx, hostMenu );
      groupMenu->addAction( addAction );
   }

   // now add all the groups to the menu in alphabetical order
   for ( GroupMenusMap::iterator it = groups.begin(); it != groups.end(); ++it )
   {
      hostMenu->addMenu( it->second );
   }
}

///////////////////////////////////////////////////////////////////////////////
