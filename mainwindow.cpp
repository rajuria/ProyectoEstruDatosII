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
    if(ID == "" || Nombre == "" || Departamento == "" || Puesto == "" || Contrasena == "" || Salario == 0.0){
        QMessageBox::warning(this, "Casillas vacias", "Debe llenar todas las casillas");
    }
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

void MainWindow::ActualizarTablaPedidos()
{
    ui->tbl_Pedidos->clear();
    ui->tbl_Pedidos->setColumnCount(6);
    ui->tbl_Pedidos->setHorizontalHeaderLabels(QStringList()<<"ID"<<"ID de Producto"<<"Cantidad"<<"ID del Cliente"<<"Estado"<<"Fecha de Entrega");
    ui->tbl_Pedidos->setRowCount(AdministradorInventario.Pedidos.size());

    for(uint i=0; i<AdministradorInventario.Pedidos.size();i++)
    {
        ui->tbl_Pedidos->setItem(i,0,new QTableWidgetItem(QString::fromStdString(AdministradorInventario.Pedidos[i].ID)));
        ui->tbl_Pedidos->setItem(i,1,new QTableWidgetItem(QString::fromStdString(AdministradorInventario.Pedidos[i].IDProducto)));
        ui->tbl_Pedidos->setItem(i,2,new QTableWidgetItem(QString::number(AdministradorInventario.Pedidos[i].Cantidad)));
        ui->tbl_Pedidos->setItem(i,3,new QTableWidgetItem(QString::fromStdString(AdministradorInventario.Pedidos[i].IDCliente)));
        ui->tbl_Pedidos->setItem(i,4,new QTableWidgetItem(QString::fromStdString(AdministradorInventario.Pedidos[i].Estado)));
        ui->tbl_Pedidos->setItem(i,5,new QTableWidgetItem(QString::fromStdString(AdministradorInventario.Pedidos[i].FechaDeEntrega)));
    }

}

void MainWindow::ActualizarTablaVentas()
{
    // Borrar todas las filas existentes en la tabla
    ui->TW_Listadeventas->clearContents();
    ui->TW_Listadeventas->setRowCount(0);
    AdministradorInventario.ventas.clear();

    // Inicializar los encabezados de la tabla
    ui->TW_Listadeventas->setColumnCount(9);
    ui->TW_Listadeventas->setHorizontalHeaderLabels(QStringList() << "ID Venta" << "Fecha" << "Producto" << "Cantidad"
                                                    << "ID Cliente" << "ID Vendedor" << "Subtotal"
                                                    << "Impuesto" << "Total");

    HANDLE file = CreateFileA("Ventas.a", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        std::cerr << "Error al abrir el archivo de ventas\n";
        return;
    }

    char buffer[1024];
    DWORD bytesRead;
    std::string linea;

    while (ReadFile(file, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0) {
        linea.append(buffer, bytesRead);
        size_t pos;
        while ((pos = linea.find('\n')) != std::string::npos) {
            std::string lineaCompleta = linea.substr(0, pos);
            linea.erase(0, pos + 1);

            std::stringstream ssLinea(lineaCompleta);
            std::string valor;
            std::vector<std::string> valores;

            while (std::getline(ssLinea, valor, ',')) {
                valores.push_back(valor);
            }

            if (valores.size() == 9) {
                int row = ui->TW_Listadeventas->rowCount();
                ui->TW_Listadeventas->insertRow(row);
                try {
                    ui->TW_Listadeventas->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(valores[0])));
                    ui->TW_Listadeventas->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(valores[1])));
                    ui->TW_Listadeventas->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(valores[2])));
                    ui->TW_Listadeventas->setItem(row, 3, new QTableWidgetItem(QString::number(std::stoi(valores[3]))));
                    ui->TW_Listadeventas->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(valores[4])));
                    ui->TW_Listadeventas->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(valores[5])));
                    ui->TW_Listadeventas->setItem(row, 6, new QTableWidgetItem(QString::number(std::stod(valores[6]))));
                    ui->TW_Listadeventas->setItem(row, 7, new QTableWidgetItem(QString::number(std::stod(valores[7]))));
                    ui->TW_Listadeventas->setItem(row, 8, new QTableWidgetItem(QString::number(std::stod(valores[8]))));

                    Venta Temp(valores[0],valores[1],valores[2],std::stoi(valores[3]),valores[4],valores[5],std::stod(valores[6])/std::stoi(valores[3]));
                    AdministradorInventario.ventas.push_back(Temp);
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Error al convertir cadena a número: " << e.what() << std::endl;
                }
            }
        }
    }
    CloseHandle(file);
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
    ActualizarTablaVentas();
    ui->stackedWidget->setCurrentIndex(6);
}


void MainWindow::on_GestionP_PB_clicked()
{
    AdministradorInventario.CargarPedidos();
    ActualizarTablaPedidos();
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
    if(ID == "" || Nombre == "" || Precio == 0.0 || Cantidad == 0){
        QMessageBox::warning(this, "Casillas vacias", "Debes llenar todas las casillas");
    }
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
    if(IDCliente == "" || Nombre == "" || Empresa == ""){
        QMessageBox::warning(this, "Casillas vacias", "Debes llenar todas las casillas!");
    }
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

        if(nombreProducto=="" || cantidadVendida == 0 || idCliente == "" || idVendedor == ""){
            QMessageBox::warning(this, "Elemento Vacio", "Debes llenar todas las casillas!");
            return;
        }

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

        AdministradorInventario.updateInventory(nombreProducto, cantidadVendida, this);
        ActualizarTablaVentas();

        QMessageBox::information(this, "Venta Realizada", "Venta registrada correctamente.");
}


void MainWindow::on_PB_CrearPedido_clicked()
{
    AdministradorInventario.CargarDatos();
    string ID=ui->Ln_IDPedido->text().toStdString();
    string IDProducto=ui->Ln_NameProduct->text().toStdString();
    int Cantidad=ui->SP_CantP_3->value();
    string IDCliente=ui->tbx_ClientePedido->text().toStdString();
    string FechaDeEntrega =ui->dateTimeEdit_P->date().toString("MM-dd-yyyy").toStdString();
    bool ProductoValido=false;
    bool SuficienteProducto=false;

    for(int i=0;i<AdministradorInventario.products.size();i++)
    {
        if(AdministradorInventario.products[i].ID==IDProducto)
        {
            ProductoValido=true;
            if(AdministradorInventario.products[i].Cantidad>=Cantidad)
            {
                SuficienteProducto=true;
            }
        }
    }

    if(ProductoValido && SuficienteProducto)
    {
        Pedido nuevo(ID,IDProducto,Cantidad,IDCliente,FechaDeEntrega);
        AdministradorInventario.Pedidos.push_back(nuevo);
        AdministradorInventario.GuardarPedidos();
        ActualizarTablaPedidos();

    }
    else if(!ProductoValido)
    {
        cerr<<"No Existe producto con ese ID";
    }
    else if(!SuficienteProducto)
    {
        cerr<<"No hay suficiente producto en inventario para ingresar la solicitud.";
    }
    else
    {
        cerr<<"Error Desconocido...";
    }

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
    if (ui->TW_Listadeventas->rowCount() == 0) {
        QMessageBox::warning(this, "Error", "No hay datos en la tabla para guardar.");
        return;
    }

    HANDLE file = CreateFileA("Ventas.a", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); // Usar CREATE_ALWAYS para sobrescribir el archivo
    if (file == INVALID_HANDLE_VALUE) {
        std::cerr << "Error al abrir el archivo para guardar las ventas.\n";
        return;
    }

    DWORD written;

    for (int i = 0; i < ui->TW_Listadeventas->rowCount(); ++i) {
        Venta venta;
        venta.IDVenta = ui->TW_Listadeventas->item(i, 0)->text().toStdString();
        venta.fecha = ui->TW_Listadeventas->item(i, 1)->text().toStdString();
        venta.nombreP = ui->TW_Listadeventas->item(i, 2)->text().toStdString();
        venta.cant = ui->TW_Listadeventas->item(i, 3)->text().toInt();
        venta.IDCliente = ui->TW_Listadeventas->item(i, 4)->text().toStdString();
        venta.IDVendedor = ui->TW_Listadeventas->item(i, 5)->text().toStdString();
        venta.subtotal = ui->TW_Listadeventas->item(i, 6)->text().toDouble();
        venta.impuesto = ui->TW_Listadeventas->item(i, 7)->text().toDouble();
        venta.total = ui->TW_Listadeventas->item(i, 8)->text().toDouble();

        std::stringstream ss;
        ss << venta.IDVenta << "," << venta.fecha << "," << venta.nombreP << "," << venta.cant << ","
           << venta.IDCliente << "," << venta.IDVendedor << "," << venta.subtotal << ","
           << venta.impuesto << "," << venta.total << "\n";
        std::string linea = ss.str();

        WriteFile(file, linea.c_str(), linea.length(), &written, NULL);
    }

    CloseHandle(file);
    QMessageBox::information(this, "Datos guardados", "Las ventas han sido guardadas correctamente.");
}


void MainWindow::on_PB_CargarV_clicked()
{
    ActualizarTablaVentas();
}

void MainWindow::on_PB_BuscarV_clicked()
{
    QString nombrePV = ui->Ln_SearchV->text();
    QTableWidget *tablaVentas = ui->TW_Listadeventas;

    // Deseleccionar todas las filas y restablecer el color de fondo
    for (int fila = 0; fila < tablaVentas->rowCount(); ++fila) {
        for (int columna = 0; columna < tablaVentas->columnCount(); ++columna) {
            QTableWidgetItem *item = tablaVentas->item(fila, columna);
            if (item) {
                item->setSelected(false); // Deseleccionar el item
                item->setBackground(QBrush(Qt::white)); // Restablecer el color de fondo a blanco
            }
        }
    }

    // Buscar y resaltar las filas que coinciden con el nombre del producto
    for (int fila = 0; fila < tablaVentas->rowCount(); ++fila) {
        QTableWidgetItem *itemProducto = tablaVentas->item(fila, 2); // Columna 2 es el nombre del producto
        if (itemProducto && itemProducto->text().contains(nombrePV, Qt::CaseInsensitive)) {
            // Resaltar la fila
            for (int columna = 0; columna < tablaVentas->columnCount(); ++columna) {
                QTableWidgetItem *item = tablaVentas->item(fila, columna);
                if (item) {
                    item->setSelected(true); // Seleccionar el item
                    item->setBackground(QBrush(Qt::yellow)); // Resaltar con color amarillo
                }
            }
        }
    }
}


void MainWindow::on_PB_ReporteVentas_clicked()
{
    AdministradorInventario.GenerarReporteDeVentas();
}


void MainWindow::on_PB_Completado_clicked()
{
    string ID = ui->tbl_Pedidos->selectedItems().at(0)->text().toStdString();
    for(int i=0;i<AdministradorInventario.Pedidos.size();i++)
    {
        if(ID==AdministradorInventario.Pedidos[i].ID)
        {
           AdministradorInventario.Pedidos[i].Estado="Completado";
           AdministradorInventario.GuardarPedidos();
           ActualizarTablaPedidos();
        }
    }
}


void MainWindow::on_PB_Completado_2_clicked()
{
    string ID = ui->tbl_Pedidos->selectedItems().at(0)->text().toStdString();
    for(int i=0;i<AdministradorInventario.Pedidos.size();i++)
    {
        if(ID==AdministradorInventario.Pedidos[i].ID)
        {
           AdministradorInventario.Pedidos[i].Estado="Cancelado";
           AdministradorInventario.GuardarPedidos();
           ActualizarTablaPedidos();
        }
    }
}


void MainWindow::on_btn_DesactivarEmpleado_clicked()
{
    string ID = ui->tbl_Output->selectedItems().at(0)->text().toStdString();
    for(int i=0; i<AdminEmpleados.Empleados.size(); i++)
    {
        if(ID==AdminEmpleados.Empleados[i].ID)
        {
            AdminEmpleados.Empleados[i].Activo=false;
            AdminEmpleados.GuardarDatos();
            ReloadTable();
        }
    }
}


void MainWindow::on_GestionI_PB_2_clicked()
{
    ActualizarProductos();
    ui->stackedWidget->setCurrentIndex(4);
}


void MainWindow::on_GestionV_PB_2_clicked()
{
    ActualizarTablaVentas();
    ui->stackedWidget->setCurrentIndex(6);
}


void MainWindow::on_GestionP_PB_2_clicked()
{
    AdministradorInventario.CargarPedidos();
    ActualizarTablaPedidos();
    ui->stackedWidget->setCurrentIndex(7);
}


void MainWindow::on_PB_Salir_2_clicked()
{
     ui->stackedWidget->setCurrentIndex(0);
}

