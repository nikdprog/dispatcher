#include "process_manager.h"
#include <windows.h>
#include <QMainWindow>
#include <tlhelp32.h>
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QTableWidget>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QProcess>
#include <psapi.h>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow mainWindow;
    mainWindow.setWindowTitle("Диспетчер процессов");

    QTableWidget processTable(&mainWindow);
    processTable.setColumnCount(4);
    processTable.setHorizontalHeaderLabels(QStringList() << "PID" << "Имя процесса" << "Приоритет" << "Память");
    //processTable.horizontalHeader()->setStretchLastSection(true);

    QPushButton refreshButton("Обновить", &mainWindow);
    QPushButton changePriorityButton("Изменить приоритет", &mainWindow);
    QVBoxLayout layout;
    layout.addWidget(&processTable);
    layout.addWidget(&refreshButton);
    layout.addWidget(&changePriorityButton);

    QWidget centralWidget;
    centralWidget.setLayout(&layout);
    mainWindow.setCentralWidget(&centralWidget);

    ProcessManager processManager(processTable, changePriorityButton);

    QTimer timer;
    timer.setInterval(1000);
    timer.start();

    QObject::connect(&timer, &QTimer::timeout, [&]() {
        processManager.updateTable();
    });
    mainWindow.show();
    return a.exec();
}
