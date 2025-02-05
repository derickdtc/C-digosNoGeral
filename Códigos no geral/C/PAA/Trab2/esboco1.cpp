
int main(int argc, char const *argv[])
{
    /*
// Exibindo os vetores para verificar se a leitura foi bem-sucedida
    /*
    cout << "Vetores lidos do arquivo:" << endl;
    for (int i = 0; i < numVetores; i++) {
        cout << "Vetor " << i << ": ";
        for (int j = 0; j < tamanhos[i]; j++) {
            cout << vetores[i][j] << " ";
        }
        cout << endl;
    }*/
    //Aplicando as ordenações lomuto padrao
    for (int i = 0; i < numVetores; i++)
    {
        //quickSortLomutoPadrao(vetores[i], 0 , tamanhos[i] - 1);
    } 
    
    //cout << "\n \n Vetores ordenados com Lomuto padrao" << endl;
    //mostrarVetoresPosOrdenacao(vetores , tamanhos , numVetores);
    // Restaurando os vetores antes de cada ordenação
    //restaurar(vetores , tamanhos , numVetores, vetoresOriginais);
    //Aplicando as ordenações hoare padrao
    for (int i = 0; i < numVetores; i++)
    {
        //quickSortHoarePadrao(vetores[i], 0 , tamanhos[i] - 1);
    }   
    //cout << "\n \n Vetores ordenados com Hoare padrao" << endl;
    //mostrarVetoresPosOrdenacao(vetores , tamanhos , numVetores);
    restaurar(vetores , tamanhos , numVetores, vetoresOriginais);
    //mostrarVetoresPosOrdenacao(vetores , tamanhos , numVetores);
    //Aplicando as ordenações Lomuto mediana
    for (int i = 0; i < numVetores; i++)
    {
        quickSortLomutoMediana(vetores[i], 0 , tamanhos[i] - 1);
    }   
    //cout << "\n \n Vetores ordenados com Lomuto mediana" << endl;
    //mostrarVetoresPosOrdenacao(vetores , tamanhos , numVetores);
    restaurar(vetores , tamanhos , numVetores, vetoresOriginais);
    //mostrarVetoresPosOrdenacao(vetores , tamanhos , numVetores);
    //Aplicando as ordenações hoare mediana
    for (int i = 0; i < numVetores; i++)
    {
        quickSortHoareMediana(vetores[i], 0 , tamanhos[i] - 1);
    }   
   // cout << "\n \n Vetores ordenados com Hoare mediana" << endl;
   // mostrarVetoresPosOrdenacao(vetores , tamanhos , numVetores);
    restaurar(vetores , tamanhos , numVetores, vetoresOriginais);
    //mostrarVetoresPosOrdenacao(vetores , tamanhos , numVetores);
    //Aplicando as ordenações Lomuto aleatorio
    for (int i = 0; i < numVetores; i++)
    {
        quickSortLomutoAleatorio(vetores[i], 0 , tamanhos[i] - 1);
    }   
    //cout << "\n \n Vetores ordenados com lomuto aleatorio" << endl;
    //mostrarVetoresPosOrdenacao(vetores , tamanhos , numVetores);
    restaurar(vetores , tamanhos , numVetores, vetoresOriginais);
    //mostrarVetoresPosOrdenacao(vetores , tamanhos , numVetores);
    //Aplicando as ordenações hoare aleatorio
    for (int i = 0; i < numVetores; i++)
    {
        quickSortHoareAleatorio(vetores[i], 0 , tamanhos[i] - 1);
    }   
    //cout << "\n \n Vetores ordenados com Hoare aleatorio" << endl;
    //mostrarVetoresPosOrdenacao(vetores , tamanhos , numVetores);
    restaurar(vetores , tamanhos , numVetores, vetoresOriginais);
   // mostrarVetoresPosOrdenacao(vetores , tamanhos , numVetores);
*/
    return 0;
}

