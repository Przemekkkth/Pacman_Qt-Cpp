#ifndef INKY_H
#define INKY_H

#include <QObject>

class Inky : public QObject
{
    Q_OBJECT
public:
    explicit Inky(QObject *parent = nullptr);

signals:

};

#endif // INKY_H
