#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "AdministradorEmpleados.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_AgregarEmpleado_clicked();

    void on_btn_Guardar_clicked();

    void on_btn_Cargar_clicked();

    void on_btn_Reporte_clicked();

    void on_pushButton_clicked();

    void on_GestionE_PB_clicked();

private:
    Ui::MainWindow *ui;
    AdministradorEmpleados AdminEmpleados;
    void ReloadTable();
};
#endif // MAINWINDOW_H
