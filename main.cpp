#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <conio.h>
using namespace std;

struct Materia {
    string nombre;
    int notas[3] = {0, 0, 0};
};

struct Estudiante {
    string nombre, carrera;
    int id;
    Materia* materias;
    int numMaterias;
};

class GestorEstudiantes {
private:
    Estudiante* estudiantes;
    int numEstudiantes = 0;

    void guardarEnArchivo() {
        ofstream archivo("datos.txt", ios::out);
        if (!archivo) {
            cout << "Error al abrir el archivo para escribir.\n";
            return;
        }
        for (int i = 0; i < numEstudiantes; i++) {
            archivo << estudiantes[i].nombre << '\n'
                    << estudiantes[i].id << '\n'
                    << estudiantes[i].carrera << '\n'
                    << estudiantes[i].numMaterias << '\n';
            for (int j = 0; j < estudiantes[i].numMaterias; j++) {
                archivo << estudiantes[i].materias[j].nombre << ' ';
                for (int k = 0; k < 3; k++) {
                    archivo << estudiantes[i].materias[j].notas[k] << ' ';
                }
                archivo << "\n";
            }
        }
        archivo.close();
    }

    void cargarDesdeArchivo() {
        ifstream archivo("datos.txt", ios::in);
        if (!archivo) {
            return;
        }
        string linea;
        while (getline(archivo, linea)) {
            Estudiante nuevoEstudiante;
            nuevoEstudiante.nombre = linea;
            getline(archivo, linea);
            try {
                nuevoEstudiante.id = stoi(linea);
            } catch (const invalid_argument& e) {
                cout << "Error: ID invalido en el archivo.\n";
                continue;
            }
            getline(archivo, linea);
            nuevoEstudiante.carrera = linea;

            getline(archivo, linea);
            nuevoEstudiante.numMaterias = stoi(linea);
            nuevoEstudiante.materias = new Materia[nuevoEstudiante.numMaterias];
            for (int j = 0; j < nuevoEstudiante.numMaterias; j++) {
                archivo >> nuevoEstudiante.materias[j].nombre;
                for (int k = 0; k < 3; k++) {
                    archivo >> nuevoEstudiante.materias[j].notas[k];
                }
            }
            archivo.ignore();

            Estudiante* temp = new Estudiante[numEstudiantes + 1];
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

    void introducirNotasEnMateria(Materia& materia) {
        int opcion;
        bool notasCompletadas[3] = {false, false, false}; // Para verificar si las notas se han introducido
        do {
            system("cls");
            cout << "Introducir notas para la materia " << materia.nombre << ":\n";
            cout << "1. Introducir Nota 1\n";
            cout << "2. Introducir Nota 2\n";
            cout << "3. Introducir Nota 3\n";
            cout << "4. Regresar al menu anterior\n";
            cout << "Seleccione una opcion: ";
            cin >> opcion;

            if (opcion >= 1 && opcion <= 3) {
                int notaIndex = opcion - 1;
                if (notaIndex > 0 && !notasCompletadas[notaIndex - 1]) {
                    cout << "Primero debe introducir la Nota " << notaIndex << ".\n";
                    system("pause");
                    continue;
                }

                int nota;
                do {
                    cout << "Introduzca la Nota " << opcion << " (0-5): ";
                    cin >> nota;
                    if (nota < 0 || nota > 5) {
                        cout << "Nota invalida. La nota debe estar entre 0 y 5.\n";
                    }
                } while (nota < 0 || nota > 5);

                materia.notas[notaIndex] = nota;
                notasCompletadas[notaIndex] = true;
                guardarEnArchivo();
            } else if (opcion == 4) {
                cout << "Regresando al menu anterior.\n";
            } else {
                cout << "Opcion no valida.\n";
            }
            system("pause");
        } while (opcion != 4);
    }

  void menuMaterias(Estudiante* estudiante) {
    int opcion;
    do {
        system("cls");
        cout << "Menu de Materias para el estudiante " << estudiante->nombre << " (Codigo: T000" << estudiante->id << "):\n";
        for (int i = 0; i < estudiante->numMaterias; i++) {
            cout << i + 1 << ". " << estudiante->materias[i].nombre << '\n';
        }
        cout << estudiante->numMaterias + 1 << ". Agregar nueva materia\n";
        cout << "0. Regresar al menu anterior\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        if (opcion > 0 && opcion <= estudiante->numMaterias) {
            int materiaIndex = opcion - 1;
            introducirNotasEnMateria(estudiante->materias[materiaIndex]);
        } else if (opcion == estudiante->numMaterias + 1) {
            Materia* nuevasMaterias = new Materia[estudiante->numMaterias + 1];
            for (int i = 0; i < estudiante->numMaterias; i++) {
                nuevasMaterias[i] = estudiante->materias[i];
            }
            delete[] estudiante->materias;
            estudiante->materias = nuevasMaterias;

            // Validación del nombre de la materia
            do {
                cout << "Nombre de la nueva materia (sin números): ";
                getline(cin, estudiante->materias[estudiante->numMaterias].nombre);
            } while (!validarCadena(estudiante->materias[estudiante->numMaterias].nombre, false)); // No permitir espacios NI NÚMEROS

            estudiante->numMaterias++;
            guardarEnArchivo();
        } else if (opcion != 0) {
            cout << "Opcion no valida.\n";
        }
    } while (opcion != 0);
}


 bool validarCadena(const string& cadena, bool permitirEspacios = true) {
    for (char c : cadena) {
        if (!isalpha(c) && (!permitirEspacios || !isspace(c))) { 
            return false; // Caracter inválido (no es letra ni espacio permitido)
        }
    }
    return !cadena.empty(); // No puede estar vacía
 }

bool validarEntero(int& numero) {
    string entrada;
    getline(cin, entrada);

    for (char c : entrada) {
        if (!isdigit(c)) {
            cout << "Entrada inválida. Ingrese solo números.\n";
            return false;
        }
    }

    // Convertir la cadena a entero
    try {
        numero = stoi(entrada);
        return true;
    } catch (const std::invalid_argument&) {
        cout << "Entrada demasiado grande o no numérica.\n";
        return false;
    }
}

public:
    GestorEstudiantes() {
        estudiantes = new Estudiante[1];
        cargarDesdeArchivo();
    }

    ~GestorEstudiantes() {
        for (int i = 0; i < numEstudiantes; i++) {
            delete[] estudiantes[i].materias;
        }
        delete[] estudiantes;
    }

    void crearEstudiante() {
        Estudiante nuevoEstudiante;
        system("cls");
        cout << "Nombre: ";
        cin.ignore();
        do {
        cout << "Nombre (solo letras): ";
        getline(cin, nuevoEstudiante.nombre);
    } while (!validarCadena(nuevoEstudiante.nombre, true)); // Permitir espacios
        
        bool idUnico;
        do {
            idUnico = true;
            cout << "Codigo (solo numeros): T000";
            if (!validarEntero(nuevoEstudiante.id)) {
                idUnico = false;
                continue;
            }

            for (int i = 0; i < numEstudiantes; i++) {
                if (estudiantes[i].id == nuevoEstudiante.id) {
                    cout << "El codigo ya existe. Por favor, introduzca uno diferente.\n";
                    idUnico = false;
                    break;
                }
            }
        } while (!idUnico);


        do {
        cout << "Carrera (solo letras): ";
        getline(cin, nuevoEstudiante.carrera);
    } while (!validarCadena(nuevoEstudiante.carrera, true)); // Permitir espacios


        nuevoEstudiante.numMaterias = 0;
        nuevoEstudiante.materias = nullptr;

        Estudiante* temp = new Estudiante[numEstudiantes + 1];
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
        cout << "Codigo del estudiante: T000";
        cin >> id;
        for (int i = 0; i < numEstudiantes; i++) {
            if (estudiantes[i].id == id) {
                menuMaterias(&estudiantes[i]);
                return;
            }
        }
        cout << "Estudiante no encontrado.\n";
    }

    void modificarEstudiante() {
        int id;
        cout << "Codigo del estudiante a modificar: T000";
        cin >> id;
        for (int i = 0; i < numEstudiantes; i++) {
            if (estudiantes[i].id == id) {
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
                        getline(cin, estudiantes[i].nombre);
                        guardarEnArchivo();
                        break;
                    case 2:
                        cout << "Nueva carrera: ";
                        cin.ignore();
                        getline(cin, estudiantes[i].carrera);
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
                    system("pause");
                } while (opcion != 4);
                return;
            }
        }
        cout << "Estudiante no encontrado.\n";
    }

void buscarEstudiante() {
    if (numEstudiantes == 0) {
        cout << "No hay estudiantes registrados.\n";
        return;
    }

    int id;
        cout << "Codigo del estudiante a modificar: T000";
        cin >> id;
        for (int i = 0; i < numEstudiantes; i++) {
            if (estudiantes[i].id == id) {
                cout << "Codigo: T000" << estudiantes[i].id << '\n'
                 << "Nombre: " << estudiantes[i].nombre << '\n'
                 << "Carrera: " << estudiantes[i].carrera << '\n';
            cout << "Materias:\n";
            for (int j = 0; j < estudiantes[i].numMaterias; j++) {
                cout << "  " << estudiantes[i].materias[j].nombre << ": ";
                for (int k = 0; k < 3; k++) {
                    cout << estudiantes[i].materias[j].notas[k] << ' ';
                }
                cout << '\n';
            }
            cout << "-----------------------------\n";
        }
}
}

void eliminarEstudiante() {
    if (numEstudiantes == 0) {
        cout << "No hay estudiantes registrados.\n";
        return;
    }

    system("cls");
    cout << "Eliminar Estudiante:\n";
    for (int i = 0; i < numEstudiantes; i++) {
        cout << i + 1 << ". " << estudiantes[i].nombre << " (Codigo: T000" << estudiantes[i].id << ")\n";
    }
    cout << "0. Regresar al menu anterior\n";
    cout << "Seleccione el estudiante a eliminar: ";

    int opcion;
    cin >> opcion;

    if (opcion >= 1 && opcion <= numEstudiantes) {
        int estudianteIndex = opcion - 1;

        char confirmacion;
        cout << "¿Está seguro de que desea eliminar a " << estudiantes[estudianteIndex].nombre << "? (s/n): ";
        cin >> confirmacion;

        if (confirmacion == 's' || confirmacion == 'S') {  // Comparación manual
            // Mover los elementos restantes hacia la izquierda para llenar el espacio vacío
            for (int j = estudianteIndex; j < numEstudiantes - 1; j++) {
                estudiantes[j] = estudiantes[j + 1];
            }

            numEstudiantes--;
            cout << "Estudiante eliminado.\n";
            guardarEnArchivo();
        } else {
            cout << "Eliminación cancelada.\n";
        }
    } else if (opcion != 0) {
        cout << "Opcion no valida.\n";
    }
}

void mostrarEstudiantes() {
     system("cls");
         cout << "Lista de estudiantes:\n";
    for (int i = 0; i < numEstudiantes; i++) {
            cout << "Codigo: T000" << estudiantes[i].id << '\n'
                << "Nombre: " << estudiantes[i].nombre << '\n'
                << "Carrera: " << estudiantes[i].carrera << '\n';
            cout << "Materias:\n";
                for (int j = 0; j < estudiantes[i].numMaterias; j++) {
                 cout << " " << estudiantes[i].materias[j].nombre << ": ";
                 for (int k = 0; k < 3; k++) {
                cout << estudiantes[i].materias[j].notas[k] << ' ';
                 }
                 cout << '\n';
         }
            cout << "-----------------------------\n";
    }
    }
};

int main() {
    GestorEstudiantes gestor;
    int opcion;
    do {
        system("cls");
        cout << "Menu Principal:\n";
        cout << "1. Crear estudiante\n";
        cout << "2. Introducir notas\n";
        cout << "3. Modificar estudiante\n";
        cout << "4. Eliminar estudiante\n";
        cout << "5. Mostrar estudiantes\n";
        cout << "6. Buscar estudiante\n";
        cout << "7. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            gestor.crearEstudiante();
            break;
        case 2:
            gestor.introducirNotas();
            break;
        case 3:
            gestor.modificarEstudiante();
            break;
        case 4:
            gestor.eliminarEstudiante();
            break;
        case 5:
            gestor.mostrarEstudiantes();
            break;
        case 6:
            gestor.buscarEstudiante();
        case 7:
            cout << "Saliendo del programa.\n";
            break;
        default:
            cout << "Opcion no valida.\n";
            break;
        }
        system("pause");
    } while (opcion != 7);
    return 0;
}