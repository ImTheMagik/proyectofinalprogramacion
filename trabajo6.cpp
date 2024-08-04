#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Funciones para calcular estadísticas
double calcularMedia(const vector<double> &notas)
{
    double suma = 0.0;
    for (double nota : notas)
    {
        suma += nota;
    }
    return suma / notas.size();
}

double calcularDesviacionEstandar(const vector<double> &notas, double media)
{
    double suma = 0.0;
    for (double nota : notas)
    {
        suma += pow(nota - media, 2);
    }
    return sqrt(suma / (notas.size() - 1));
}

double calcularCoeficienteVariacion(double desviacionEstandar, double media)
{
    return desviacionEstandar / media;
}

double calcularCurtosis(const vector<double> &notas, double media, double desviacionEstandar)
{
    double suma = 0.0;
    for (double nota : notas)
    {
        suma += pow((nota - media) / desviacionEstandar, 4);
    }
    return (suma / notas.size()) - 3;
}

string interpretarCurtosis(double curtosis)
{
    if (curtosis > 0)
    {
        return "La distribucion tiene colas mas largas y picos mas altos que una distribucion normal.";
    }
    else if (curtosis < 0)
    {
        return "La distribucion tiene colas mas cortas y picos mas planos que una distribucion normal.";
    }
    else
    {
        return "La distribucion es aproximadamente normal.";
    }
}

double calcularPorcentajeAprobacion(const vector<double> &notas, double notaAprobacion)
{
    int contador = 0;
    for (double nota : notas)
    {
        if (nota >= notaAprobacion)
        {
            contador++;
        }
    }
    return (static_cast<double>(contador) / notas.size()) * 100.0;
}

double calcularMediana(vector<double> &datos)
{
    sort(datos.begin(), datos.end());
    size_t n = datos.size();
    if (n % 2 == 0)
    {
        return (datos[n / 2 - 1] + datos[n / 2]) / 2.0;
    }
    else
    {
        return datos[n / 2];
    }
}

vector<double> calcularModa(const vector<double> &datos)
{
    unordered_map<double, int> frecuencia;
    vector<double> modas;
    int maxFrecuencia = 0;

    for (double dato : datos)
    {
        frecuencia[dato]++;
        if (frecuencia[dato] > maxFrecuencia)
        {
            maxFrecuencia = frecuencia[dato];
        }
    }

    for (const auto &[valor, freq] : frecuencia)
    {
        if (freq == maxFrecuencia)
        {
            modas.push_back(valor);
        }
    }

    return modas;
}

double calcularRango(const vector<double> &datos)
{
    auto minMax = minmax_element(datos.begin(), datos.end());
    return *minMax.second - *minMax.first;
}

string interpretarModa(const vector<double> &modas)
{
    if (modas.size() == 1)
    {
        return "La nota que mas se repite es " + to_string(static_cast<int>(modas[0]));
    }
    else if (modas.size() > 1)
    {
        string resultado = "Las notas que mas se repiten son ";
        for (size_t i = 0; i < modas.size(); ++i)
        {
            resultado += to_string(static_cast<int>(modas[i]));
            if (i < modas.size() - 1)
            {
                resultado += ", ";
            }
        }
        return resultado;
    }
    else
    {
        return "No hay moda en los datos.";
    }
}

string interpretarMediana(double mediana)
{
    return "La mediana de los datos es " + to_string(static_cast<int>(mediana));
}

string interpretarRango(double rango)
{
    return "El rango de los datos es " + to_string(static_cast<int>(rango));
}

int main()
{
    string archivoCSV;
    string linea;

    cout << "Ingrese la ruta del archivo CSV: ";
    getline(cin, archivoCSV);

    ifstream archivo(archivoCSV);

    if (!archivo.is_open())
    {
        cerr << "No se pudo abrir el archivo CSV: " << archivoCSV << endl;
        return 1;
    }

    cout << "Archivo CSV abierto correctamente." << endl;

    getline(archivo, linea);

    vector<double> todasNotas;
    vector<double> todasNotasFinales;

    while (getline(archivo, linea))
    {
        stringstream ss(linea);
        string valor, nombre;
        vector<double> practicas(3);
        double examenParcial, examenFinal;

        for (int i = 0; i < 3; ++i)
        {
            if (getline(ss, valor, ','))
            {
                try
                {
                    practicas[i] = stod(valor);
                }
                catch (const invalid_argument &)
                {
                    cerr << "Error al convertir valor a numero: " << valor << endl;
                    return 1;
                }
            }
            else
            {
                cerr << "No se encontraron suficientes columnas en la linea: " << linea << endl;
                continue;
            }
        }

        if (getline(ss, valor, ','))
        {
            try
            {
                examenParcial = stod(valor);
            }
            catch (const invalid_argument &)
            {
                cerr << "Error al convertir valor a numero: " << valor << endl;
                return 1;
            }
        }
        else
        {
            cerr << "No se encontro nota del examen parcial en la linea: " << linea << endl;
            continue;
        }

        if (getline(ss, valor, ','))
        {
            try
            {
                examenFinal = stod(valor);
            }
            catch (const invalid_argument &)
            {
                cerr << "Error al convertir valor a numero: " << valor << endl;
                return 1;
            }
        }
        else
        {
            cerr << "No se encontro nota del examen final en la linea: " << linea << endl;
            continue;
        }

        if (getline(ss, valor, ','))
        {
            nombre = valor;
        }
        else
        {
            cerr << "No se encontro nombre del alumno en la linea: " << linea << endl;
            continue;
        }

        double mediaPracticas = calcularMedia(practicas);
        double notaFinal = (0.4 * mediaPracticas) + (0.3 * examenParcial) + (0.3 * examenFinal);

        // Redondear la nota final al entero más cercano
        notaFinal = round(notaFinal);

        // Redondear la media de las prácticas al entero más cercano
        double mediaPracticasRedondeada = round(mediaPracticas);

        todasNotas.insert(todasNotas.end(), practicas.begin(), practicas.end());
        todasNotasFinales.push_back(notaFinal);

        cout << "Alumno: " << nombre << endl;
        cout << "Notas de practicas: " << practicas[0] << ", " << practicas[1] << ", " << practicas[2] << endl;
        cout << "Media de las practicas: " << mediaPracticasRedondeada << endl;
        cout << "Nota del examen parcial: " << examenParcial << endl;
        cout << "Nota del examen final: " << examenFinal << endl;
        cout << "La nota final del alumno es: " << notaFinal << endl;
        if (notaFinal >= 10.5)
        {
            cout << "El alumno esta aprobado." << endl;
        }
        else
        {
            cout << "El alumno esta desaprobado." << endl;
        }

        cout << "-------------------------------------" << endl;
    }

    archivo.close();

    double notaAprobacion = 10.5;
    double porcentajeAprobacion = calcularPorcentajeAprobacion(todasNotas, notaAprobacion);
    cout << "Porcentaje de alumnos que aprobaron: " << porcentajeAprobacion << "%" << endl;

    // Calculo y presentacion de estadisticas globales
    double mediaGlobal = calcularMedia(todasNotas);
    double desviacionEstandarGlobal = calcularDesviacionEstandar(todasNotas, mediaGlobal);
    double rangoGlobal = calcularRango(todasNotas);
    vector<double> modaGlobal = calcularModa(todasNotasFinales);
    double medianaGlobal = calcularMediana(todasNotas);

    cout << "Estadisticas globales:" << endl;
    cout << "Media global: " << round(mediaGlobal) << endl;
    cout << "Desviacion estandar global: " << desviacionEstandarGlobal << endl;
    cout << interpretarMediana(round(medianaGlobal)) << endl;
    cout << interpretarRango(round(rangoGlobal)) << endl;
    cout << interpretarModa(modaGlobal) << endl;

    system("pause");
    return 0;
}
