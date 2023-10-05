#include <iostream>

class enemigo {

    private:
        int Vidac;
        int Velocidad=10;
        int Damage=15;

    public:
        //por mientras no recibe el sprite , por falta de libreria
        enemigo(int Vida)
        {if(Vida<=0){
            Vidac=50;
            }}
        
        void Movimiento(int Variant){
            if (Variant==1){
                Velocidad=+5;
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
                Damage=5;
            }
        }
};