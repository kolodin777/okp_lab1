#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QObject>
#include <QString>
#include "validator.h"

class Calculator : public QObject
{
    Q_OBJECT

public:
    explicit Calculator(QObject *parent = nullptr);

    double result() const { return m_result; }
    bool hasError() const { return m_hasError; }
    QString errorMessage() const { return m_errorMessage; }

    // Доступ к валидатору
    Validator *validator() const { return m_validator; }

public slots:
    // Основные слоты (принимают уже готовые числа)
    void add(double a, double b);
    void subtract(double a, double b);
    void multiply(double a, double b);
    void divide(double a, double b);
    void ln(double a);
    void reset();

    // Слоты, принимающие СТРОКИ (валидация через Validator)
    void addFromStrings(const QString &a, const QString &b);
    void subtractFromStrings(const QString &a, const QString &b);
    void multiplyFromStrings(const QString &a, const QString &b);
    void divideFromStrings(const QString &a, const QString &b);
    void lnFromString(const QString &a);

private slots:
    // Ловит сигнал от Validator и превращает его в свой errorOccurred
    void onValidationFailed(const QString &message);

signals:
    void resultReady(double result);
    void errorOccurred(const QString &message);

private:
    double  m_result;
    bool    m_hasError;
    QString m_errorMessage;

    Validator *m_validator;

    void setResult(double value);
    void setError(const QString &message);
};

#endif // CALCULATOR_H