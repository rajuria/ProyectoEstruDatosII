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

class AdminInventario {
public:
    vector<Producto> products;

    void addProduct(Producto newProduct);
    void updateInventory(string productID, int cantidad);
    Producto* searchProduct(const string* productID);
    void deleteProduct(const string* productID);
    void createReport(const string& filename = "Inventario.csv") const;
    void LowStockAlert(int minimum = 5) const;
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
        std::cout << "No hay productos insuficientes." << std::endl;
    }
}

#endif // ADMININVENTARIO_H
