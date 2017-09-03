#include <qapplication.h>
#include "galaxians.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    Galaxians gal;
    gal.show();
    return app.exec();
}
