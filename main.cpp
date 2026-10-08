#include <QCoreApplication>
#include <QTextStream>
#include <QStringList>
#include <QByteArray>
#include <cstdio>
#include <string>

#ifdef Q_OS_WIN
#  include <windows.h>
#endif

#include "calculator.h"

// Печать строки в консоль. На Windows — через WriteConsoleW,
// чтобы не зависеть от chcp и шрифта.
static void printLine(const QString &text)
{
#ifdef Q_OS_WIN
    const std::wstring w = text.toStdWString();
    DWORD written = 0;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    WriteConsoleW(h, w.c_str(), static_cast<DWORD>(w.size()), &written, nullptr);
#else
    const QByteArray bytes = text.toLocal8Bit();
    std::fwrite(bytes.constData(), 1, bytes.size(), stdout);
    std::fflush(stdout);
#endif
}

static void printHelp()
{
    printLine(QString::fromUtf8("Доступные команды:\n"));
    printLine(QString::fromUtf8("  add <a> <b>   - сложение\n"));
    printLine(QString::fromUtf8("  sub <a> <b>   - вычитание\n"));
    printLine(QString::fromUtf8("  mul <a> <b>   - умножение\n"));
    printLine(QString::fromUtf8("  div <a> <b>   - деление\n"));
    printLine(QString::fromUtf8("  ln  <a>       - натуральный логарифм (a > 0)\n"));
    printLine(QString::fromUtf8("  reset         - сброс\n"));
    printLine(QString::fromUtf8("  help          - эта справка\n"));
    printLine(QString::fromUtf8("  quit          - выход\n"));
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

#ifdef Q_OS_WIN
    SetConsoleOutputCP(CP_UTF8);
#endif

    QTextStream in(stdin);

    Calculator calc;

    // Подключаемся к сигналам калькулятора
    QObject::connect(&calc, &Calculator::resultReady,
                     [](double result) {
                         printLine(QString::fromUtf8("Результат: %1\n").arg(result));
                     });

    QObject::connect(&calc, &Calculator::errorOccurred,
                     [](const QString &msg) {
                         printLine(QString::fromUtf8("Ошибка: %1\n").arg(msg));
                     });

    printLine(QString::fromUtf8("=== Консольный калькулятор с валидацией (средний, вариант 10) ===\n"));
    printHelp();
    printLine(QString::fromUtf8("\n> "));

    QString line;
    while (in.readLineInto(&line)) {
        line = line.trimmed();

        if (line.isEmpty()) {
            printLine(QString::fromUtf8("> "));
            continue;
        }

        const QStringList parts = line.split(' ', Qt::SkipEmptyParts);
        const QString command = parts.value(0).toLower();

        if (command == QStringLiteral("quit") || command == QStringLiteral("exit")) {
            printLine(QString::fromUtf8("До свидания!\n"));
            break;
        }
        if (command == QStringLiteral("help")) {
            printHelp();
            printLine(QString::fromUtf8("> "));
            continue;
        }
        if (command == QStringLiteral("reset")) {
            calc.reset();
            printLine(QString::fromUtf8("> "));
            continue;
        }

        // Унарная команда ln
        if (command == QStringLiteral("ln")) {
            if (parts.size() != 2) {
                printLine(QString::fromUtf8("Ошибка: формат: ln <a>\n"));
                printLine(QString::fromUtf8("> "));
                continue;
            }
            // ← вот здесь работает валидация через Validator
            calc.lnFromString(parts[1]);
            printLine(QString::fromUtf8("> "));
            continue;
        }

        // Бинарные команды
        if (parts.size() != 3) {
            printLine(QString::fromUtf8("Ошибка: неверный формат. Используйте: <команда> <a> <b>\n"));
            printLine(QString::fromUtf8("> "));
            continue;
        }

        // ← здесь строки передаются как есть — валидатор сам проверит
        if (command == QStringLiteral("add")) {
            calc.addFromStrings(parts[1], parts[2]);
        } else if (command == QStringLiteral("sub")) {
            calc.subtractFromStrings(parts[1], parts[2]);
        } else if (command == QStringLiteral("mul")) {
            calc.multiplyFromStrings(parts[1], parts[2]);
        } else if (command == QStringLiteral("div")) {
            calc.divideFromStrings(parts[1], parts[2]);
        } else {
            printLine(QString::fromUtf8("Неизвестная команда: %1\n").arg(command));
        }

        printLine(QString::fromUtf8("> "));
    }

    return 0;
}