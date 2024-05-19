#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <conio.h>
using namespace std;

struct Estudiante {
    string nombre, carrera;
    int id;
    int notas[3] = {0,0,0};
};

class GestorEstudiantes {
private:
    Estudiante** estudiantes;
    int numEstudiantes = 0;

    void guardarEnArchivo() {
        ofstream archivo("datos.txt", ios::out);
        if (!archivo) {
            cout << "Error al abrir el archivo para escribir.\n";
            return;
        }
        for (int i = 0; i < numEstudiantes; i++) {
            archivo << estudiantes[i]->nombre << '\n'
                    << estudiantes[i]->id << '\n'
                    << estudiantes[i]->carrera << '\n';
            for (int j = 0; j < 3; j++) {
                archivo << estudiantes[i]->notas[j] << ' ';
            }
            archivo << '\n';
        }
        archivo.close();
    }

    void cargarDesdeArchivo() {
        ifstream archivo("datos.txt", ios::in);
        if (!archivo) {
            // Si no existe el archivo, no hay nada que cargar.
            return;
        }

        string linea;
        while (getline(archivo, linea)) {
            Estudiante* nuevoEstudiante = new Estudiante;
            nuevoEstudiante->nombre = linea;
            getline(archivo, linea);
            try {
                nuevoEstudiante->id = stoi(linea);
            } catch (const invalid_argument& e) {
                cout << "Error: ID invalido en el archivo.\n";
                delete nuevoEstudiante;
                continue;
            }
            getline(archivo, linea);
            nuevoEstudiante->carrera = linea;

            for (int j = 0; j < 3; j++) {
                archivo >> nuevoEstudiante->notas[j];
            }
            archivo.ignore(); // Ignorar el salto de línea al final de las notas.

            Estudiante** temp = new Estudiante*[numEstudiantes + 1];
            for (int i = 0; i < numEstudiantes; i++) {
                temp[i] = estudiantes[i];
            }
            temp[numEstudiantes] = nuevoEstudiante;
            delete[] estudiantes;
            estudiantes = temp;
            numEstudiantes++;
        }
        archivo.close();
    }

public:
    GestorEstudiantes() {
        estudiantes = new Estudiante*[1];
        cargarDesdeArchivo();
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

    bool idUnico;
    do {
        idUnico = true;
        cout << "ID: ";
        cin >> nuevoEstudiante->id;

        // Verificar si el ID ya existe
        for (int i = 0; i < numEstudiantes; i++) {
            if (estudiantes[i]->id == nuevoEstudiante->id) {
                cout << "El ID ya existe. Por favor, introduzca un ID diferente.\n";
                idUnico = false;
                break;
            }
        }
    } while (!idUnico);

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

    guardarEnArchivo();
}

    void introducirNotas() {
    int id;
    system("cls");
    cout << "ID del estudiante: ";
    cin >> id;
    for (int i = 0; i < numEstudiantes; i++) {
        if (estudiantes[i]->id == id) {
            int opcion;
            bool notasCompletadas[3] = {false, false, false}; // Para verificar si las notas se han introducido
            do {
                system("cls");
                cout << "Introducir notas para el estudiante con ID " << id << ":\n";
                cout << "1. Introducir Nota 1\n";
                cout << "2. Introducir Nota 2\n";
                cout << "3. Introducir Nota 3\n";
                cout << "4. Regresar al menu anterior\n";
                cout << "Seleccione una opcion: ";
                cin >> opcion;

                if (opcion >= 1 and opcion <= 3) {
                    int notaIndex = opcion - 1;
                    if (notaIndex > 0 and !notasCompletadas[notaIndex - 1]) {
                        cout << "Primero debe introducir la Nota " << notaIndex << ".\n";
                        system("pause");
                        continue;
                    }

                    int nota;
                    do {
                        cout << "Introduzca la Nota " << opcion << " (0-5): ";
                        cin >> nota;
                        if (nota < 0 or nota > 5) {
                            cout << "Nota invalida. La nota debe estar entre 0 y 5.\n";
                        }
                    } while (nota < 0 or nota > 5);

                    estudiantes[i]->notas[notaIndex] = nota;
                    notasCompletadas[notaIndex] = true;
                    guardarEnArchivo();
                } else if (opcion == 4) {
                    cout << "Regresando al menu anterior.\n";
                } else {
                    cout << "Opcion no valida.\n";
                }
                system("pause");
            } while (opcion != 4);
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
                        guardarEnArchivo();
                        break;
                    case 2:
                        cout << "Nueva carrera: ";
                        cin.ignore();
                        getline(cin, estudiantes[i]->carrera);
                        guardarEnArchivo();
                        break;
                    case 3:
                        introducirNotas();
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

    void eliminarEstudiante() {
    if (numEstudiantes == 0) {
        cout << "No hay estudiantes para eliminar.\n";
        system("pause");
        return;
    }

    system("cls");
    cout << "Seleccione el estudiante a eliminar:\n";
    for (int i = 0; i < numEstudiantes; i++) {
        cout << i + 1 << ". " << estudiantes[i]->nombre << " (ID: " << estudiantes[i]->id << ")\n";
    }
    cout << "0. Cancelar\n";
    cout << "Seleccione una opcion: ";
    int opcion;
    cin >> opcion;

    if (opcion == 0) {
        cout << "Cancelando...\n";
        system("pause");
        return;
    }

    if (opcion < 1 || opcion > numEstudiantes) {
        cout << "Opcion no valida.\n";
        system("pause");
        return;
    }

    int index = opcion - 1;
    delete estudiantes[index];

    for (int i = index; i < numEstudiantes - 1; i++) {
        estudiantes[i] = estudiantes[i + 1];
    }

    numEstudiantes--;
    guardarEnArchivo();
    cout << "Estudiante eliminado correctamente.\n";
    system("pause");
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
        cout << "5. Eliminar estudiante\n";
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
        case 5:
            gestor.eliminarEstudiante();
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