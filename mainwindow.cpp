#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>

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

string employeeFound;

void MainWindow::on_btn_AgregarEmpleado_clicked()
{
    string ID=ui->tbx_ID->text().toStdString();
    string Nombre=ui->tbx_Nombre->text().toStdString();
    string Departamento=ui->tbx_Departamento->text().toStdString();
    string Puesto=ui->tbx_Puesto->text().toStdString();
    string Contrasena=AdminEmpleados.EncriptarContrasena(ui->tbx_Contrasena->text().toStdString());
    double Salario=ui->tbx_Salario->text().toDouble();
    AdminEmpleados.AgregarEmpleado(Empleado(ID,Nombre,Departamento,Puesto,Contrasena,Salario));
    ui->tbx_ID->setText("");
    ui->tbx_Nombre->setText("");
    ui->tbx_Departamento->setText("");
    ui->tbx_Puesto->setText("");
    ui->tbx_Salario->setText("");
    ui->tbx_Contrasena->setText("");
    ReloadTable();
}

void MainWindow::ReloadTable()
{
    ui->tbl_Output->clear();
    ui->tbl_Output->setColumnCount(7);
    ui->tbl_Output->setHorizontalHeaderLabels(QStringList()<<"ID"<<"Nombre"<<"Departamento"<<"Puesto"<<"Salario"<<"Estado"<<"Contrasena");
    ui->tbl_Output->setRowCount(AdminEmpleados.Empleados.size());
    for(uint i=0;i<AdminEmpleados.Empleados.size();i++)
    {
       ui->tbl_Output->setItem(i,0,new QTableWidgetItem(QString::fromStdString(AdminEmpleados.Empleados[i].ID)));
       ui->tbl_Output->setItem(i,1,new QTableWidgetItem(QString::fromStdString(AdminEmpleados.Empleados[i].Nombre)));
       ui->tbl_Output->setItem(i,2,new QTableWidgetItem(QString::fromStdString(AdminEmpleados.Empleados[i].Departamento)));
       ui->tbl_Output->setItem(i,3,new QTableWidgetItem(QString::fromStdString(AdminEmpleados.Empleados[i].Puesto)));
       ui->tbl_Output->setItem(i,4,new QTableWidgetItem(QString::number(AdminEmpleados.Empleados[i].Salario)));
       ui->tbl_Output->setItem(i,5,new QTableWidgetItem(QString::fromStdString(AdminEmpleados.Empleados[i].Activo ? "Activo" : "De baja en la Empresa")));
       ui->tbl_Output->setItem(i,6,new QTableWidgetItem(QString::fromStdString(AdminEmpleados.Empleados[i].Contrasena)));
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
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_GestionI_PB_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_BackEmpleados_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
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
    if(employeeFound == "Gerente" || employeeFound == "gerente"){
        ui->stackedWidget->setCurrentIndex(1);
    }else if(employeeFound == "Vendedor" || employeeFound == "vendedor"){
        ui->stackedWidget->setCurrentIndex(2);
    }

}


void MainWindow::on_BackVentas_clicked()
{
    if(employeeFound == "Gerente" || employeeFound == "gerente"){
        ui->stackedWidget->setCurrentIndex(1);
    }else if(employeeFound == "Vendedor" || employeeFound == "vendedor"){
        ui->stackedWidget->setCurrentIndex(2);
    }
}


void MainWindow::on_BackCliente_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_BackInventario_clicked()
{
    if(employeeFound == "Gerente" || employeeFound == "gerente"){
        ui->stackedWidget->setCurrentIndex(1);
    }else if(employeeFound == "Vendedor" || employeeFound == "vendedor"){
        ui->stackedWidget->setCurrentIndex(2);
    }
}


void MainWindow::on_PB_Salir_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_pushButton_clicked()
{

    string ID = ui->lineEdit_4->text().toStdString();
    string Contrasena = AdminEmpleados.EncriptarContrasena(ui->lineEdit_6->text().toStdString());

    if(ID.empty() || Contrasena.empty()){
        QMessageBox::warning(this, "Advertencia", "Porfavor, ingrese tanto el ID como su Contrasena");
        return;
    }
    AdminEmpleados.CargarDatos();
    bool Autorizado=false;
    for(uint i =0; i<AdminEmpleados.Empleados.size();i++)
    {
        if(ID==AdminEmpleados.Empleados[i].ID && Contrasena==AdminEmpleados.Empleados[i].Contrasena)
        {

           Autorizado=true;
        }
    }
    Autorizado?QMessageBox::information(this, "Exito", "Usuario Correcto"):QMessageBox::warning(this, "Advertencia", "Usuario o Contrasena incorrectos");

}


void MainWindow::on_btn_AgregarInventario_clicked()
{
    string ID=ui->tbx_ID_2->text().toStdString();
    string Nombre = ui->tbx_Nombre_2->text().toStdString();
    double Precio = ui->tbx_precio->text().toDouble();
    int Cantidad = ui->sb_Cantidad->value();
    AdministradorInventario.addProduct(Producto(ID,Nombre,Precio,Cantidad));
    ActualizarProductos();
}

void MainWindow::ActualizarProductos()
{
    ui->tbl_Output->clear();
    ui->tbl_Inventario->setColumnCount(4);
    ui->tbl_Inventario->setHorizontalHeaderLabels(QStringList()<<"ID"<<"Nombre"<<"Precio"<<"Cantidad Disponible");
    ui->tbl_Inventario->setRowCount(AdministradorInventario.products.size());
    for(uint i=0;i<AdministradorInventario.products.size();i++)
    {
        ui->tbl_Inventario->setItem(i,0,new QTableWidgetItem(QString::fromStdString(AdministradorInventario.products[i].ID)));
        ui->tbl_Inventario->setItem(i,1,new QTableWidgetItem(QString::fromStdString(AdministradorInventario.products[i].Nombre)));
        ui->tbl_Inventario->setItem(i,2,new QTableWidgetItem(QString::number(AdministradorInventario.products[i].Precio)));
        ui->tbl_Inventario->setItem(i,3,new QTableWidgetItem(QString::number(AdministradorInventario.products[i].Cantidad)));
    }
}


void MainWindow::on_btn_CargarInv_clicked()
{
    AdministradorInventario.CargarDatos();
}


void MainWindow::on_btn_GuardarInv_clicked()
{
    AdministradorInventario.GuardarDatos();
}


void MainWindow::on_pushButton_2_clicked()
{
    bool ok;
        QString password = QInputDialog::getText(this, "Verificar Administrador", "Ingrese la contraseña de administrador:", QLineEdit::Password, "", &ok);
        if (!ok || password.isEmpty())
        {
            QMessageBox::information(this, "Acceso Denegado", "No se ingresó una contraseña.");
            return;
        }
        if(password == "Admin")
        {
            QMessageBox::information(this, "Acceso Concedido", "Administrador verificado. Proceda a agregar el cliente.");
            ui->stackedWidget->setCurrentIndex(3);
        }
        else
        {
            QMessageBox::critical(this, "Acceso Denegado", "La contraseña ingresada es incorrecta.");
        }

}

