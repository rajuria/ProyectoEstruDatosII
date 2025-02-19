#ifndef ADMINCLIENTES_H
#define ADMINCLIENTES_H
#include<string>
#include<vector>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>

using std::string;
using std::vector;

struct Cliente {
    string id;
    string Nombre;
    string Contacto;

    Cliente() {}
    Cliente(string ID, string Nombre, string Contacto)
    {
        this->id = ID;
        this->Nombre = Nombre;
        this->Contacto = Contacto;
    }
};

struct Factura {
    Cliente cliente;
    vector<string> ProductoID;
    double Total;

    Factura () : Total (0.0) {}
    Factura(Cliente cliente, vector<string> Product, double Total) : cliente(cliente), ProductoID(Product), Total(Total) {}
};

class AdminClientes {
public:
    vector<Cliente> clients;
    vector<Factura> receipts;
    void addClient(Cliente nuevoCliente);
    void buy(string clienteD, vector<string> Porductos, double total);
    void createReceipt(const string &filename = "receipts.csv") const;
};

inline void AdminClientes::addClient(Cliente nuevoCliente)
{
    clients.push_back(nuevoCliente);
}

inline void AdminClientes::buy(std::string clienteID, vector<std::string> Productos, double total)
{
    bool clientFound = false;
    Cliente* cliente = nullptr;
    for(auto& c: clients){
        if(c.id == clienteID){
            cliente = &c;
            clientFound = true;
            break;
        }
    }
    if(!cliente){
        clientFound = false;
    }
    receipts.emplace_back(*cliente, Productos, total);
}

inline void AdminClientes::createReceipt(const std::string &filename) const
{
    std::fstream file(filename);
    if(!file){
        std::cerr<< "Archivo no encontrado";
        return;
    }
    file << "ID Cliente, Nombre, Productos Comprados, Total \n";
    for(const auto& f : receipts){
        file << f.cliente.id << ", " << f.cliente.Nombre << ", ";
        for(const auto& p: f.ProductoID){
            file << p << " | ";
        }
        file << ", " << f.Total << "\n";
    }
}

#endif // ADMINCLIENTES_H
