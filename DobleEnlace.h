// Cola doblemente enlazada en C++ con plantillas
// (C) Marzo 2002, Salvador Pozo
// C con Clase: http://c.conclase.net

#ifndef _LISTADOBLEABIERTA_
#define _LISTADOBLEABIERTA_

#include <iostream>
#include "CCadena.h"
using namespace std;

#define ASCENDENTE 1
#define DESCENDENTE 0

template<class TIPO> class lista;

template<class TIPO>
class nodo {
   public:
    nodo(TIPO v, nodo<TIPO> *sig = NULL, nodo<TIPO> *ant = NULL) :
       valor(v), siguiente(sig), anterior(ant) {}

   private:
    TIPO valor;
    nodo<TIPO> *siguiente;
    nodo<TIPO> *anterior;

   friend class lista<TIPO>;
};

template<class TIPO>
class lista {
   public:
    lista() : plista(NULL) {}
    ~lista();

    void Insertar(TIPO v);
    void Borrar(TIPO v);
    bool ListaVacia() { return plista == NULL; }
    void Mostrar(int);
    void Siguiente();
    void Anterior();
    void Primero();
    void Ultimo();
    void ParaCada(void (*func)(TIPO&));
    bool Actual() { return plista != NULL; }
    TIPO ValorActual() { return plista->valor; }

   private:
    nodo<TIPO> *plista;
};

template<class TIPO>
lista<TIPO>::~lista()
{
   nodo<TIPO> *aux;

   Primero();
   while(plista) {
      aux = plista;
      plista = plista->siguiente;
      delete aux;
   }
}

template<class TIPO>
void lista<TIPO>::Insertar(TIPO v)
{
   nodo<TIPO> *nuevo;

   Primero();
   // Si la lista est� vac�a
   if(ListaVacia() || plista->valor > v) {
      // Asignamos a lista un nuevo nodo de valor v y
      // cuyo siguiente elemento es la lista actual
      nuevo = new nodo<TIPO>(v, plista);
      if(!plista) plista = nuevo;
      else plista->anterior = nuevo;
   }
   else {
      // Buscar el nodo de valor menor a v
      // Avanzamos hasta el �ltimo elemento o hasta que el siguiente tenga
      // un valor mayor que v
      while(plista->siguiente && plista->siguiente->valor <= v) Siguiente();
      // Creamos un nuevo nodo despu�s del nodo actual
      nuevo = new nodo<TIPO>(v, plista->siguiente, plista);
      plista->siguiente = nuevo;
      if(nuevo->siguiente) nuevo->siguiente->anterior = nuevo;
   }
}

template<class TIPO>
void lista<TIPO>::Borrar(TIPO v)
{
   nodo<TIPO> *pnodo;

   pnodo = plista;
   while(pnodo && pnodo->valor < v) pnodo = pnodo->siguiente;
   while(pnodo && pnodo->valor > v) pnodo = pnodo->anterior;

   if(!pnodo || pnodo->valor != v) return;
   // Borrar el nodo

   if(pnodo->anterior) // no es el primer elemento
      pnodo->anterior->siguiente = pnodo->siguiente;
   if(pnodo->siguiente) // no el el �ltimo nodo
      pnodo->siguiente->anterior = pnodo->anterior;
}

template<class TIPO>
void lista<TIPO>::Mostrar(int orden)
{
   nodo<TIPO> *pnodo;
   if(orden == ASCENDENTE) {
      Primero();
      pnodo = plista;
      while(pnodo) {
         cout << pnodo->valor << "-> ";
         pnodo = pnodo->siguiente;
      }
   }
   else {
      Ultimo();
      pnodo = plista;
      while(pnodo) {
         cout << pnodo->valor << "-> ";
         pnodo = pnodo->anterior;
      }
   }
   cout << endl;
}

template<class TIPO>
void lista<TIPO>::Siguiente()
{
   if(plista) plista = plista->siguiente;
}

template<class TIPO>
void lista<TIPO>::Anterior()
{
   if(plista) plista = plista->anterior;
}

template<class TIPO>
void lista<TIPO>::Primero()
{
   while(plista && plista->anterior) plista = plista->anterior;
}

template<class TIPO>
void lista<TIPO>::Ultimo()
{
   while(plista && plista->siguiente) plista = plista->siguiente;
}

// Aplicar una funci�n a cada nodo de la lista:
template<class TIPO>
void lista<TIPO>::ParaCada(void (*func)(TIPO&))
{
  int orden = 1;
  nodo<TIPO> *pnodo;
  if(orden == ASCENDENTE) {
     Primero();
     pnodo = plista;
     while(pnodo) {
        func(pnodo->valor);
        //cout << pnodo->valor << "-> ";
        pnodo = pnodo->siguiente;
     }
  }
  else {
     Ultimo();
     pnodo = plista;
     while(pnodo) {
        cout << pnodo->valor << "-> ";
        pnodo = pnodo->anterior;
     }
  }
  //cout << endl;
}

#endif
