#include <QApplication>
#include "view.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    View v;
    v.show();

    return app.exec();
}
