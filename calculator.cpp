#include "calculator.h"
#include <QDebug>
#include <cmath>

Calculator::Calculator(QObject *parent)
    : QObject(parent)
    , m_result(0.0)
    , m_hasError(false)
    , m_validator(new Validator(this))   // родитель — this, удалится автоматически
{
    qDebug() << "Calculator created";

    // ГЛАВНОЕ СОЕДИНЕНИЕ ВАРИАНТА:
    // сигнал Validator'а → приватный слот Calculator'а
    QObject::connect(m_validator, &Validator::validationFailed,
                     this,        &Calculator::onValidationFailed);
}

void Calculator::setResult(double value)
{
    m_result = value;
    m_hasError = false;
    m_errorMessage.clear();
    emit resultReady(m_result);
}

void Calculator::setError(const QString &message)
{
    m_hasError = true;
    m_errorMessage = message;
    emit errorOccurred(m_errorMessage);
}

// ---- Числовые слоты ----

void Calculator::add(double a, double b)
{
    qDebug() << "add(" << a << ", " << b << ")";
    setResult(a + b);
}

void Calculator::subtract(double a, double b)
{
    qDebug() << "subtract(" << a << ", " << b << ")";
    setResult(a - b);
}

void Calculator::multiply(double a, double b)
{
    qDebug() << "multiply(" << a << ", " << b << ")";
    setResult(a * b);
}

void Calculator::divide(double a, double b)
{
    qDebug() << "divide(" << a << ", " << b << ")";
    if (qFuzzyIsNull(b)) {
        setError(QString::fromUtf8("Деление на ноль невозможно!"));
        return;
    }
    setResult(a / b);
}

void Calculator::ln(double a)
{
    qDebug() << "ln(" << a << ")";
    if (a <= 0.0) {
        setError(QString::fromUtf8("Логарифм определён только для положительных чисел (a > 0)!"));
        return;
    }
    setResult(std::log(a));
}

void Calculator::reset()
{
    qDebug() << "reset()";
    m_result = 0.0;
    m_hasError = false;
    m_errorMessage.clear();
    emit resultReady(m_result);
}

// ---- Слоты со строками (валидация через Validator) ----

void Calculator::addFromStrings(const QString &a, const QString &b)
{
    double x = 0, y = 0;
    if (!m_validator->validateNumber(a, x)) return;
    if (!m_validator->validateNumber(b, y)) return;
    add(x, y);
}

void Calculator::subtractFromStrings(const QString &a, const QString &b)
{
    double x = 0, y = 0;
    if (!m_validator->validateNumber(a, x)) return;
    if (!m_validator->validateNumber(b, y)) return;
    subtract(x, y);
}

void Calculator::multiplyFromStrings(const QString &a, const QString &b)
{
    double x = 0, y = 0;
    if (!m_validator->validateNumber(a, x)) return;
    if (!m_validator->validateNumber(b, y)) return;
    multiply(x, y);
}

void Calculator::divideFromStrings(const QString &a, const QString &b)
{
    double x = 0, y = 0;
    if (!m_validator->validateNumber(a, x)) return;
    if (!m_validator->validateNumber(b, y)) return;
    divide(x, y);
}

void Calculator::lnFromString(const QString &a)
{
    double x = 0;
    if (!m_validator->validateNumber(a, x)) return;
    ln(x);
}

// ---- Реакция на сигнал Validator'а ----

void Calculator::onValidationFailed(const QString &message)
{
    qDebug() << "onValidationFailed(" << message << ")";
    setError(message);
}