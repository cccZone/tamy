#include "tamyeditor.h"
#include <QtGui/QApplication>
#include <QMessageBox.h>
#include "assertDialog.h"
#include "core\Assert.h"


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

   TamyEditor w(a);

   // start the application
   w.show();
   return a.exec();
}

///////////////////////////////////////////////////////////////////////////////
