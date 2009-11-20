#include "tamyeditor.h"
#include <QtGui/QApplication>
#include "SceneQueries.h"
#include "SceneNavigator.h"
#include "PropertiesEditor.h"


///////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);

   TamyEditor w(a);

   // register services
   w.add(new SceneQueries());
   w.add(new SceneNavigator());
   w.add(new PropertiesEditor());

   // start the application
   w.show();
   return a.exec();
}

///////////////////////////////////////////////////////////////////////////////
