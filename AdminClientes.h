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
    string Correo;
    string HistorialDeCompras;
    int Telefono;
    double Saldo;

    Cliente() : Saldo(0.0) {}
    Cliente(string ID, string Nombre, string Correo, string HDC, double Telefono)
    {
        this->id=ID;
        this->Nombre=Nombre;
        this->Correo=Correo;
        this->HistorialDeCompras=HDC;
        this->Telefono=Telefono;
    }
};

class AdminClientes {
public:
    vector<Cliente> clients;
    void SaveData(const string& filename= "Clients.a");
    void LoadData(const string& filename= "Clients.a");
    void Report(const string& filename= "Clients.csv") const;
    void AddClient(Cliente newClient);
};

#endif // ADMINCLIENTES_H
