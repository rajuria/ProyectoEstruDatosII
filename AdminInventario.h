#ifndef ADMININVENTARIO_H
#define ADMININVENTARIO_H

#include<string>
#include<vector>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>

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

struct Venta
{
    string IDVenta;
    vector<Producto>ProductosVendidos;
    Cliente IDCliente;
    string Vendedor;

    Venta(string IDVenta, vector<Producto> ProductosVendidos, Cliente IDCliente, string Vendedor)
    {
        this->IDVenta=IDVenta;
        this->ProductosVendidos=ProductosVendidos;
        this->IDCliente=IDCliente;
        this->Vendedor=Vendedor;
    }
};

class AdminInventario {
public:
    vector<Producto> products;
    vector<Cliente> Clientes;
    void addProduct(Producto newProduct);
    void updateInventory(string productID, int cantidad);
    Producto* searchProduct(const string* productID);
    void deleteProduct(const string* productID);
    void createReport(const string& filename = "Inventario.csv") const;
    void GuardarDatos(const string& filename= "Inventario.a");
    void CargarDatos(const string& filename= "Inventario.a");
    void LowStockAlert(int minimum = 5) const;

    void GuardarClientes(const string& filename= "Clientes.a");
    void CargarClientes(const string& filename= "Clientes.a");
};

inline void AdminInventario::addProduct(Producto newProduct)
{
    products.push_back(newProduct);
}

inline void AdminInventario::updateInventory(std::string productID, int cantidad)
{
    Producto *product= searchProduct(&productID);
    if(product) {
        product->Cantidad += cantidad;
        std::cout<< "Inventario actualizado. Nuevo Stock: "<< product->Cantidad << "unidades\n";
    }else{
        std::cerr << "Producto no encontrado.";
    }

}

inline Producto* AdminInventario::searchProduct(const std::string *productID)
{
    for(auto& p : products){
        if(p.ID == *productID){
            return &p;
        }
    }
    return nullptr;
}

inline void AdminInventario::deleteProduct(const std::string *productID)
{
    // ... PENDIENDTE ...
}

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

inline void AdminInventario::GuardarClientes(const std::string &filename)
{
    HANDLE file = CreateFileA(filename.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE)
    {
        std::cerr << "No se pudo abrir el archivo\n";
        return;
    }
    DWORD written;
    size_t size = Clientes.size();
    WriteFile(file, &size, sizeof(size), &written, NULL);

    for (const auto& Cliente : Clientes)
    {//ForEach que recorre cada elemento dentro del vector de Empleados
        size_t idSize = Cliente.IDCliente.size();//Lee el tamano del string asignado a la variable.
        WriteFile(file, &idSize, sizeof(idSize), &written, NULL);//Escribe el tamano del string en el archivo
        WriteFile(file, Cliente.IDCliente.c_str(), idSize, &written, NULL);//Escribe los datos al archivo
        size_t nombreSize = Cliente.Nombre.size();
        WriteFile(file, &nombreSize, sizeof(nombreSize), &written, NULL);
        WriteFile(file, Cliente.Nombre.c_str(), nombreSize, &written, NULL);
        size_t empresaSize = Cliente.Empresa.size();
        WriteFile(file, &empresaSize, sizeof(nombreSize), &written, NULL);
        WriteFile(file, Cliente.Empresa.c_str(), empresaSize, &written, NULL);
    }
    CloseHandle(file);
}

inline void AdminInventario::CargarClientes(const std::string &filename)
{
    HANDLE file = CreateFileA(filename.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        std::cerr << "Error al abrir el archivo\n";
        return;
    }

    DWORD read;
    size_t size;
    ReadFile(file, &size, sizeof(size), &read, NULL);

    Clientes.resize(size);
    for (auto& Cliente : Clientes)
    {
        size_t idSize;
        ReadFile(file, &idSize, sizeof(idSize), &read, NULL);
        Cliente.IDCliente.resize(idSize);
        ReadFile(file, &Cliente.IDCliente[0], idSize, &read, NULL);
        size_t nombreSize;
        ReadFile(file, &nombreSize, sizeof(nombreSize), &read, NULL);
        Cliente.Nombre.resize(nombreSize);
        ReadFile(file, &Cliente.Nombre[0], nombreSize, &read, NULL);
        size_t empresaSize;
        ReadFile(file, &empresaSize, sizeof(empresaSize), &read, NULL);
        Cliente.Empresa.resize(empresaSize);
        ReadFile(file, &Cliente.Empresa[0], empresaSize, &read, NULL);
    }

    CloseHandle(file);
}

#endif // ADMININVENTARIO_H
