package Java;
import java.util.Scanner;
import java.util.Arrays;

 public class Att1{
 	 public static void main(String[] args) {
 		Scanner scanner= new Scanner(System.in);
 		int nCanditados=0;
 		System.out.println("Digite o número de vagas");
 		int nVagas = scanner.nextInt(); 		
 		scanner.nextLine();
 		char[] gabarito= new char[10];
 		for (int i=0; i<10 ; i++ ) {
 		System.out.print("Digite a reposta da questão "+(i+1)+": ");
 		gabarito[i]= scanner.next().charAt(0);	
 		}
 		// scanner.nextLine();

 		String[][] tabela= new String[50][11];
 		boolean continuar = true;
 		while ( continuar) {
 		if ( !scanner.hasNextLine()) {
 			scanner.close();
 			break;
 		}		


		 		for (int j=0; j<tabela[nCanditados].length ; j++ ) {
 		 	   	System.out.print("Dados do candidato: Primeiro nome, depois reposta de questão "+j+" em caixa alta: ");
 		 	  
 				tabela[nCanditados][j] =scanner.next();
 				if (tabela[nCanditados][j].equals("*")) {
 					
 					continuar=false;
 					break;
 				}else{
 					String nome=tabela[nCanditados][0];
 				} 				
	 					

 			}
 			 nCanditados++;

 		}
 		scanner.close();
 		classificarTabela(tabela,gabarito,nVagas, nCanditados);
 		

	}
	public static void classificarTabela(String[][] tabela, char[] gabarito, int nVagas, int nCanditados){
 		 Integer[] numCorretas = new Integer[tabela.length];

 		// Inicializar todos os elementos do array como 0
    		Arrays.fill(numCorretas, 0);

 		for (int i=0; i<nCanditados ;i++ ) {
 			for (int j=1; j<11 ;j++  ) {
 			if (tabela[i][j].charAt(0)==gabarito[j-1]) {
 				numCorretas[i]++;

 			}	
 			}
 		
 		}
 		Integer[] indices = new Integer[tabela.length];
        for (int i = 0; i < indices.length; i++) {
            indices[i] = i;
        }
        Arrays.sort(indices, (a, b) -> Integer.compare(numCorretas[b], numCorretas[a]));
    
   		for (int i = 0; i < nVagas ; i++) {
    String nome = tabela[indices[i]][0];
    int numAcertos = numCorretas[indices[i]];
    System.out.println(nome + " " + numAcertos + " Aprovado(a)");
}

		for (int i = nVagas; i < tabela.length; i++) {
	    String nome = tabela[indices[i]][0];
	    int numAcertos = numCorretas[indices[i]];

	     if (nome!=null && nome instanceof String  ) {
             System.out.println(nome + " " + numAcertos + " Nao Aprovado(a)");
              
            }else{

            }

	}


  }      
 }