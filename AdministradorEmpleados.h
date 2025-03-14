#ifndef ADMINISTRADOREMPLEADOS_H
#define ADMINISTRADOREMPLEADOS_H
#include<string>
#include<vector>

#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <QString>
#include <QTableWidget>

using std::string;
using std::vector;

struct Empleado
{
    string ID;
    string Nombre;
    string Departamento;
    string Puesto;
    string Contrasena;
    double Salario;
    bool Activo;

    Empleado() : Salario(0.0), Activo(false) {}
    Empleado(string ID, string Nombre, string Departamento, string Puesto,string Contrasena, double Salario)
    {
        this->ID=ID;
        this->Nombre=Nombre;
        this->Departamento=Departamento;
        this->Puesto=Puesto;
        this->Contrasena=Contrasena;
        this->Salario=Salario;
        this->Activo=true;
    }
};

class AdministradorEmpleados
{
public:

    vector<Empleado> Empleados;
    void GuardarDatos(const string& filename= "Empleados.a");
    void CargarDatos(const string& filename= "Empleados.a");
    void GenerarReporte(const string& filename= "Empleados.csv") const;
    void AgregarEmpleado(Empleado EmpleadoNuevo);
    string EncriptarContrasena(string Contrasena);
    string ObtenerEmpleado(const string& ID, const string& nombre);
    bool buscarEmpleado(QTableWidget* tabla, const string& id);

};

inline void AdministradorEmpleados::GuardarDatos(const string &filename)
{
    HANDLE file = CreateFileA(filename.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE)
    {
        std::cerr << "No se pudo abrir el archivo\n";
        return;
    }
    DWORD written;
    size_t size = Empleados.size();
    WriteFile(file, &size, sizeof(size), &written, NULL);
    for (const auto& Empleado : Empleados)
    {//ForEach que recorre cada elemento dentro del vector de Empleados
        size_t idSize = Empleado.ID.size();//Lee el tamano del string asignado a la variable.
        WriteFile(file, &idSize, sizeof(idSize), &written, NULL);//Escribe el tamano del string en el archivo
        WriteFile(file, Empleado.ID.c_str(), idSize, &written, NULL);//Escribe los datos al archivo
        size_t nombreSize = Empleado.Nombre.size();
        WriteFile(file, &nombreSize, sizeof(nombreSize), &written, NULL);
        WriteFile(file, Empleado.Nombre.c_str(), nombreSize, &written, NULL);
        size_t departamentoSize = Empleado.Departamento.size();
        WriteFile(file, &departamentoSize, sizeof(departamentoSize), &written, NULL);
        WriteFile(file, Empleado.Departamento.c_str(), departamentoSize, &written, NULL);
        size_t puestoSize = Empleado.Puesto.size();
        WriteFile(file, &puestoSize, sizeof(puestoSize), &written, NULL);
        WriteFile(file, Empleado.Puesto.c_str(), puestoSize, &written, NULL);
        size_t contrasenaSize = Empleado.Contrasena.size();
        WriteFile(file, &contrasenaSize, sizeof(contrasenaSize), &written, NULL);
        WriteFile(file, Empleado.Contrasena.c_str(), contrasenaSize, &written, NULL);
        WriteFile(file, &Empleado.Salario, sizeof(Empleado.Salario), &written, NULL); //Escribe directamente los datos numericos
        WriteFile(file, &Empleado.Activo, sizeof(Empleado.Activo), &written, NULL);
    }
    CloseHandle(file);
}

inline void AdministradorEmpleados::CargarDatos(const string &filename)
{
    HANDLE file = CreateFileA(filename.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        std::cerr << "Error al abrir el archivo\n";
        return;
    }

    DWORD read;
    size_t size;
    ReadFile(file, &size, sizeof(size), &read, NULL);

    Empleados.resize(size);
    for (auto& Empleado : Empleados)
    {
        size_t idSize;
        ReadFile(file, &idSize, sizeof(idSize), &read, NULL);
        Empleado.ID.resize(idSize);
        ReadFile(file, &Empleado.ID[0], idSize, &read, NULL);
        size_t nombreSize;
        ReadFile(file, &nombreSize, sizeof(nombreSize), &read, NULL);
        Empleado.Nombre.resize(nombreSize);
        ReadFile(file, &Empleado.Nombre[0], nombreSize, &read, NULL);

        size_t departamentoSize;
        ReadFile(file, &departamentoSize, sizeof(departamentoSize), &read, NULL);
        Empleado.Departamento.resize(departamentoSize);
        ReadFile(file, &Empleado.Departamento[0], departamentoSize, &read, NULL);

        size_t puestoSize;
        ReadFile(file, &puestoSize, sizeof(puestoSize), &read, NULL);
        Empleado.Puesto.resize(puestoSize);
        ReadFile(file, &Empleado.Puesto[0], puestoSize, &read, NULL);

        size_t contrasenaSize;
        ReadFile(file, &contrasenaSize, sizeof(contrasenaSize), &read, NULL);
        Empleado.Contrasena.resize(contrasenaSize);
        ReadFile(file, &Empleado.Contrasena[0], contrasenaSize, &read, NULL);
        ReadFile(file, &Empleado.Salario, sizeof(Empleado.Salario), &read, NULL);
        ReadFile(file, &Empleado.Activo, sizeof(Empleado.Activo), &read, NULL);
    }

    CloseHandle(file);
}

inline void AdministradorEmpleados::GenerarReporte(const std::string &filename) const
{
    std::ofstream file(filename);
    if (!file)
    {
        std::cerr << "Error al abrir el archivo\n";
        return;
    }
    file << "ID,Nombre,Departamento,Puesto,Salario,Estado\n";
    for (const auto& Empleado : Empleados)
    {
        file << Empleado.ID << ","
             << Empleado.Nombre << ","
             << Empleado.Departamento << ","
             << Empleado.Puesto << ","
             << Empleado.Salario << ","
             << (Empleado.Activo ? "Activo" : "De baja en la Empresa") << "\n";
    }
}

inline void AdministradorEmpleados::AgregarEmpleado(Empleado EmpleadoNuevo)
{
    Empleados.push_back(EmpleadoNuevo);
}

inline string AdministradorEmpleados::EncriptarContrasena(std::string Contrasena)
{
    QChar c;
    QString Encriptado;
    for(uint i=0;i<Contrasena.length();i++)
    {
        c=QString::fromStdString(Contrasena).at(i).toLatin1()+(980487/(980487%100))%10;
        Encriptado+=c;
    }
    return Encriptado.toStdString();
}

inline string AdministradorEmpleados::ObtenerEmpleado(const std::string &ID, const std::string &nombre)
{
    for(const auto& emp: Empleados){
        if(emp.ID == ID && emp.Nombre == nombre && emp.Activo){
            return emp.Puesto;
        }
    }
    return "";
}

inline bool AdministradorEmpleados::buscarEmpleado(QTableWidget *tabla, const std::string &id)
{
    for(int fila = 0; fila < tabla->rowCount(); fila++){
        QTableWidgetItem* item = tabla->item(fila, 0);
        if(item){
            string idTabla = item->text().trimmed().toStdString();
            std::cerr << "Comparando: " << idTabla << "con" << id;
            if(idTabla == id){
                tabla->selectRow(fila);
                return true;
            }
        }
    }
    return false;
}

#endif // ADMINISTRADOREMPLEADOS_H
