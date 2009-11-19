#include "tamyeditor.h"
#include <QtGui/QApplication>
#include "SceneQueries.h"
#include "PropertiesEditor.h"
#include "SceneNavigator.h"


///////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);

   TamyEditor w(a);

   // register services
   w.add(new SceneQueries());
   w.add(new PropertiesEditor());
   w.add(new SceneNavigator());

   // start the application
   w.show();
   return a.exec();
}

///////////////////////////////////////////////////////////////////////////////
