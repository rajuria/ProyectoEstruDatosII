#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "AdministradorEmpleados.h"
#include "AdminInventario.h"
#include <QMessageBox>
#include <iostream>

using namespace std;

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

//    void on_btn_Guardar_clicked();

    void on_btn_Cargar_clicked();

    void on_btn_Reporte_clicked();

    void on_GestionE_PB_clicked();

    void on_GestionI_PB_clicked();

    void on_BackEmpleados_clicked();

    void on_GestionC_PB_clicked();

    void on_GestionV_PB_clicked();

    void on_GestionP_PB_clicked();

    void on_BackPedidos_clicked();

    void on_BackVentas_clicked();

    void on_BackCliente_clicked();

    void on_BackInventario_clicked();

    void on_PB_Salir_clicked();

    void on_pushButton_clicked();

    void on_btn_AgregarInventario_clicked();
    void ActualizarProductos();

    void on_btn_CargarInv_clicked();

    void on_btn_GuardarInv_clicked();

    void on_pushButton_2_clicked();

    void on_btn_AgregarCliente_clicked();

    void on_btn_BuscarEmpleado_clicked();

    void on_pB_SchInventory_clicked();

    void on_PB_Vender_clicked();

    void on_PB_CrearPedido_clicked();

    void on_btn_CargarCliente_clicked();

    void on_btn_BuscarCliente_clicked();

private:
    Ui::MainWindow *ui;
    AdministradorEmpleados AdminEmpleados;
    AdminInventario AdministradorInventario;
    void ReloadTable();
    void ActualizarTablaClientes();
};
#endif // MAINWINDOW_H
