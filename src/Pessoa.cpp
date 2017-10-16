#include "Pessoa.h"
#include <mutex>

void Pessoa::entraNoBanheiroH(){

    while (true){

        entrada.lock; // Entra na fila

        if ( mulheresNoBanheiro > 0 ){
            homensDormindo++;
            entrada.unlock;
            homem.lock;
        }

        homensNoBanheiro++; // Entra no banheiro

        mulheresConsecutivos = 0;
        homensConsecutivos++;

        // Antes do signal, apenas um processo executa por vez

        // SIGNAL - AJUSTAR RESTRIÇÕES
        if (homensNoBanheiro == 0 && mulheresDormindo > 0)
            mulheresDormindo--;
            mulher.unlock;
        } else if (mulheresNoBanheiro == 0 && homensNoBanheiro == 0 && homensDormindo > 0) {
            homensDormindo--;
            homem.unlock;
        } else {
            entrada.unlock;
        }

        /*
         SECAO CRITICA
        */

        entrada.lock;

        homensNoBanheiro--;

        //SIGNAL- AJUSTAR RESTRIÇÕES
        if (homensNoBanheiro == 0 && mulheresDormindo > 0)
            mulheresDormindo--;
            mulher.unlock;
        } else if (mulheresNoBanheiro == 0 && homensNoBanheiro == 0 && homensDormindo > 0) {
            homensDormindo--;
            homem.unlock;
        } else {
            entrada.unlock;
        }

        // FICA MOSCANDO - NON CRITICAL

    }

}

void Pessoa::entraNoBanheiroM(){

    while (true){

        entrada.lock;

        if ( homensNoBanheiro > 0 ){
            mulheresDormindo++;
            entrada.unlock;
            mulheres.lock;
        }

        mulheresNoBanheiro++;

        // SIGNAL

        /*
         SECAO CRITICA
        */

        entrada.lock;

        mulheresNoBanheiro--;

        //SIGNAL

        // FICA MOSCANDO - NON CRITICAL

    }



}
