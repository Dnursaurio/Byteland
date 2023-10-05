#include <iostream>

class Jugador {

    private:
        int Vidac;
        int Velocidad=10;
        int Damage=15;

    public:
        //por mientras no recibe el sprite , por falta de libreria
        Jugador(int Vida)
        {if(Vida<=0){
            Vidac=100;
            }}
        
        void Movimiento(int Variant){
            if (Variant==1){
                Velocidad=-4;
            }
            if (Variant==0){
                Velocidad=10;
            }
        }
        void Ataque(int Multiplicador, int Variant){
            if (Multiplicador>0 && Variant==1){
                Damage=Damage+(Damage*Multiplicador);
            } 
            else if(Multiplicador==0 && Variant==1){
                Damage=15;
            }
        }
};