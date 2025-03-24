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

string MainWindow::generateIDVenta()
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y%m%d%H%M%S", &tstruct);
    return "V" + std::string(buf);
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
            ui->stackedWidget->setCurrentIndex(2);
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
    AdministradorInventario.showP();
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
    AdministradorInventario.CargarDatos();
    // Obtener los datos de la venta desde el UI
        string nombreProducto = ui->Ln_NameP->text().toStdString();
        int cantidadVendida = ui->SP_CantP->value();
        string idCliente = ui->Ln_IDCliente->text().toStdString();
        string idVendedor = ui->Ln_IDSeller->text().toStdString();
        QDate fecha = ui->dateTimeEdit->date();
        double precioProducto = AdministradorInventario.obtenerPrecioProducto(nombreProducto);
        double subtotal = cantidadVendida * precioProducto;
        double impuesto = subtotal * 0.15;  // Impuesto del 15%
        double total = subtotal + impuesto;

        Venta nuevaVenta;
        nuevaVenta.IDVenta = generateIDVenta();
        nuevaVenta.fecha = fecha.toString("MM-dd-yyyy").toStdString();
        nuevaVenta.nombreP = nombreProducto;
        nuevaVenta.cant = cantidadVendida;
        nuevaVenta.IDCliente = idCliente;
        nuevaVenta.IDVendedor = idVendedor;
        nuevaVenta.subtotal = subtotal;
        nuevaVenta.impuesto = impuesto;
        nuevaVenta.total = total;
        AdministradorInventario.GuardarVenta(nuevaVenta);

        AdministradorInventario.updateInventory(nombreProducto, cantidadVendida);

        QMessageBox::information(this, "Venta Realizada", "Venta registrada correctamente.");
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

    // Verificar si la tabla no está vacía
    if (ui->TW_Listadeventas->rowCount() == 0) {
        QMessageBox::warning(this, "Error", "No hay datos en la tabla para guardar.");
        return;  // Si la tabla está vacía, no hacer nada
    }

    // Abrir el archivo en modo de escritura, eliminando su contenido
    HANDLE file = CreateFileA("Ventas.a", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        std::cerr << "Error al abrir el archivo para guardar las ventas.\n";
        return;
    }

    DWORD written;

    // Recorrer todas las filas de la tabla y escribirlas en el archivo
    for (int i = 0; i < ui->TW_Listadeventas->rowCount(); ++i) {
        Venta venta;

        // Leer los valores de la fila en la tabla y asignarlos al objeto 'Venta'
        venta.IDVenta = ui->TW_Listadeventas->item(i, 0)->text().toStdString();
        venta.fecha = ui->TW_Listadeventas->item(i, 1)->text().toStdString();
        venta.nombreP = ui->TW_Listadeventas->item(i, 2)->text().toStdString();
        venta.cant = ui->TW_Listadeventas->item(i, 3)->text().toInt();
        venta.IDCliente = ui->TW_Listadeventas->item(i, 4)->text().toStdString();
        venta.IDVendedor = ui->TW_Listadeventas->item(i, 5)->text().toStdString();
        venta.subtotal = ui->TW_Listadeventas->item(i, 6)->text().toDouble();
        venta.impuesto = ui->TW_Listadeventas->item(i, 7)->text().toDouble();
        venta.total = ui->TW_Listadeventas->item(i, 8)->text().toDouble();

        // Escribir los datos de la venta en el archivo
        WriteFile(file, &venta.IDVenta, sizeof(venta.IDVenta), &written, NULL);
        WriteFile(file, &venta.fecha, sizeof(venta.fecha), &written, NULL);
        WriteFile(file, &venta.nombreP, sizeof(venta.nombreP), &written, NULL);
        WriteFile(file, &venta.cant, sizeof(venta.cant), &written, NULL);
        WriteFile(file, &venta.IDCliente, sizeof(venta.IDCliente), &written, NULL);
        WriteFile(file, &venta.IDVendedor, sizeof(venta.IDVendedor), &written, NULL);
        WriteFile(file, &venta.subtotal, sizeof(venta.subtotal), &written, NULL);
        WriteFile(file, &venta.impuesto, sizeof(venta.impuesto), &written, NULL);
        WriteFile(file, &venta.total, sizeof(venta.total), &written, NULL);
    }

    // Cerrar el archivo después de escribir los datos
    CloseHandle(file);

    QMessageBox::information(this, "Datos guardados", "Las ventas han sido guardadas correctamente.");
}


void MainWindow::on_PB_CargarV_clicked()
{
    // Limpiar la tabla antes de cargar los datos
        ui->TW_Listadeventas->clearContents();
        ui->TW_Listadeventas->setColumnCount(9);
        ui->TW_Listadeventas->setHorizontalHeaderLabels(QStringList()<<"ID Venta" << "Fecha" << "Producto" << "Cantidad"
                                                        << "ID Cliente" << "ID Vendedor" << "Subtotal"
                                                        << "Impuesto" << "Total");
        for (const Venta& venta : AdministradorInventario.ventas) {
            int row = ui->TW_Listadeventas->rowCount();  // Obtener la última fila disponible
            ui->TW_Listadeventas->insertRow(row);  // Insertar una nueva fila

            // Insertar los valores de la venta en la tabla
            ui->TW_Listadeventas->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(venta.IDVenta)));
            ui->TW_Listadeventas->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(venta.fecha)));
            ui->TW_Listadeventas->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(venta.nombreP)));
            ui->TW_Listadeventas->setItem(row, 3, new QTableWidgetItem(QString::number(venta.cant)));
            ui->TW_Listadeventas->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(venta.IDCliente)));
            ui->TW_Listadeventas->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(venta.IDVendedor)));
            ui->TW_Listadeventas->setItem(row, 6, new QTableWidgetItem(QString::number(venta.subtotal)));
            ui->TW_Listadeventas->setItem(row, 7, new QTableWidgetItem(QString::number(venta.impuesto)));
            ui->TW_Listadeventas->setItem(row, 8, new QTableWidgetItem(QString::number(venta.total)));
        }
}

