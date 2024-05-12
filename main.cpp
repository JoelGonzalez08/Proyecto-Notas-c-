#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <fstream>
using namespace std;


struct Estudiante {
    string nombre, carrera;
    int id;
    int notas[3];
};


class GestorEstudiantes {
private:
    Estudiante** estudiantes;
    int numEstudiantes = 0;

public:
    GestorEstudiantes() {
        estudiantes = new Estudiante*[1]; 
    }

    ~GestorEstudiantes() {
        for (int i = 0; i < numEstudiantes; i++) {
            delete estudiantes[i]; 
        }
        delete[] estudiantes; 
    }

    void crearEstudiante() {
        Estudiante* nuevoEstudiante = new Estudiante; 
        system("cls");
        cout << "Nombre: ";
        cin.ignore(); 
        getline(cin, nuevoEstudiante->nombre);
        cout << "ID: ";
        cin >> nuevoEstudiante->id;
        cout << "Carrera: ";
        cin.ignore(); 
        getline(cin, nuevoEstudiante->carrera);
        
        Estudiante** temp = new Estudiante*[numEstudiantes + 1];
        for (int i = 0; i < numEstudiantes; i++) {
            temp[i] = estudiantes[i];
        }
        temp[numEstudiantes] = nuevoEstudiante;
        delete[] estudiantes;
        estudiantes = temp;
        numEstudiantes++;
    }

    void introducirNotas() {
        int id;
        system("cls");
        cout << "ID del estudiante: ";
        cin >> id;
        for (int i = 0; i < numEstudiantes; i++) {
            if (estudiantes[i]->id == id) {
                cout << "Introducir 3 notas:\n";
                for (int j = 0; j < 3; j++) {
                    cout << "Nota " << j + 1 << ": ";
                    cin >> estudiantes[i]->notas[j];
                }
                return;
            }
        }
        cout << "Estudiante no encontrado.\n";
    }

    void modificarEstudiante() {
        int id;
        cout << "ID del estudiante a modificar: ";
        cin >> id;
        for (int i = 0; i < numEstudiantes; i++) {
            if (estudiantes[i]->id == id) {
                int opcion;
                do {
                    system("cls");
                    cout << "\nMenu de Modificacion:\n";
                    cout << "1. Nombre\n";
                    cout << "2. Carrera\n";
                    cout << "3. Notas\n";
                    cout << "4. Regresar al menu anterior\n";
                    cout << "Seleccione una opcion: ";
                    cin >> opcion;

                    switch (opcion) {
                    case 1:
                        cout << "Nuevo nombre: ";
                        cin.ignore(); 
                        getline(cin, estudiantes[i]->nombre);
                        break;
                    case 2:
                        cout << "Nueva carrera: ";
                        cin.ignore();
                        getline(cin, estudiantes[i]->carrera);
                        break;
                    case 3:
                        break;
                    case 4:
                        cout << "Regresando al menu anterior.\n";
                        break;
                    default:
                        cout << "Opcion no valida.\n";
                        break;
                    }
                } while (opcion != 4);
                return;
            }
        }
        cout << "Estudiante no encontrado.\n";
    }

    void mostrarEstudiantes() {
        for (int i = 0; i < numEstudiantes; i++) {
            system("cls");
            cout << "Nombre: " << estudiantes[i]->nombre << endl;
            cout << "ID: " << estudiantes[i]->id << endl;
            cout << "Carrera: " << estudiantes[i]->carrera << endl;
            cout << "Notas:";
            for (int j = 0; j < 3; j++) {
                cout << " " << estudiantes[i]->notas[j];
            }
            cout << endl;
            double suma = 0;
            for (int j = 0; j < 3; j++) {
                suma += estudiantes[i]->notas[j];
            }
            double promedio = suma / 3;
            cout << "Nota final: " << promedio << endl;
            getch();
        }
    }
};

int main() {
    GestorEstudiantes gestor;
    int opcion;
    do {
        system("cls");
        cout << "\nMenu:\n";
        cout << "1. Crear estudiante\n";
        cout << "2. Modificar estudiante\n";
        cout << "3. Introducir notas\n";
        cout << "4. Mostrar estudiantes\n";
        cout << "0. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            gestor.crearEstudiante();
            break;
        case 2:
            gestor.modificarEstudiante();
            break;
        case 3:
            gestor.introducirNotas();
            break;
        case 4:
            gestor.mostrarEstudiantes();
            break;
        case 0:
            cout << "Saliendo del programa.\n";
            break;
        default:
            cout << "Opcion no valida.\n";
            break;
        }
    } while (opcion != 0);

    return 0;
}