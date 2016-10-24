#include <QApplication>
#include "mprexposuredialog.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);
    app.setOrganizationName(QString("Matt Ruffner 2016"));
    app.setOrganizationDomain(QString("github.com/ruffner"));
    app.setApplicationName(QString("Exposure Configuration"));

    MPRExposureDialog w;
    return w.exec();

    return app.exec();
}

