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

inline void AdminClientes::buy(std::string clienteD, vector<std::string> Porductos, double total)
{

}

#endif // ADMINCLIENTES_H
