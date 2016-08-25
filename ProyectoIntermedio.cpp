#include <iostream>
#include <cmath>
#include "Random64.h"
#include "linear.h"
//N es la cantidad de unidades basicas que existen
const int N=100000;
//Eta es la probabilidad de que se fragmente alguno de los grupos
const double Eta=0.01; 
using namespace std;

class M19{
	private:
  //Se guarda la cantidad de unidades que existen
  int Cantidad;
  int CantHist;
  //En cada celda se guarda la información del poder de ataque de la unidad, se organizan de izquierda a derecha (mayor a menor), si no hay unidades en la celda se pone un cero.
  int n[N+3];
  double hist[N][2];
  double loghist[N][2];
  
	public:
  void Inicie(void);
  void Evolucione(Crandom & Ran);
  void Reorganice(void);
  void Imprimase(void);
  void Histograma(void);
  void FitLinear(void);
 };

void M19::Inicie(void){
  //Se inicia con N unidades con poder de ataque 1
  for(int i=0;i<N;i++) {
    n[i]=1;
    hist[i][0]=hist[i][1]=0;
    loghist[i][0]=hist[i][1]=0;
  }
  //n[N] indica si se separó un unidad o se unio con otra. n[N+1] si se separó indica la posicion de la inicial que se separó, si se unieron indica uno de los lugares de las que se unieron. n[N+2] si se unieron indica el lugar de la segunda unidad que participó
  n[N]=n[N+1]=n[N+2]=3;
  //Cantidad inicial de unidades
  Cantidad=N;
}

void M19::Evolucione(Crandom & Ran){
  //Se implementa la regla de evolución dada en la página 18 del artículo "Universal patterns underlying ongoing wars and terrorism"
  int Suma=0;
  //Escoger es un numero aleatorio entre 0 y N obtenido de una distribución uniforme.
  double Escoger=Ran.r()*N,Prob1;
  for(int i=0;i<Cantidad;i++){
    //Se escoge la unidad que va a interactuar en este paso de tiempo
    if((Suma<=Escoger)&&(Escoger<=(Suma+n[i]))){
      //Se genera un numero aleatorio entre 0 y 1
      Prob1=Ran.r();
      //Se fragmenta la unidad
      if(Prob1<=Eta){
        for(int j=0;j<n[i]-1;j++){n[Cantidad+j]=1;}
        Cantidad+=n[i]-1;
	n[i]=1;
	n[N]=0; n[N+1]=i;}
      //Se une la unidad con otra unidad
      else{
	int Suma2=0;
	//Se genera un numero aleatorio entre o y N-n[i]
	double Escoger2=Ran.r()*(N-n[i]);
	//Escoger la segunda unidad con la cual se va a fusionar (Se escoge de las restantes unidades).
	for(int j=0;j<Cantidad;j++){
	  if(j!=i){
	    if((Suma2<=Escoger2)&&(Escoger2<=Suma2+n[j])){
              if(j<i){
		n[j]+=n[i];
		n[i]=0;
		n[N+1]=i;
		n[N+2]=j;}
	      else {
		n[i]+=n[j];
		n[j]=0;
		n[N+1]=j;
		n[N+2]=i;}
              n[N]=1; 
	      j=Cantidad+1;
	       }
	    Suma2+=n[j];
	  }
	}
      }
      //Salir del ciclo
      i=Cantidad+1;
      }
    Suma+=n[i];
      }
}


void M19::Reorganice(void){
  //En el paso realizado se juntaron dos grupos
  if(n[N]==1){
    int i,a;
    //n[N+1]>n[N+2]
    int indice1=n[N+1], indice2=n[N+2];
    //Mover los grupos a la izquierda del arreglo
    for(i=indice1;i<Cantidad-1;i++){
      n[i]=n[i+1];}
    //Reposicionar el grupo formado para que quede ordenado
    for(i=indice2;i>=1;i--){
      if(n[i-1]<n[i]){
	a=n[i];
	n[i]=n[i-1];
	n[i-1]=a;}
      else {i=0;}
	 }
    //Se actualiza la cantidad de grupos que existen
    Cantidad-=1;
     }
  //En el paso realizado se separó un grupo
  if(n[N]==0){
    int b=n[N+1],c;
    while(n[b+1]>n[b]){
      c=n[b+1];
      n[b+1]=n[b];
      n[b]=c;
      b+=1;}
  }
}
//Se genera una lista de la potencia de ataque de cada unidad
void M19::Imprimase(void){
  for(int i=0;i<Cantidad;i++){cout<<n[i]<<endl;}
  }
//Se genera el histograma
void M19::Histograma(void){
  int Tam=n[0];
  bool Cambio=false;
  int jj=0,kk=0,ii;
  //Creación del histograma
  for(ii=Tam;ii>0;ii--){
    while(n[jj]==ii){
    hist[kk][0]=ii;
    hist[kk][1]+=1;
    jj++;
    Cambio=true;
    }
    if((n[jj]!=ii)&&(Cambio==true)){
      kk++;
      Cambio=false;}
}
  CantHist=kk+1;
  for(ii=0;ii<CantHist;ii++){
    hist[ii][1]/=N;}
  for(ii=0;ii<CantHist;ii++){
    loghist[ii][0]=log(hist[ii][0]);
    loghist[ii][1]=log(hist[ii][1]);
}
}
void M19::FitLinear(void){
  double x[CantHist-1],y[CantHist-1];
  for(int ii=0;ii<CantHist-1;ii++){
    x[ii]=loghist[ii][0];
    y[ii]=loghist[ii][1];   
    //  cout<<x[ii]<<" "<<y[ii]<<endl;
  }
  Maths::Regression::Linear A(CantHist, x, y);
  cout << "    Slope = " << A.getSlope() << endl;
  cout << "Intercept = " << A.getIntercept() << endl << endl;
  cout << "Regression coefficient = " << A.getCoefficient() << endl;
  }
int main(void){
  M19 Frente;
  Crandom ran2(1);
  int t, tmax=1000000;//Tiempo maximo de la simulación
  Frente.Inicie();
  for(t=0;t<tmax;t++){
    Frente.Evolucione(ran2);
    Frente.Reorganice();
    if((t>=990000)&&((t%1000)==0)){
	cout<<t<<endl;
	Frente.Histograma();
	Frente.FitLinear();
	}
      }
  
  return 0;
}
