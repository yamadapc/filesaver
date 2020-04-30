//
// Created by Pedro Tacla Yamada on 01/05/20.
//

#include <QApplication>
#include <filesaver/FileSaver.h>

int main (int argc, char* argv[])
{
    QApplication app (argc, argv);
    filesaver::FileSaver fileSaver;

    fileSaver.scan("/");
    fileSaver.start();

    return app.exec ();
}