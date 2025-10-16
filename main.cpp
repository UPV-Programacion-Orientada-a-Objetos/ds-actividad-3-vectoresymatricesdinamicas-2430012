#include <iostream>

void limpiarPantalla();
void esperarEnter();
void validarEntradaNumerica(int &opcion);
void validarEntradaNumerica(float &opcion);

class PlanificadorProduccion {
private:
    float** matrizPlanificacion;
    float* vectorCostosUnitarios;
    float* vectorMateriaPrima;
    float* costosAdquisicionMP;
    const int (*requerimientosFijos)[4];
    int numProductos;
    int numSemanas;
    int numMateriasPrimas;
    static const int REQUERIMIENTOS_DATA[5][4];

public:
    PlanificadorProduccion(int productos, int semanas, int materiasPrimas) {
        numProductos = productos;
        numSemanas = semanas;
        numMateriasPrimas = materiasPrimas;
        requerimientosFijos = REQUERIMIENTOS_DATA;

        matrizPlanificacion = new float*[numProductos];
        for (int i = 0; i < numProductos; ++i) {
            matrizPlanificacion[i] = new float[numSemanas];
        }

        vectorCostosUnitarios = new float[numProductos];
        vectorMateriaPrima = new float[numMateriasPrimas];
        costosAdquisicionMP = new float[numMateriasPrimas];
        
        std::cout << "Inicializando sistema..." << std::endl;
        cargarDatosIniciales();
        std::cout << "Inicializacion exitosa. Matriz de Planificacion: " << numProductos << " Productos x " << numSemanas << " Semanas." << std::endl;
        esperarEnter();
    }

    ~PlanificadorProduccion() {
        std::cout << "\nLiberando memoria de Matriz y Vectores Dinamicos..." << std::endl;
        
        for (int i = 0; i < numProductos; ++i) {
            delete[] matrizPlanificacion[i];
        }
        delete[] matrizPlanificacion;

        delete[] vectorCostosUnitarios;
        delete[] vectorMateriaPrima;
        delete[] costosAdquisicionMP;
        
        std::cout << "Memoria liberada. Sistema cerrado." << std::endl;
    }

    void cargarDatosIniciales() {
        float plan[5][4] = {
            {1000, 1200, 1100, 1300},
            {2000, 2100, 2200, 2050},
            {800,  850,  900,  870},
            {500,  520,  510,  530},
            {1500, 1600, 1550, 1650}
        };
        for(int i = 0; i < numProductos; ++i) {
            for(int j = 0; j < numSemanas; ++j) {
                matrizPlanificacion[i][j] = plan[i][j];
            }
        }

        float costos[5] = {5.50, 6.20, 7.80, 4.30, 8.00};
        for(int i = 0; i < numProductos; ++i) vectorCostosUnitarios[i] = costos[i];

        float materia[4] = {10000, 15000, 8000, 20000};
        for(int i = 0; i < numMateriasPrimas; ++i) vectorMateriaPrima[i] = materia[i];

        float costosMP[4] = {2.00, 1.50, 3.00, 0.75};
        for(int i = 0; i < numMateriasPrimas; ++i) costosAdquisicionMP[i] = costosMP[i];
    }
    
    void ejecutar() {
        int opcion = 0;
        do {
            limpiarPantalla();
            std::cout << "--- Sistema de Planificacion y Costos (COGS) ---\n" << std::endl;
            std::cout << "--- Menu Principal ---\n";
            std::cout << "1. Ver Plan de Produccion\n";
            std::cout << "2. Agregar Nueva Semana (Redimensionar Matriz)\n";
            std::cout << "3. Modificar Produccion\n";
            std::cout << "4. Calcular COGS e Inventario Final\n";
            std::cout << "5. Reporte de Consumo de Materia Prima\n";
            std::cout << "6. Salir\n";
            std::cout << "\nOpcion seleccionada: ";
            
            validarEntradaNumerica(opcion);

            switch (opcion) {
                case 1: verPlanProduccion(); break;
                case 2: agregarNuevaSemana(); break;
                case 3: modificarProduccion(); break;
                case 4: calcularCOGSyInventarioFinal(); break;
                case 5: reporteConsumo(); break;
                case 6: break;
                default:
                    std::cout << "Opcion no valida. Intente de nuevo." << std::endl;
                    esperarEnter();
            }
        } while (opcion != 6);
    }

private:
    void verPlanProduccion() {
        limpiarPantalla();
        std::cout << "--- Plan de Produccion (Unidades) ---\n\n";
        
        std::cout << "Producto" << " | ";
        for (int j = 0; j < numSemanas; ++j) {
            std::cout << "Semana " << j << " | ";
        }
        std::cout << "\n----------------------------------------------------\n";

        for (int i = 0; i < numProductos; ++i) {
            std::cout << "Producto " << i << " | ";
            for (int j = 0; j < numSemanas; ++j) {
                std::cout << matrizPlanificacion[i][j] << " | ";
            }
            std::cout << std::endl;
        }
        esperarEnter();
    }
    
    void agregarNuevaSemana() {
        limpiarPantalla();
        std::cout << "Agregando Semana " << numSemanas << "...\n";

        int nuevasSemanas = numSemanas + 1;

        float** nuevaMatriz = new float*[numProductos];
        for (int i = 0; i < numProductos; ++i) {
            nuevaMatriz[i] = new float[nuevasSemanas];
        }

        for (int i = 0; i < numProductos; ++i) {
            for (int j = 0; j < numSemanas; ++j) {
                nuevaMatriz[i][j] = matrizPlanificacion[i][j];
            }
        }
        
        std::cout << "\nIngrese la produccion para la nueva semana (" << numSemanas << "):\n";
        for (int i = 0; i < numProductos; ++i) {
             std::cout << "  - Producto " << i << ": ";
             validarEntradaNumerica(nuevaMatriz[i][numSemanas]);
        }

        for (int i = 0; i < numProductos; ++i) {
            delete[] matrizPlanificacion[i];
        }
        delete[] matrizPlanificacion;

        matrizPlanificacion = nuevaMatriz;
        numSemanas = nuevasSemanas;

        std::cout << "\nMatriz redimensionada a " << numProductos << "x" << numSemanas << ". ¡Memoria gestionada con exito!" << std::endl;
        esperarEnter();
    }
    
    void modificarProduccion() {
        limpiarPantalla();
        std::cout << "--- Modificar Plan de Produccion ---\n";
        int producto, semana;
        float cantidad;

        while (true) {
            std::cout << "Ingrese Producto (0-" << numProductos - 1 << "): ";
            validarEntradaNumerica(producto);
            if (producto >= 0 && producto < numProductos) break;
            std::cout << "Error: Producto fuera de rango.\n";
        }

        while (true) {
            std::cout << "Ingrese Semana (0-" << numSemanas - 1 << "): ";
            validarEntradaNumerica(semana);
            if (semana >= 0 && semana < numSemanas) break;
            std::cout << "Error: Semana fuera de rango.\n";
        }

        std::cout << "Cantidad a producir: ";
        validarEntradaNumerica(cantidad);

        matrizPlanificacion[producto][semana] = cantidad;
        std::cout << "\nProduccion actualizada con exito." << std::endl;
        esperarEnter();
    }

    void calcularCOGSyInventarioFinal() {
        limpiarPantalla();
        std::cout << "--- Calculo de COGS e Inventario Final ---\n";
        int semana;

        while (true) {
            std::cout << "Ingrese Semana para el calculo (0-" << numSemanas - 1 << "): ";
            validarEntradaNumerica(semana);
            if (semana >= 0 && semana < numSemanas) break;
            std::cout << "Error: Semana fuera de rango.\n";
        }
        
        float produccionTotalSemana = 0;
        float cogsSemana = 0;
        float* consumoMP = new float[numMateriasPrimas]();
        float valorInventarioFinal = 0;

        for (int i = 0; i < numProductos; ++i) {
            float produccionProducto = matrizPlanificacion[i][semana];
            produccionTotalSemana += produccionProducto;
            cogsSemana += produccionProducto * vectorCostosUnitarios[i];
        }

        for (int j = 0; j < numMateriasPrimas; ++j) {
            for (int i = 0; i < numProductos; ++i) {
                consumoMP[j] += matrizPlanificacion[i][semana] * requerimientosFijos[i][j];
            }
        }
        
        std::cout << "\n--- Reporte de Costos (Semana " << semana << ") ---\n";
        std::cout << "Produccion Total (Semana " << semana << "): " << produccionTotalSemana << " unidades.\n";
        std::cout << "Costo Total de Produccion (COGS): $" << cogsSemana << "\n";
        
        std::cout << "\n--- Detalle de Inventario de Materia Prima ---\n";
        std::cout << "MP" << " | " << "Inv. Inicial" << " | " << "Consumo" << " | " << "Inv. Final" << " | " << "Valor Final" << " | \n";
        std::cout << "----------------------------------------------------------------------\n";
                  
        for (int j = 0; j < numMateriasPrimas; ++j) {
            float inventarioFinalMP = vectorMateriaPrima[j] - consumoMP[j];
            float valorMPRestante = inventarioFinalMP * costosAdquisicionMP[j];
            if (inventarioFinalMP < 0) {
                 std::cout << "¡Alerta! Stock negativo para MP " << j << std::endl;
                 valorMPRestante = 0;
            }
            valorInventarioFinal += valorMPRestante;
            
             std::cout << "MP-" << j << " | " << vectorMateriaPrima[j] << " | " << consumoMP[j] << " | " << inventarioFinalMP << " | " << valorMPRestante << " | \n";
        }
        
        std::cout << "\nValor Total del Inventario Final (M.P. restante): $" << valorInventarioFinal << "\n";
        
        delete[] consumoMP;
        esperarEnter();
    }

    void reporteConsumo() {
        limpiarPantalla();
        std::cout << "--- Reporte de Consumo de Materia Prima ---\n";
        int semana;

        while (true) {
            std::cout << "Ingrese Semana para el reporte (0-" << numSemanas - 1 << "): ";
            validarEntradaNumerica(semana);
            if (semana >= 0 && semana < numSemanas) break;
            std::cout << "Error: Semana fuera de rango.\n";
        }

        float* consumoMP = new float[numMateriasPrimas]();
        
        for (int j = 0; j < numMateriasPrimas; ++j) {
            for (int i = 0; i < numProductos; ++i) {
                consumoMP[j] += matrizPlanificacion[i][semana] * requerimientosFijos[i][j];
            }
        }
        
        std::cout << "\n--- Consumo Total de MP (Semana " << semana << ") ---\n";
        for (int j = 0; j < numMateriasPrimas; ++j) {
             std::cout << " - Materia Prima " << j << ": " << consumoMP[j] << " unidades." << std::endl;
        }

        delete[] consumoMP;
        esperarEnter();
    }
};

const int PlanificadorProduccion::REQUERIMIENTOS_DATA[5][4] = {
    {2, 1, 0, 3},
    {1, 3, 1, 1},
    {0, 2, 4, 1},
    {3, 0, 1, 2},
    {1, 1, 2, 2}
};

int main() {
    PlanificadorProduccion sistema(5, 4, 4);
    sistema.ejecutar();
    return 0;
}

void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void esperarEnter() {
    std::cout << "\nPresione Enter para continuar...";
    std::cin.ignore(10000, '\n');
    if (std::cin.rdbuf()->in_avail() == 0) {
        std::cin.get();
    }
}

void validarEntradaNumerica(int &opcion) {
    while (!(std::cin >> opcion)) {
        std::cout << "Entrada invalida. Por favor, ingrese un numero: ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
}

void validarEntradaNumerica(float &opcion) {
    while (!(std::cin >> opcion)) {
        std::cout << "Entrada invalida. Por favor, ingrese un numero: ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
}
