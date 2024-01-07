#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <windows.h>

QT_BEGIN_NAMESPACE
namespace Ui { class ProcessManager; }
QT_END_NAMESPACE

class ProcessManager : public QMainWindow
{
    Q_OBJECT

public:
    ProcessManager(QTableWidget &table, QPushButton &button);
    ~ProcessManager();
    void updateTable();
    void setPriority();

public slots:
    void changePriority();

private:
    Ui::ProcessManager *ui;
    QTableWidget &processTable;
    QPushButton &changePriorityButton;
    DWORD selectedProcessId;

};
#endif // PROCESS_MANAGER_H
