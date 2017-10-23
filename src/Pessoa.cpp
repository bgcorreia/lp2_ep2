#include "Pessoa.h"
#include "Banheiro.h"
#include <mutex>

void Pessoa::entraNoBanheiro(sexo){

    if (sexo == 'H') {

        while (true){

            entrada.lock; // Método P(e); -- A primeira thread a executar o método é a primeira a sair.

            if ( utilizacaoGeral == MAX_UTILIZACAO ) // Verifica se chegou no limite de utilizações.
            {
                // destroy thread
            }

            else ( mulheresNoBanheiro > 0 || homensNoBanheiro == CAPACIDADE_BANHEIRO ) // Se a condição for verdadeira, o homem entra na fila.
            {
                homensDormindo++; // Incremento do contador de homens na fila.
                entrada.unlock; // Método V(e); -- Libera a fila do P(e);
                homem.lock; // "Dorme" na fila até ser liberado.
            }

            homensNoBanheiro++; // Homem entra no WC

            mulheresConsecutivos = 0; // Zera mulheres no WC.

            homensConsecutivos++; // Incrementa nº de homens no WC

            utilzacaoGeral++; // Incrementa o nº de utilizações


            // SIGNAL DE ENTRADA ( HOMEM ): AQUI OCORRE A "PASSAGEM DE BASTÃO".
            if (homensNoBanheiro < CAPACIDADE_BANHEIRO) // Verifica se cabe outro homem.
            {

                if (homensConsecutivos < UTILIZACAO_CONSEC && homensDormindo > 0 ) // Verifica se tem homens na fila e se ainda não chegou no limite de utilização.
                {

                    homensDormindo--; // Decrementa contador de homens na fila.
                    homem.unlock; // Libera um homem dormindo na fila a entrar no banheiro.


                } else {

                    entrada.unlock; // Método V(e); -- Libera a fila do P(e);

                }

            } else {

                entrada.unlock; // Método V(e); -- Libera a fila do P(e);
            }

            // SEÇÃO CRÍTICA
           utilizaOBanheiro();
           // FIM DA SEÇÃO CRÍTICA

            entrada.lock; // P(e); -- Necessário para garantir atomicidade.

            homensNoBanheiro--; // Decrementa o contador de homens no banheiro.

            // SIGNAL DE SAÍDA ( HOMEM ): AQUI OCORRE A "PASSAGEM DE BASTÃO".
            if (homensConsecutivos >= UTILIZACAO_CONSEC && mulheresDormindo > 0 && homensNoBanheiro == 0) // Condição para liberar uma mulher da fila.
                {

                mulheresDormindo--; // Decrementa contador de mulheres na fila.
                mulher.unlock; // Libera uma mulher da fila.

            else if ((homensConsecutivos < UTILIZACAO_CONSEC && homensDormindo > 0 && mulheresDormindo == 0) || (homensConsecutivos >= UTILIZACAO_CONSEC && mulheresDormindo == 0)) // Condição para liberar um homem da fila.
            {

                homensDormindo--; // Decrementa contador de homens na fila
                homem.unlock; // Libera um homem da fila.

            } else {

                entrada.unlock; // Método V(e);

            }

          // SEÇÃO NÃO CRÍTICA
            ficaMoscando();

        }

    } else if (sexo == 'M') {

        while (true){

            entrada.lock; // Método P(e); -- A primeira thread a executar o método é a primeira a sair.

            if ( utilizacaoGeral == MAX_UTILIZACAO ) // Verifica se chegou no limite de utilizações.
            {
                // destroy thread
            }

            else ( homensNoBanheiro > 0 || mulheresNoBanheiro == CAPACIDADE_BANHEIRO ) // Se a condição for verdadeira, a mulher entra na fila.
            {
                mulheresDormindo++; // Incremento do contador de mulheres na fila.
                entrada.unlock; // Método V(e); -- Libera a fila do P(e);
                mulher.lock; // "Dorme" na fila até ser liberada.
            }

            mulheresNoBanheiro++; // Mulher entra no WC

            homensConsecutivos = 0; // Zera homens no WC

            mulheresConsecutivos++; // Incrementa nº de mulheres no WC

            utilzacaoGeral++; // Incrementa o nº de utilizações


            // SIGNAL DE ENTRADA ( MULHER ): AQUI OCORRE A "PASSAGEM DE BASTÃO".
            if (mulheresNoBanheiro < CAPACIDADE_BANHEIRO) // Verifica se cabe outra mulher.
            {

                if (mulheresConsecutivos < UTILIZACAO_CONSEC && mulheresDormindo > 0 ) // Verifica se tem mulheres na fila e se ainda não chegou no limite de utilização.
                {

                    mulheresDormindo--; // Decrementa contador de mulheres na fila.
                    mulher.unlock; // Libera uma mulher dormindo na fila a entrar no banheiro.


                } else {

                    entrada.unlock; // Método V(e); -- Libera a fila do P(e);

                }

            } else {

                entrada.unlock; // Método V(e); -- Libera a fila do P(e);
            }

            // SEÇÃO CRÍTICA
           utilizaOBanheiro();
           // FIM DA SEÇÃO CRÍTICA

            entrada.lock; // P(e); -- Necessário para garantir atomicidade.

            mulheresNoBanheiro--; // Decrementa o contador de mulheres no banheiro.

            // SIGNAL DE SAÍDA ( MULHER ): AQUI OCORRE A "PASSAGEM DE BASTÃO".
            if (mulheresConsecutivos >= UTILIZACAO_CONSEC && homensDormindo > 0 && mulheresNoBanheiro == 0) // Condição para liberar um homem da fila.
                {

                homensDormindo--; // Decrementa contador de homens na fila.
                homem.unlock; // Libera um homem da fila.

            else if ((mulheresConsecutivos < UTILIZACAO_CONSEC && mulheresDormindo > 0 && homensDormindo == 0) || (mulheresConsecutivos > UTILIZACAO_CONSEC && homensDormindo == 0)) // Condição para liberar uma mulher da fila.
            {

                mulheresDormindo--; // Decrementa contador de mulheres na fila
                mulher.unlock; // Libera uma mulher da fila.

            } else {

                entrada.unlock; // Método V(e);

            }

            // SEÇÃO NÃO CRÍTICA
            ficaMoscando();

        }

    } else {

        cout << endl << "Sexo indefinido." << endl;

    }


}
