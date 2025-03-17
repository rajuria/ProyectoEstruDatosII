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

class BTreeNode {
public:
    bool leaf;
    vector<Empleado> keys;
    vector<BTreeNode*> children;
    BTreeNode(bool leaf){
        this->leaf = leaf;
    }
};

class BTree {
public:
    BTreeNode* root;
    int t;
    BTree(int t){
        this->t = t;
        root = new BTreeNode(true);
    }
    void insert(Empleado k);
    void traverse();
    Empleado* searchB(string ID);
private:
    void insertNonFull(BTreeNode* node, Empleado k);
    void splitChild(BTreeNode* parent, int i, BTreeNode* child);
};

inline void BTree::insert(Empleado k)
{
    if (static_cast<int>(root->keys.size()) == (2 * t - 1)){
        BTreeNode* s = new BTreeNode(false);
        s->children.push_back(root);
        splitChild(s,0,root);
        root = s;
    }
    insertNonFull(root,k);
}

inline Empleado *BTree::searchB(std::string ID)
{
    BTreeNode* node = root;
    while (node) {
        int i = 0;
        while (i < static_cast<int>(node->keys.size()) && ID > node->keys[i].ID) {
            i++;
        }
        if (i < static_cast<int>(node->keys.size()) && ID == node->keys[i].ID) {
            return &node->keys[i];
        }
        if (node->leaf) {
            return nullptr;
        }
        node = node->children[i];
    }
    return nullptr;
}

inline void BTree::insertNonFull(BTreeNode *node, Empleado k)
{
    int i = node->keys.size() - 1;
    if(node->leaf){
        node->keys.push_back(k);
        while(i >=0 && node->keys[i].ID > k.ID){
            node->keys[i+1] = node->keys[i];
            i--;
        }
        node->keys[i+1] = k;
    } else {
        while ( i >= 0 && node->keys[i].ID > k.ID){
            i--;
        }
        i++;
        if (static_cast<int>(node->children[i]->keys.size()) == (2 * t - 1)){
            splitChild(node, i, node->children[i]);
            if(node->keys[i].ID < k.ID){
                i++;
            }
            insertNonFull(node->children[i], k);
        }
    }
}

inline void BTree::splitChild(BTreeNode *parent, int i, BTreeNode *child)
{
    BTreeNode* newChild = new BTreeNode(child->leaf);
    for (int j = 0; j < t - 1; j++) {
        newChild->keys.push_back(child->keys[j + t]);
    }
    if (!child->leaf) {
        for (int j = 0; j < t; j++) {
            newChild->children.push_back(child->children[j + t]);
        }
    }
    child->keys.resize(t - 1);
    parent->children.insert(parent->children.begin() + i + 1, newChild);
    parent->keys.insert(parent->keys.begin() + i, child->keys[t - 1]);
}

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
    void ImportarCSV(const string& archivoCSV="Empleados.csv");

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

inline void AdministradorEmpleados::ImportarCSV(const std::string &archivoCSV)
{
    std::ifstream archivo(archivoCSV);

    if (!archivo.is_open())
    {
        std::cerr << "No se pudo abrir el archivo CSV." << std::endl;
        return;
    }

    std::string linea;
    // Leer la primera línea (encabezados) y descartarla
    std::getline(archivo, linea);

    while (std::getline(archivo, linea))
    {
        std::istringstream ss(linea);
        std::string elemento;

        Empleado empleado;

        // ID
        std::getline(ss, elemento, ',');
        empleado.ID = elemento;

        // Nombre
        std::getline(ss, elemento, ',');
        empleado.Nombre = elemento;

        // Departamento
        std::getline(ss, elemento, ',');
        empleado.Departamento = elemento;

        // Puesto
        std::getline(ss, elemento, ',');
        empleado.Puesto = elemento;

        // Contrasena
        std::getline(ss, elemento, ',');
        empleado.Contrasena = elemento;

        // Salario
        std::getline(ss, elemento, ',');
        empleado.Salario = std::stod(elemento);

        // Activo
        std::getline(ss, elemento, ',');
        empleado.Activo = (elemento == "1");

        // Añadir el empleado al vector
        Empleados.push_back(empleado);
    }

    archivo.close();
}


#endif // ADMINISTRADOREMPLEADOS_H
