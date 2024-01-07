#include "process_manager.h"
#include "ui_process_manager.h"
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
#include <QDebug>

ProcessManager::ProcessManager(QTableWidget &table, QPushButton &button) : processTable(table), changePriorityButton(button), selectedProcessId(0)
{
    QObject::connect(&changePriorityButton, &QPushButton::clicked, this, &ProcessManager::changePriority);
}
ProcessManager::~ProcessManager()
{
    delete ui;
}

void ProcessManager::updateTable() {
    processTable.clearContents();

    processTable.setRowCount(0);
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    PROCESS_MEMORY_COUNTERS pmc;
    int countsProcesses = 0;
    DWORD processIds[1024];
    unsigned int processCount = 0;
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return;
    }
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hProcessSnap, &pe32)) {
        HANDLE hProcess;
        do {

            int row = processTable.rowCount();
            hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pe32.th32ProcessID);

            int priority = GetPriorityClass(hProcess);
            if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
            {

                processTable.insertRow(row);
                processTable.setItem(row, 0, new QTableWidgetItem(QString::number(pe32.th32ProcessID)));
                processTable.setItem(row, 1, new QTableWidgetItem(QString::fromWCharArray(pe32.szExeFile)));
                processTable.setItem(row, 2, new QTableWidgetItem(QString::number(priority)));
                processTable.setItem(row, 3, new QTableWidgetItem(QString::number(pmc.WorkingSetSize)));
                CloseHandle(hProcess);
            }


        } while (Process32Next(hProcessSnap, &pe32));

        CloseHandle(hProcessSnap);
}
}

void ProcessManager::changePriority()
{
if (selectedProcessId != 0)
{
        HANDLE hProcess = OpenProcess(PROCESS_SET_INFORMATION, FALSE, selectedProcessId);
        if (hProcess)
        {
            int newPriority = HIGH_PRIORITY_CLASS; // Установите нужный приоритет
            if (SetPriorityClass(hProcess, newPriority))
            {
                qDebug() << "Приоритет процесса изменен успешно.";
                // Выполните дополнительные действия, если необходимо
            }
            else
            {
                qDebug() << "Ошибка при изменении приоритета процесса.";
            }

            CloseHandle(hProcess);
        }
        else
        {
            qDebug() << "Ошибка при открытии процесса для изменения приоритета.";
        }
}
else
{
        qDebug() << "Выберите процесс для изменения приоритета.";
}
}
