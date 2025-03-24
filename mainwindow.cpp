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
    string Puesto=ui->cb_Puesto->currentText().toStdString();
    string Contrasena=AdminEmpleados.EncriptarContrasena(ui->tbx_Contrasena->text().toStdString());
    double Salario=ui->tbx_Salario->text().toDouble();
    AdminEmpleados.AgregarEmpleado(Empleado(ID,Nombre,Departamento,Puesto,Contrasena,Salario));
    ui->tbx_ID->setText("");
    ui->tbx_Nombre->setText("");
    ui->tbx_Departamento->setText("");
    ui->tbx_Salario->setText("");
    ui->tbx_Contrasena->setText("");
    AdminEmpleados.GuardarDatos();
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

void MainWindow::ActualizarTablaClientes()
{
    ui->tbl_Clientes->clear();
    ui->tbl_Clientes->setColumnCount(3);
    ui->tbl_Clientes->setHorizontalHeaderLabels(QStringList()<<"ID"<<"Nombre"<<"Empresa");
    ui->tbl_Clientes->setRowCount(AdministradorInventario.Clientes.size());
    for(uint i=0;i<AdministradorInventario.Clientes.size();i++)
    {
       ui->tbl_Clientes->setItem(i,0,new QTableWidgetItem(QString::fromStdString(AdministradorInventario.Clientes[i].IDCliente)));
       ui->tbl_Clientes->setItem(i,1,new QTableWidgetItem(QString::fromStdString(AdministradorInventario.Clientes[i].Nombre)));
       ui->tbl_Clientes->setItem(i,2,new QTableWidgetItem(QString::fromStdString(AdministradorInventario.Clientes[i].Empresa)));
    }
}


void MainWindow::on_btn_Cargar_clicked()
{
    AdminEmpleados.ImportarCSV();
    ReloadTable();
}


void MainWindow::on_btn_Reporte_clicked()
{
    AdminEmpleados.GenerarReporte();
}

void MainWindow::on_GestionE_PB_clicked()
{
    AdminEmpleados.CargarDatos();
    ReloadTable();
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_GestionI_PB_clicked()
{
    ActualizarProductos();
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_BackEmpleados_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_GestionC_PB_clicked()
{
    AdministradorInventario.CargarClientes();
    ActualizarTablaClientes();
    ui->stackedWidget->setCurrentIndex(5);
}


void MainWindow::on_GestionV_PB_clicked()
{
     ui->stackedWidget->setCurrentIndex(6);
}


void MainWindow::on_GestionP_PB_clicked()
{
     ui->stackedWidget->setCurrentIndex(7);
}


void MainWindow::on_BackPedidos_clicked()
{
    if(employeeFound == "Gerente" || employeeFound == "gerente"){
        ui->stackedWidget->setCurrentIndex(1);
    }else if(employeeFound == "Vendedor" || employeeFound == "vendedor"){
        ui->stackedWidget->setCurrentIndex(2);
    }
    else
    {
       ui->stackedWidget->setCurrentIndex(1);
    }

}


void MainWindow::on_BackVentas_clicked()
{
    if(employeeFound == "Gerente" || employeeFound == "gerente"){
        ui->stackedWidget->setCurrentIndex(1);
    }else if(employeeFound == "Vendedor" || employeeFound == "vendedor"){
        ui->stackedWidget->setCurrentIndex(2);
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(1);
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
    else
    {
       ui->stackedWidget->setCurrentIndex(1);
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
    string Puesto ="";

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
            Puesto=AdminEmpleados.Empleados[i].Puesto;
           Autorizado=true;
        }
    }
    if(!Autorizado)
    {
      QMessageBox::warning(this, "Advertencia", "Usuario o Contrasena incorrectos");
    }
    else
    {
        if(Puesto=="Administrador")
        {
            ui->stackedWidget->setCurrentIndex(1);
        }
        else
        {
            QMessageBox::warning(this, "Advertencia", "No se ha encontrado un perfil para su puesto.");
        }
    }

}


void MainWindow::on_btn_AgregarInventario_clicked()
{
    string ID=ui->tbx_ID_2->text().toStdString();
    string Nombre = ui->tbx_Nombre_2->text().toStdString();
    double Precio = ui->tbx_precio->text().toDouble();
    int Cantidad = ui->sb_Cantidad->value();
    AdministradorInventario.addProduct(Producto(ID,Nombre,Precio,Cantidad));
    AdministradorInventario.GuardarDatos();
    ActualizarProductos();
}

void MainWindow::ActualizarProductos()
{
    AdministradorInventario.CargarDatos();
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
    if (!ok || password.isEmpty()){
        QMessageBox::information(this, "Acceso Denegado", "No se ingresó una contraseña.");
        return;
    }
    if(password == "Admin"){
        QMessageBox::information(this, "Acceso Concedido", "Administrador verificado. Proceda a agregar el cliente.");
        ui->stackedWidget->setCurrentIndex(3);
    }else{
        QMessageBox::critical(this, "Acceso Denegado", "La contraseña ingresada es incorrecta.");
    }

}


void MainWindow::on_btn_AgregarCliente_clicked()
{
    string IDCliente = ui->tbx_IDCliente->text().toStdString();
    string Nombre = ui->tbx_NombreCliente->text().toStdString();
    string Empresa = ui->tbx_EmpresaCliente->text().toStdString();
    Cliente temp = Cliente(IDCliente,Nombre,Empresa);
    AdministradorInventario.Clientes.push_back(temp);
    ActualizarTablaClientes();
    AdministradorInventario.GuardarClientes();
    ui->tbx_IDCliente->setText("");
    ui->tbx_NombreCliente->setText("");
    ui->tbx_EmpresaCliente->setText("");
}


void MainWindow::on_btn_BuscarEmpleado_clicked()
{
    string id = ui->tbx_BuscarEmpleado->text().toStdString();
    std::cerr << "ID a buscar: " << id;
    if(AdminEmpleados.buscarEmpleado(ui->tbl_Output, id)){
       QMessageBox::information(this, "Busqueda", "Empleado encontrado con exito!");
    } else {
       QMessageBox::warning(this, "Busqueda", "Empleado no encontrado.");
    }

}


void MainWindow::on_pB_SchInventory_clicked()
{
    string Producto = ui->tbx_BuscarProductoEnInventario->text().toStdString();

    if(AdministradorInventario.searchProduct(&Producto)){
    // Si se encuentra el producto, seleccionar la fila en la tabla
        for (int i = 0; i < ui->tbl_Inventario->rowCount(); i++) {
            QString idCelda = ui->tbl_Inventario->item(i, 0)->text();
            if (idCelda == QString::fromStdString(Producto)) {
                ui->tbl_Inventario->selectRow(i);
                break;
            }
        }
    } else {
        // Si no se encuentra el producto, mostrar un mensaje de error
        QMessageBox::information(this, "Producto no encontrado", "No se encontró el producto con ID: " + QString::fromStdString(Producto));
    }
}



void MainWindow::on_PB_Vender_clicked()
{

}


void MainWindow::on_PB_CrearPedido_clicked()
{

}


void MainWindow::on_btn_CargarCliente_clicked()
{
    AdministradorInventario.CargarClientes("Clientes.a");
    ActualizarTablaClientes();
}


void MainWindow::on_btn_BuscarCliente_clicked()
{
    string idSearched = ui->tbx_BuscarClientes->text().trimmed().toStdString();
    if(idSearched.empty()){
        QMessageBox::warning(this, "Búsqueda", "Ingrese un ID para buscar.");
        return;
    }

    Cliente* clienteEncontrado = AdministradorInventario.BuscarClientePorID(idSearched);
    if(!clienteEncontrado){
        QMessageBox::information(this, "Búsqueda", "Cliente no encontrado.");
        return;
    }

    for (int row = 0; row < ui->tbl_Clientes->rowCount(); ++row) {
        QTableWidgetItem* item = ui->tbl_Clientes->item(row, 0); // Columna 0 = ID
        if (item && item->text().toStdString() == idSearched) {
            ui->tbl_Clientes->setCurrentCell(row, 0); // Seleccionar la celda
            ui->tbl_Clientes->selectRow(row); // Resaltar toda la fila
            return;
        }
    }

    // Si no se encuentra en la tabla
    QMessageBox::warning(this, "Búsqueda", "Cliente encontrado en la lista, pero no en la tabla.");
}


void MainWindow::on_PB_SaveV_clicked()
{

}

