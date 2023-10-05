#include <iostream>

class Arma{

    private:
        float Multiplicador=0;
        int Type;
    public:
        Arma(int Tipo){
            if (Tipo==1){
                Multiplicador=1,2;}
            if (Tipo==2){
                Multiplicador=1,5;}
            if (Tipo==3){
                Multiplicador=2,0;}
            if (Tipo=100){
                Multiplicador=3000,1;}
        }   
        int getMultiplicador(){
            return Multiplicador;}    
};