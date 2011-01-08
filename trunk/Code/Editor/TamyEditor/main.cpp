#include "tamyeditor.h"
#include <QtGui/QApplication>
#include <QMessageBox.h>
#include "assertDialog.h"
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

// type includes
#include "core/TypesRegistry.cpp"
#include "core-MVC/TypesRegistry.cpp"
#include "core-Renderer/TypesRegistry.cpp"

///////////////////////////////////////////////////////////////////////////////

class TamyEditorApp : public QApplication
{
public:
   TamyEditorApp(int argc, char* argv[])
   : QApplication(argc, argv)
   {}

protected:
   bool notify(QObject* receiver, QEvent* event )
   {
      bool result = false;
      try
      {
         result = QApplication::notify(receiver, event);
      }
      catch (std::exception& ex)
      {
         QMessageBox::warning(NULL, tr("Tamy editor error"),
            QString(ex.what()),
            QMessageBox::Ok);
      }

      return result;
   }
};

///////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
   TamyEditorApp a(argc, argv);

   registerAssertCallback( &AssertDialog::createDialog );

   TamyEditor w( a, "D:/Docs/Projects/Tamy/Assets/" );

   // start the application
   w.show();
   return a.exec();
}

///////////////////////////////////////////////////////////////////////////////
