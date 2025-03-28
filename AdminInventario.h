#ifndef ADMININVENTARIO_H
#define ADMININVENTARIO_H

#include<string>
#include<vector>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <QComboBox>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <QFile>
#include <unordered_map>
#include "AdministradorEmpleados.h"
#include <QInputDialog>

using std::string;
using std::vector;

struct Producto{
    string ID;
    string Nombre;
    double Precio;
    int Cantidad;

    Producto() : Precio(0.0), Cantidad(0) {}
    Producto(string ID, string name, double price, int cant){
        this->ID = ID;
        Nombre = name;
        Precio = price;
        Cantidad = cant;
    }
    string getID() const{
        return ID;
    }
    string getNombre() const{
        return Nombre;
    }
};

struct Cliente
{
    string IDCliente;
    string Nombre;
    string Empresa;

    Cliente(string IDCliente, string Nombre, string Empresa)
    {
        this->IDCliente=IDCliente;
        this->Nombre=Nombre;
        this->Empresa=Empresa;
    }

    Cliente(): IDCliente(""), Nombre(""), Empresa("")
    {
    };
};

struct Pedido
{
    string ID;
    string IDProducto;
    int Cantidad;
    string IDCliente;
    string Estado;//Pendiente,Completado,Cancelado
    string FechaDeEntrega;

    Pedido(string ID, string IDProducto, int Cantidad, string IDCliente, string FechaDeEntrega)
    {
        this->ID=ID;
        this->IDProducto=IDProducto;
        this->Cantidad=Cantidad;
        this->IDCliente=IDCliente;
        this->Estado="Pendiente";
        this->FechaDeEntrega=FechaDeEntrega;
    }
    Pedido()
    {
        this->ID="";
        this->IDProducto="";
        this->Cantidad=0;
        this->IDCliente="";
        this->Estado="Pendiente";
        this->FechaDeEntrega="";
    }
};


struct Venta
{
    string IDVenta;
    string fecha;
    string nombreP;
    int cant;
    string IDVendedor;
    string IDCliente;
    double subtotal;
    double total;
    double impuesto;

    Venta(string IDVenta, string fecha, string nombreP, int cant, string IDCliente, string IDVendedor, double PrecioP )
    {
        this->IDVenta = IDVenta;
        this->fecha = fecha;
        this->cant = cant;
        this->IDVendedor = IDVendedor;
        this->nombreP = nombreP;
        this->IDCliente=IDCliente;
        subtotal = cant * PrecioP;
        impuesto = subtotal * 0.15;
        total = subtotal + impuesto;
    }
    Venta() :
    IDVenta(""), fecha(""), nombreP(""), cant(0), IDVendedor(""), IDCliente(""), subtotal(0.0), total(0.0)
    {
    };
};

class AdminInventario {
public:
    vector<Producto> products;
    void addProduct(Producto newProduct);
    void updateInventory(string productID, int cantidad, QWidget* parent);
    Producto* searchProduct(const string* productID);
    void createReport(const string& filename = "Inventario.csv") const;
    void GuardarDatos(const string& filename= "Inventario.a");
    void CargarDatos(const string& filename= "Inventario.a");
    void LowStockAlert(int minimum = 5) const;
    void GuardarInventario(const string& filename = "Inventario.a");

    vector<Cliente> Clientes;
    std::unordered_map<std::string, Cliente> ClientsMap;
    void GuardarClientes(const string& filename= "Clientes.a");
    void CargarClientes(const string& filename= "Clientes.a");
    Cliente* BuscarClientePorID(const std::string& id);
    std::vector<Cliente> ObtenerClientes() const;

    vector<Pedido> Pedidos;
    void CargarPedidos(const string& filename = "Pedidos.a");
    void GuardarPedidos(const string& filename = "Pedidos.a");

    vector<Venta> ventas;
    void CargarVentas(const string& filename = "Ventas.a");
    void GuardarVenta(const Venta& nuevaVenta, const string& filename = "Ventas.a");
    void GenerarReporteDeVentas(const string& filename= "Ventas.csv") const;
    double obtenerPrecioProducto(const string &nombre) const
    {
        for (const auto& producto : products) {
            if (producto.getNombre() == nombre) {
                return producto.Precio;
            }
        }
        return -1.0;
    }

    void showP(){
    if (products.empty()) {
           std::cout << "El inventario está vacío." << std::endl;
           return;
       }

       std::cout << "Inventario actual:" << std::endl;
       for (const auto& producto : products) {
           std::cout << "ID: " << producto.ID
                     << ", Nombre: " << producto.Nombre
                     << ", Precio: $" << producto.Precio
                     << ", Cantidad: " << producto.Cantidad << std::endl;
       }
    }

};

//Funcion inventario
inline void AdminInventario::addProduct(Producto newProduct)
{
    products.push_back(newProduct);
}

//Funcion inventario
inline void AdminInventario::updateInventory(std::string productID, int cantidad, QWidget* parent)
{
    // Buscar el producto por su ID
    for (auto& producto : products) {
        std::cout<<producto.getID()<<" - "<<productID<<"\n";
        if (producto.getNombre() == productID) {
            // Verificar si la cantidad vendida es menor o igual a la cantidad disponible
            if (producto.Cantidad >= cantidad) {
                std::cout << producto.Cantidad << " - " << cantidad << std::endl;
                producto.Cantidad -= cantidad;  // Restamos la cantidad vendida
                std::cout << producto.Cantidad << std::endl;
                std::cout << "Inventario actualizado para el producto: " << producto.Nombre << std::endl;
                // Verificar si el stock llegó a cero
                if (producto.Cantidad == 0) {
                    bool ok;
                    int cantidadAdicional = QInputDialog::getInt(parent, "Añadir Stock", "Cantidad a añadir:", 0, 0, 1000, 1, &ok);
                    if (ok) {
                        producto.Cantidad += cantidadAdicional;
                        std::cout << "Stock añadido para el producto: " << producto.Nombre << std::endl;
                        GuardarInventario();
                        return;
                    } else {
                        std::cerr << "Operación de añadir stock cancelada." << std::endl;
                        GuardarInventario(); // Guardar cambios aunque se cancele
                        return;
                    }
                } else if (producto.Cantidad < 0) {
                  std::cerr << "No hay suficiente cantidad en el inventario para este producto." << std::endl;
                  producto.Cantidad += cantidad; //Revertir la resta
                  return;
                }

                std::cout << "Inventario actualizado para el producto: " << producto.Nombre << std::endl;
                GuardarInventario();
                return;
            } else {
                std::cerr << "No hay suficiente cantidad en el inventario para este producto." << std::endl;
                // Mostrar InputDialog para solicitar la cantidad a añadir
                bool ok;
                int cantidadAdicional = QInputDialog::getInt(parent, "Añadir Stock", "Cantidad a añadir:", 0, 0, 1000, 1, &ok);
                if (ok) {
                    producto.Cantidad += cantidadAdicional; // Añadimos la cantidad ingresada
                    std::cout << "Stock añadido para el producto: " << producto.Nombre << std::endl;
                    GuardarInventario();
                    return;
                } else {
                    std::cerr << "Operación de añadir stock cancelada." << std::endl;
                    GuardarInventario(); // Guardar cambios aunque se cancele
                    return;
                }
                std::cout << "Inventario actualizado para el producto: " << producto.Nombre << std::endl;
                GuardarInventario();
                return;
                return;
            }
        }
    }

    std::cerr << "Producto no encontrado en el inventario." << std::endl;
}

//Funcion inventario
inline Producto* AdminInventario::searchProduct(const std::string *productID)
{
    for(auto& p : products){
        if(p.ID == *productID){
            return &p;
        }
    }
    return nullptr;
}

//Funcion inventario
inline void AdminInventario::createReport(const std::string &filename) const
{
    std::fstream file(filename);
    if(!file){
        std::cerr<<"Error al abrir el archivo." << std::endl;
    }
    file << "ID, Nombre, Precio, Cantidad\n";
    for(const auto& p : products){
        file << p.ID << ", "
             << p.Nombre << ", "
             << p.Precio << ", "
             << p.Cantidad << "\n ";
    }
    std::cout<< "Reporte de inventario generado con exito. \n";
}

//Funcion inventario
inline void AdminInventario::GuardarDatos(const std::string &filename)
{
    HANDLE file = CreateFileA(filename.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE)
    {
        std::cerr << "No se pudo abrir el archivo\n";
        return;
    }
    DWORD written;
    size_t size = products.size();
    WriteFile(file, &size, sizeof(size), &written, NULL);
    for (const auto& Producto : products)
    {//ForEach que recorre cada elemento dentro del vector de Empleados
        size_t idSize = Producto.ID.size();//Lee el tamano del string asignado a la variable.
        WriteFile(file, &idSize, sizeof(idSize), &written, NULL);//Escribe el tamano del string en el archivo
        WriteFile(file, Producto.ID.c_str(), idSize, &written, NULL);//Escribe los datos al archivo
        size_t nombreSize = Producto.Nombre.size();
        WriteFile(file, &nombreSize, sizeof(nombreSize), &written, NULL);
        WriteFile(file, Producto.Nombre.c_str(), nombreSize, &written, NULL);
        WriteFile(file, &Producto.Precio, sizeof(Producto.Precio), &written, NULL); //Escribe directamente los datos numericos
        WriteFile(file, &Producto.Cantidad, sizeof(Producto.Cantidad), &written, NULL);
    }
    CloseHandle(file);
}

//Funcion inventario
inline void AdminInventario::CargarDatos(const std::string &filename)
{
    HANDLE file = CreateFileA(filename.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        std::cerr << "Error al abrir el archivo\n";
        return;
    }

    DWORD read;
    size_t size;
    ReadFile(file, &size, sizeof(size), &read, NULL);

    products.resize(size);
    for (auto& Producto : products)
    {
        size_t idSize;
        ReadFile(file, &idSize, sizeof(idSize), &read, NULL);
        Producto.ID.resize(idSize);
        ReadFile(file, &Producto.ID[0], idSize, &read, NULL);
        size_t nombreSize;
        ReadFile(file, &nombreSize, sizeof(nombreSize), &read, NULL);
        Producto.Nombre.resize(nombreSize);
        ReadFile(file, &Producto.Nombre[0], nombreSize, &read, NULL);
        ReadFile(file, &Producto.Precio, sizeof(Producto.Precio), &read, NULL);
        ReadFile(file, &Producto.Cantidad, sizeof(Producto.Cantidad), &read, NULL);
    }

    CloseHandle(file);
}

//Funcion inventario
inline void AdminInventario::LowStockAlert(int minimum) const
{
    std::cout << "Productos con bajo stock (menos de " << minimum << "unidades): \n";
    bool LSAlert = false;
    for(const auto& producto: products){
        if(producto.Cantidad <= minimum){
            std::cout << " - " << producto.Nombre << "( " << producto.Cantidad << "unidades disponibles)\n";
            LSAlert = true;
        }
    }
    if(!LSAlert){
        std::cout << "No hay productos suficientes." << std::endl;
    }
}

inline void AdminInventario::GuardarInventario(const std::string &filename)
{
    HANDLE file = CreateFileA(filename.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        std::cerr << "Error al abrir el archivo de inventario\n";
        return;
    }

    DWORD written;
    size_t size = products.size();
    WriteFile(file, &size, sizeof(size), &written, NULL);

    for (const auto& producto : products) {
        size_t idSize = producto.ID.size();
        WriteFile(file, &idSize, sizeof(idSize), &written, NULL);
        WriteFile(file, producto.ID.c_str(), idSize, &written, NULL);

        size_t nombreSize = producto.Nombre.size();
        WriteFile(file, &nombreSize, sizeof(nombreSize), &written, NULL);
        WriteFile(file, producto.Nombre.c_str(), nombreSize, &written, NULL);

        WriteFile(file, &producto.Precio, sizeof(producto.Precio), &written, NULL);
        WriteFile(file, &producto.Cantidad, sizeof(producto.Cantidad), &written, NULL);
    }

    CloseHandle(file);
}

//Funcion clientes
inline void AdminInventario::GuardarClientes(const std::string &filename)
{
    // Intenta abrir el archivo para lectura, si no existe, lo creará
    HANDLE file = CreateFileA(filename.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (file == INVALID_HANDLE_VALUE) {
        // Si el archivo no existe, lo creamos
        file = CreateFileA(filename.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

        if (file == INVALID_HANDLE_VALUE) {
            std::cerr << "No se pudo abrir o crear el archivo\n";
            return;
        }

        // Escribir los nuevos datos si el archivo es creado
        std::cout << "El archivo fue creado correctamente, guardando los datos...\n";
    }

       std::vector<Cliente> clientesExistentes;
    if(file != INVALID_HANDLE_VALUE){
       // Leer los clientes ya guardados en el archivo
       DWORD read;
       size_t existingSize;
       if (ReadFile(file, &existingSize, sizeof(existingSize), &read, NULL))
       {
           for (size_t i = 0; i < existingSize; ++i)
           {
               Cliente cliente;
               size_t idSize, nombreSize, empresaSize;

               // Leer IDCliente
               if (ReadFile(file, &idSize, sizeof(idSize), &read, NULL))
               {
                   std::vector<char> idCliente(idSize);
                   if (ReadFile(file, idCliente.data(), idSize, &read, NULL))
                   {
                       cliente.IDCliente = std::string(idCliente.begin(), idCliente.end());
                   }
               }

               // Leer Nombre
               if (ReadFile(file, &nombreSize, sizeof(nombreSize), &read, NULL))
               {
                   std::vector<char> nombre(nombreSize);
                   if (ReadFile(file, nombre.data(), nombreSize, &read, NULL))
                   {
                       cliente.Nombre = std::string(nombre.begin(), nombre.end());
                   }
               }

               // Leer Empresa
               if (ReadFile(file, &empresaSize, sizeof(empresaSize), &read, NULL))
               {
                   std::vector<char> empresa(empresaSize);
                   if (ReadFile(file, empresa.data(), empresaSize, &read, NULL))
                   {
                       cliente.Empresa = std::string(empresa.begin(), empresa.end());
                   }
               }

               clientesExistentes.push_back(cliente);
           }
       }
    }

   // Cerrar el archivo después de leer
   CloseHandle(file);

   // Abrir el archivo en modo escritura para sobrescribir con los datos completos
   file = CreateFileA(filename.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

   if (file == INVALID_HANDLE_VALUE)
   {
       std::cerr << "No se pudo abrir el archivo para escritura\n";
       return;
   }

   // Combinar los clientes existentes con los nuevos
   clientesExistentes.insert(clientesExistentes.end(), Clientes.begin(), Clientes.end());

   // Escribir los nuevos clientes en el archivo
   size_t totalClientes = clientesExistentes.size();
   DWORD written;
   WriteFile(file, &totalClientes, sizeof(totalClientes), &written, NULL);

   for (const auto& cliente : clientesExistentes)
   {
       size_t idSize = cliente.IDCliente.size();
       WriteFile(file, &idSize, sizeof(idSize), &written, NULL);
       WriteFile(file, cliente.IDCliente.c_str(), idSize, &written, NULL);

       size_t nombreSize = cliente.Nombre.size();
       WriteFile(file, &nombreSize, sizeof(nombreSize), &written, NULL);
       WriteFile(file, cliente.Nombre.c_str(), nombreSize, &written, NULL);

       size_t empresaSize = cliente.Empresa.size();
       WriteFile(file, &empresaSize, sizeof(empresaSize), &written, NULL);
       WriteFile(file, cliente.Empresa.c_str(), empresaSize, &written, NULL);
   }

   CloseHandle(file);
}

//Funcion clientes
inline void AdminInventario::CargarClientes(const std::string &filename)
{
    // Abrir el archivo
   HANDLE file = CreateFileA(filename.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
   if (file == INVALID_HANDLE_VALUE) {
       // Si no puede abrirse el archivo porque no existe o tiene problemas, lo crea
       file = CreateFileA(filename.c_str(), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
       if (file == INVALID_HANDLE_VALUE) {
           std::cerr << "No se pudo crear o abrir el archivo\n";
           return;
       }
       // El archivo está vacío, puedes escribir los datos nuevos directamente
       std::cout << "El archivo estaba vacío. Procediendo a crear nuevos datos...\n";
       size_t size = Clientes.size();
       WriteFile(file, &size, sizeof(size), NULL, NULL);

       for (const Cliente& cliente : Clientes) {
           size_t idSize = cliente.IDCliente.size();
           size_t nombreSize = cliente.Nombre.size();
           size_t empresaSize = cliente.Empresa.size();

           WriteFile(file, &idSize, sizeof(idSize), NULL, NULL);
           WriteFile(file, cliente.IDCliente.c_str(), idSize, NULL, NULL);

           WriteFile(file, &nombreSize, sizeof(nombreSize), NULL, NULL);
           WriteFile(file, cliente.Nombre.c_str(), nombreSize, NULL, NULL);

           WriteFile(file, &empresaSize, sizeof(empresaSize), NULL, NULL);
           WriteFile(file, cliente.Empresa.c_str(), empresaSize, NULL, NULL);
       }

       CloseHandle(file);
       return;
   }

   DWORD read;
   size_t size;
   ReadFile(file, &size, sizeof(size), &read, NULL);

   if (size == 0) {
       std::cerr << "El archivo está vacío.\n";
       CloseHandle(file);
       return; // Salimos si no hay datos
   }

   // Vaciamos la lista de clientes antes de cargar los datos
   Clientes.clear();

   // Leer los datos del archivo y cargarlos en la lista
   for (size_t i = 0; i < size; ++i) {
       size_t idSize, nombreSize, empresaSize;
       ReadFile(file, &idSize, sizeof(idSize), &read, NULL);
       std::string id(idSize, '\0');
       ReadFile(file, &id[0], idSize, &read, NULL);

       ReadFile(file, &nombreSize, sizeof(nombreSize), &read, NULL);
       std::string nombre(nombreSize, '\0');
       ReadFile(file, &nombre[0], nombreSize, &read, NULL);

       ReadFile(file, &empresaSize, sizeof(empresaSize), &read, NULL);
       std::string empresa(empresaSize, '\0');
       ReadFile(file, &empresa[0], empresaSize, &read, NULL);

       Cliente nuevoCliente(id, nombre, empresa);
       Clientes.push_back(nuevoCliente); // Agregar los clientes leídos del archivo
   }

   CloseHandle(file);

   ClientsMap.clear();
   for (const auto& cliente : Clientes) {
       ClientsMap[cliente.IDCliente] = cliente;
   }
}

inline Cliente *AdminInventario::BuscarClientePorID(const std::string &id)
{
    auto it = ClientsMap.find(id);
        if (it != ClientsMap.end()) {
            return &it->second; // Retorna un puntero al cliente encontrado
        }
        return nullptr; // No encontrado
}

inline std::vector<Cliente> AdminInventario::ObtenerClientes() const
{
    return Clientes;
}

inline void AdminInventario::CargarPedidos(const std::string &filename)
{
    HANDLE file = CreateFileA(filename.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        std::cerr << "Error al abrir el archivo\n";
        return;
    }

    DWORD read;
    size_t size;
    ReadFile(file, &size, sizeof(size), &read, NULL);

    Pedidos.resize(size);
    for (auto& Pedido : Pedidos)
    {
        size_t idSize;
        ReadFile(file, &idSize, sizeof(idSize), &read, NULL);
        Pedido.ID.resize(idSize);
        ReadFile(file, &Pedido.ID[0], idSize, &read, NULL);
        size_t IDProductoSize;
        ReadFile(file, &IDProductoSize, sizeof(IDProductoSize), &read, NULL);
        Pedido.IDProducto.resize(IDProductoSize);
        ReadFile(file, &Pedido.IDProducto[0], IDProductoSize, &read, NULL);
        ReadFile(file, &Pedido.Cantidad, sizeof(Pedido.Cantidad), &read, NULL);
        size_t IDClienteSize;
        ReadFile(file, &IDClienteSize, sizeof(IDClienteSize), &read, NULL);
        Pedido.IDCliente.resize(IDClienteSize);
        ReadFile(file, &Pedido.IDCliente[0], IDClienteSize, &read, NULL);
        size_t EstadoSize;
        ReadFile(file, &EstadoSize, sizeof(EstadoSize), &read, NULL);
        Pedido.Estado.resize(EstadoSize);
        ReadFile(file, &Pedido.Estado[0], EstadoSize, &read, NULL);
        size_t FechaSize;
        ReadFile(file, &FechaSize, sizeof(FechaSize), &read, NULL);
        Pedido.FechaDeEntrega.resize(FechaSize);
        ReadFile(file, &Pedido.FechaDeEntrega[0], FechaSize, &read, NULL);
    }

    CloseHandle(file);
}

inline void AdminInventario::GuardarPedidos(const std::string &filename)
{
    HANDLE file = CreateFileA(filename.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE)
    {
        std::cerr << "No se pudo abrir el archivo\n";
        return;
    }
    DWORD written;
    size_t size = Pedidos.size();
    WriteFile(file, &size, sizeof(size), &written, NULL);
    for (const auto& Pedido : Pedidos)
    {//ForEach que recorre cada elemento dentro del vector de Empleados
        size_t idSize = Pedido.ID.size();//Lee el tamano del string asignado a la variable.
        WriteFile(file, &idSize, sizeof(idSize), &written, NULL);//Escribe el tamano del string en el archivo
        WriteFile(file, Pedido.ID.c_str(), idSize, &written, NULL);//Escribe los datos al archivo
        size_t IDProductoSize = Pedido.IDProducto.size();
        WriteFile(file, &IDProductoSize, sizeof(IDProductoSize), &written, NULL);
        WriteFile(file, Pedido.IDProducto.c_str(), IDProductoSize, &written, NULL);
        WriteFile(file, &Pedido.Cantidad, sizeof(Pedido.Cantidad), &written, NULL);
        size_t IDClienteSize = Pedido.IDCliente.size();
        WriteFile(file, &IDClienteSize, sizeof(IDClienteSize), &written, NULL);
        WriteFile(file, Pedido.IDCliente.c_str(), IDClienteSize, &written, NULL);
        size_t EstadoSize = Pedido.Estado.size();
        WriteFile(file, &EstadoSize, sizeof(EstadoSize), &written, NULL);
        WriteFile(file, Pedido.Estado.c_str(), EstadoSize, &written, NULL);
        size_t FechaSize = Pedido.FechaDeEntrega.size();
        WriteFile(file, &FechaSize, sizeof(FechaSize), &written, NULL);
        WriteFile(file, Pedido.FechaDeEntrega.c_str(), FechaSize, &written, NULL);
    }
    CloseHandle(file);
}

inline void AdminInventario::CargarVentas(const std::string &filename)
{
    HANDLE file = CreateFileA(filename.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        std::cerr << "Error al abrir el archivo de ventas\n";
        return;
    }

    char buffer[1024];
    DWORD bytesRead;
    std::string linea;

    ventas.clear(); // Limpiar el vector antes de cargar nuevos datos

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
                try {
                    Venta venta;
                    venta.IDVenta = valores[0];
                    venta.fecha = valores[1];
                    venta.nombreP = valores[2];
                    venta.cant = std::stoi(valores[3]);
                    venta.IDCliente = valores[4];
                    venta.IDVendedor = valores[5];
                    venta.subtotal = std::stod(valores[6]);
                    venta.impuesto = std::stod(valores[7]);
                    venta.total = std::stod(valores[8]);
                    ventas.push_back(venta);
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Error al convertir cadena a número: " << e.what() << std::endl;
                }
            }
        }
    }
    CloseHandle(file);
}

inline void AdminInventario::GuardarVenta(const Venta &nuevaVenta, const std::string &filename)
{
    Producto* producto = nullptr;
        for (auto &p : products) {
            if (p.getNombre() == nuevaVenta.nombreP) {
                producto = &p;
                break;
            }
        }

        if (!producto) {
            std::cerr << "Producto no encontrado en inventario\n";
            return;
        }

        if (producto->Cantidad < nuevaVenta.cant) {
            std::cerr << "No hay suficiente cantidad en inventario para realizar la venta.\n";
            return;
        }

        ventas.push_back(nuevaVenta);

        // Guardar en archivo como texto delimitado por comas
        HANDLE file = CreateFileA(filename.c_str(), GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (file == INVALID_HANDLE_VALUE) {
            std::cerr << "Error al abrir el archivo de ventas\n";
            return;
        }

        SetFilePointer(file, 0, NULL, FILE_END);

        DWORD written;
        std::stringstream ss;
        ss << nuevaVenta.IDVenta << "," << nuevaVenta.fecha << "," << nuevaVenta.nombreP << ","
           << nuevaVenta.cant << "," << nuevaVenta.IDCliente << "," << nuevaVenta.IDVendedor << ","
           << nuevaVenta.subtotal << "," << nuevaVenta.impuesto << "," << nuevaVenta.total << "\n";
        std::string linea = ss.str();

        WriteFile(file, linea.c_str(), linea.length(), &written, NULL);

        CloseHandle(file);
}

inline void AdminInventario::GenerarReporteDeVentas(const std::string &filename) const
{
    std::ofstream file(filename);
    if (!file)
    {
        std::cerr << "Error al abrir el archivo\n";
        return;
    }
    file << "ID,Fecha,Producto,Cantidad,Vendedor,Cliente,SubTotal,Impuestos,Total\n";
    for (const auto& Venta : ventas)
    {
        file << Venta.IDVenta << ","
             << Venta.fecha << ","
             << Venta.nombreP << ","
             << Venta.cant << ","
             << Venta.IDVendedor << ","
             << Venta.IDCliente << ","
             << Venta.subtotal << ","
             << Venta.impuesto << ","
             << Venta.total << "\n";
    }
}

#endif // ADMININVENTARIO_H
