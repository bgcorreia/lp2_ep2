#include "Pessoa.h"
#include "Banheiro.h"
#include <mutex>

void Pessoa::entraNoBanheiro(sexo){

    if (sexo == 'M') {

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
            if (homensNoBanheiro == 0 && mulheresDormindo > 0) {
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
            if (homensNoBanheiro == 0 && mulheresDormindo > 0) {
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

    } else if (sexo == 'F') {

        while (true){

            entrada.lock; // Formação da fila

            if ( utilizacaoGeral == MAX_UTILIZACAO ) {

                // destroy thread
                return 1; // DUVIDA

            } else ( homensNoBanheiro > 0 || mulheresNoBanheiro == CAPACIDADE_BANHEIRO ){
                mulheresDormindo++; // Espera na fila
                entrada.unlock; // Libera entrada na fila

                //if ( !(homensDormindo > 0 && mulheresConsecutivos >= UTILIZACAO_CONSEC ) ) { // DUVIDA
                    mulher.lock; // Permite a mulher entrar no banheiro
                //} else {
                    mulheresDormindo--; // Tira mulher da fila

                //}
            }

            mulheresNoBanheiro++; // Mulher entra no WC

            homensConsecutivos = 0; // Zera homens no WC

            mulheresConsecutivos++; // Incrementa nº de mulheres no WC

            utilzacaoGeral++; // Incrementa o nº de utilizações


            //SIGNAL- AJUSTAR RESTRIÇÕES
            if (mulheresNoBanheiro <= CAPACIDADE_BANHEIRO) {

                if ( homensDormindo == 0 && mulheresConsecutivos < UTILIZACAO_CONSEC && mulheresDormindo > 0 ){

                    //mulheresDormindo--; // Tira mulher da fila
                    mulher.unlock; // Permite entrada no WC

                } else {

                    entrada.unlock; // Libera entrada na fila

                }

            } else {

                entrada.unlock; // Libera entrada na fila

            }

            /*
             SECAO CRITICA
            */

            entrada.lock; // DÚVIDA

            mulheresNoBanheiro--;

            //SIGNAL- AJUSTAR RESTRIÇÕES
            if (mulheresConsecutivos >= UTILIZACAO_CONSEC && homensDormindo > 0 && mulheresNoBanheiro == 0) { // Libera Homem

                homensDormindo--;
                homem.unlock;

            else if (mulheresConsecutivos < UTILIZACAO_CONSEC && mulheresDormindo > 0 && homensDormindo == 0){

                mulheresDormindo--;
                mulher.unlock;

            } else { // Libera entrada na fila

                entrada.unlock;

            }

            // FICA MOSCANDO - NON CRITICAL

        }

    } else {

        cout << endl << "Sexo indefinido." << endl;

    }


}
