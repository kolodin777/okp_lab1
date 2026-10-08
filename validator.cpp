#include "validator.h"
#include <QDebug>

Validator::Validator(QObject *parent)
    : QObject(parent)
{
    qDebug() << "Validator created";
}

bool Validator::validateNumber(const QString &input, double &out)
{
    bool ok = false;
    const double value = input.toDouble(&ok);

    if (!ok) {
        emit validationFailed(
            QString::fromUtf8("Не удалось преобразовать «%1» в число").arg(input));
        return false;
    }

    out = value;
    return true;
}