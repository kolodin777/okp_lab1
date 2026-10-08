#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <QObject>
#include <QString>

class Validator : public QObject
{
    Q_OBJECT

public:
    explicit Validator(QObject *parent = nullptr);

public slots:
    // Пытается превратить строку в число.
    // Возвращает true и пишет результат в out — если получилось.
    // Иначе излучает validationFailed(...) и возвращает false.
    bool validateNumber(const QString &input, double &out);

signals:
    // Излучается, когда строка не является числом
    void validationFailed(const QString &message);
};

#endif // VALIDATOR_H