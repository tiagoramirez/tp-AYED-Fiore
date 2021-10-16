#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

struct Empleado {
	char codEmp[9 + 1];
	char nombYApe[50 + 1];
	int cantProdVend;
};

struct Venta {
	char codEmp[9 + 1];
	int codProd;
	int fecha;
	float precioVenta;
};

struct NodoPila{
    Venta info;
    NodoPila* siguiente;
};

struct EmpleadosFinal{
	Empleado empleado;
	int totalRecaudado=0;
	NodoPila* venta;
};

void crearEmpleados() {

	FILE* empleados = fopen("Empleados.dat","wb+");

	Empleado vecE[4] = {
		{ "EE", "Juan Gomez", 50 },
		{ "AA", "Pablo Perez", 100 },
		{ "ZZ", "Ariel Gimenez", 10 },
		{ "NN", "Roberto Diaz", 200 }
	};

	fwrite(vecE, sizeof(Empleado), 4, empleados);
	fclose(empleados);
}

void mostrarEmpleados() {
	FILE* empleados = fopen("Empleados.dat","rb+");

	cout << "Codigo Empleado, Nombre y Apellido, Cantidad de productos Vendidos" << endl;
	Empleado e;
	fread(&e, sizeof(Empleado), 1, empleados);

	while (!feof(empleados)) {
		cout << e.codEmp << ", " << e.nombYApe << ", " << e.cantProdVend << endl;
		fread(&e, sizeof(Empleado), 1, empleados);
	}

	cout << endl;
	fclose(empleados);
}

void crearVentas() {

	FILE* ventas = fopen("Ventas.dat","wb+");

	Venta vecV[11] = {
		{ "EE", 1, 20141001, 40.0 },
		{ "ZZ", 5, 20141001, 30.0 },
		{ "EE", 2, 20141002, 15.0 },
		{ "AA", 5, 20141002, 30.0 },
		{ "AA", 5, 20141002, 30.0 },
		{ "EE", 2, 20141002, 15.0 },
		{ "ZZ", 3, 20141004, 70.0 },
		{ "ZZ", 3, 20141010, 70.0 },
		{ "EE", 2, 20141011, 15.0 },
		{ "ZZ", 4, 20141015, 60.0 },
		{ "ZZ", 5, 20141015, 30.0 }
	};

	fwrite(vecV, sizeof(Venta), 11, ventas);
	fclose(ventas);
}

void mostrarVentas() {
	FILE* ventas = fopen("Ventas.dat","rb+");

	cout << "Codigo Empleado, Codigo Producto, Fecha, Precio de Venta" << endl;
	Venta v;
	fread(&v, sizeof(Venta), 1, ventas);

	while (!feof(ventas)) {
		cout << v.codEmp << ", " << v.codProd << ", " << v.fecha << ", " << v.precioVenta << endl;
		fread(&v, sizeof(Venta), 1, ventas);
	}

	cout << endl;
	fclose(ventas);
}

void inicializarVector (int& len){
    len=0;
}

template <typename T>
void ordenar(T arr[], int len, int (*cmpTT)(T,T)){
    T aux;
    for (int i=0;i<len-1;i++){
        for(int x=0;x<len-1;x++){
            if(cmpTT(arr[x],arr[x+1])==1){//veo si el primero es mas grande que el segundo
                aux=arr[x];
                arr[x]=arr[x+1];
                arr[x+1]=aux;
            }
        }
    }
}

template <typename T>
int insertarOrdenado(T arr[], int& len, T v, int (*cmpTT)(T,T)){
    int i=len;
    while(i>0 and cmpTT(v,arr[i-1])==-1){
        arr[i]=arr[i-1];
        i--;
    }
    arr[i]=v;
    len++;
    return i;
}

template <typename T>
T read(FILE* f){
    T x;
    fread(&x,sizeof(T),1,f);
    return x;
}

void inicializarPila(NodoPila*& raiz){
    raiz=NULL;
}

void push(NodoPila*& raiz, Venta v){
    NodoPila* puntero=new NodoPila();
    puntero->info=v;
    puntero->siguiente=raiz;
    raiz=puntero;
}

Venta pop(NodoPila*& raiz){
    NodoPila* puntero=raiz;
    Venta info=puntero->info;
    raiz=puntero->siguiente;
    delete puntero;
    return info;
}

bool estaVacia(NodoPila* raiz){
    return raiz==NULL;
}

int cmpEmpEmp(EmpleadosFinal e1, EmpleadosFinal e2){
	return strcmp(e1.empleado.codEmp,e2.empleado.codEmp);
}

int cmpEmpEmp2(EmpleadosFinal e1, EmpleadosFinal e2){
	if(e1.totalRecaudado==e2.totalRecaudado){
		return 0;
	}
	else{
		if(e1.totalRecaudado>e2.totalRecaudado){
			return -1;
		}
		else{
			return 1;
		}
	}
}

void llenarRegistroFinal(EmpleadosFinal& regFinal, Empleado regEmpleado){
	regFinal.empleado=regEmpleado;
	FILE* archivoVentas=fopen("Ventas.dat","rb+");
	Venta regVenta=read<Venta>(archivoVentas);
	NodoPila* raizPila;
	inicializarPila(raizPila);
	while(!feof(archivoVentas)){
		if(strcmp(regVenta.codEmp,regEmpleado.codEmp)==0){
			push(raizPila,regVenta);
			regFinal.empleado.cantProdVend++;
			regFinal.totalRecaudado+=regVenta.precioVenta;
		}
		regVenta=read<Venta>(archivoVentas);
	}

	fclose(archivoVentas);

	regFinal.venta=raizPila;
}

void mostrarResultado(EmpleadosFinal empleados[],int lenEmpleados){
	for(int i=0;i<lenEmpleados;i++){
		NodoPila* raizPila=empleados[i].venta;
		Venta venta;
		cout<<"Codigo empleado: "<<empleados[i].empleado.codEmp<<endl;
		cout<<"Nombre y apellido: "<<empleados[i].empleado.nombYApe<<endl;
		cout<<"Total de productos vendidos: "<<empleados[i].empleado.cantProdVend<<endl;
		cout<<"Total recaudado: "<<empleados[i].totalRecaudado<<endl;
		cout<<"Productos Vendidos: "<<endl;
		cout<<"Codigo Producto\tFecha"<<endl;
		while(!estaVacia(raizPila)){
			Venta venta=pop(raizPila);
			cout<<venta.codProd<<"\t"<<venta.fecha<<endl;
		}
	}
}

void resolucionTp() {
	//TODO completar aquí con la resolución del TP
	// recordar usar la libreria string.h para el manejo de comparación y copia de valores de cadenas
	// funciones útiles para usar: strcmp y stcpy
	EmpleadosFinal empleados[50];
	int lenEmpleados;
	inicializarVector(lenEmpleados);

	FILE* archivoEmpleados=fopen("Empleados.dat","rb+");
	Empleado regEmpleado=read<Empleado>(archivoEmpleados);

	while(!feof(archivoEmpleados)){
		EmpleadosFinal regFinal;
	
		llenarRegistroFinal(regFinal,regEmpleado);

		insertarOrdenado<EmpleadosFinal>(empleados,lenEmpleados,regFinal,cmpEmpEmp);

		regEmpleado=read<Empleado>(archivoEmpleados);
	}

	fclose(archivoEmpleados);

	ordenar(empleados,lenEmpleados,cmpEmpEmp2);

	mostrarResultado(empleados,lenEmpleados);
}

int main() {
	crearEmpleados();
	mostrarEmpleados();
	crearVentas();
	mostrarVentas();

	resolucionTp();

	return 0;
}
