#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_AgregarEmpleado_clicked()
{
    string ID=ui->tbx_ID->text().toStdString();
    string Nombre=ui->tbx_Nombre->text().toStdString();
    string Departamento=ui->tbx_Departamento->text().toStdString();
    string Puesto=ui->tbx_Puesto->text().toStdString();
    double Salario=ui->tbx_Salario->text().toDouble();
    AdminEmpleados.AgregarEmpleado(Empleado(ID,Nombre,Departamento,Puesto,Salario));
    ui->tbx_ID->text()="";;
    ui->tbx_Nombre->text()="";
    ui->tbx_Departamento->text()="";
    ui->tbx_Puesto->text()="";
    ui->tbx_Salario->text()="";
    ReloadTable();
}

void MainWindow::ReloadTable()
{
    ui->tbl_Output->clear();
    ui->tbl_Output->setColumnCount(6);
    ui->tbl_Output->setHorizontalHeaderLabels(QStringList()<<"ID"<<"Nombre"<<"Departamento"<<"Puesto"<<"Salario"<<"Estado");
    ui->tbl_Output->setRowCount(AdminEmpleados.Empleados.size());
    for(uint i=0;i<AdminEmpleados.Empleados.size();i++)
    {
       ui->tbl_Output->setItem(i,0,new QTableWidgetItem(QString::fromStdString(AdminEmpleados.Empleados[i].ID)));
       ui->tbl_Output->setItem(i,1,new QTableWidgetItem(QString::fromStdString(AdminEmpleados.Empleados[i].Nombre)));
       ui->tbl_Output->setItem(i,2,new QTableWidgetItem(QString::fromStdString(AdminEmpleados.Empleados[i].Departamento)));
       ui->tbl_Output->setItem(i,3,new QTableWidgetItem(QString::fromStdString(AdminEmpleados.Empleados[i].Puesto)));
       ui->tbl_Output->setItem(i,4,new QTableWidgetItem(QString::number(AdminEmpleados.Empleados[i].Salario)));
       ui->tbl_Output->setItem(i,5,new QTableWidgetItem(QString::fromStdString(AdminEmpleados.Empleados[i].Activo ? "Activo" : "De baja en la Empresa")));
    }
}


void MainWindow::on_btn_Guardar_clicked()
{
    AdminEmpleados.GuardarDatos();
}


void MainWindow::on_btn_Cargar_clicked()
{
    AdminEmpleados.CargarDatos();
    ReloadTable();
}


void MainWindow::on_btn_Reporte_clicked()
{
    AdminEmpleados.GenerarReporte();
}

void MainWindow::on_GestionE_PB_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_GestionI_PB_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_BackEmpleados_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_GestionC_PB_clicked()
{
     ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_GestionV_PB_clicked()
{
     ui->stackedWidget->setCurrentIndex(4);
}


void MainWindow::on_GestionP_PB_clicked()
{
     ui->stackedWidget->setCurrentIndex(5);
}


void MainWindow::on_BackPedidos_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_BackVentas_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_BackCliente_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_BackInventario_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_PB_Salir_clicked()
{
    exit(0);
}

