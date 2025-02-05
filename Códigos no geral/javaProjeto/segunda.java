package Java;
import java.util.Scanner;
import java.util.Random;



public class segunda{
	public static void main(String[] args){
		Scanner scanner= new Scanner(System.in);
		Random random= new Random();
		int limiteInferior = 1; // Limite inferior
        int limiteSuperior = 50;  // Limite superior

		/*
		int[] k= {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
		for (int i = 0; i < k.length/2; i++) {			
				int valorArm= k[i];
				k[i]=k[k.length-1-(1*i)];
				k[k.length-1-(1*i)]=valorArm;
			}
		
		for (int i = 0; i < k.length; i++) {
			System.out.println(k[i]);
		}
		
		int[] locClient = new int[500];
		for (int i=0; i<500 ; i++ ) {
			locClient[i]= random.nextInt(limiteSuperior- limiteInferior+1)+limiteInferior;
	
		}
		double[] qtdFilmesGratis = new double[500];
		for (int i=0;i<500 ;i++ ) {
			qtdFilmesGratis[i]= Math.round(locClient[i]/10);

		}
		for (int i=0;i<500 ;i++) {
			if (qtdFilmesGratis[i]<1) {
				
			}else{
			System.out.println("O cliente "+(i+1)+" tem direito a "+qtdFilmesGratis[i]+" filmes grátis");

			}
		}
		*/
		char[][] matriz  = new char[12][12];
		for (int i=0; i< matriz.length ;i++ ) {
			for (int j=0; j< matriz[i].length ;j++ ) {
				matriz[i][j]=' ';
			}
		}
		  for (int i = 0; i <12; i++) {
          for (int j = 12; j >1; j--) {
             matriz[i][j] = '*';
             }
        }
        
        // Exibir a matriz
        for (int i = 0; i < matriz.length; i++) {
            for (int j = 0; j < matriz[i].length; j++) {
                System.out.print(matriz[i][j] + " ");
            }
            System.out.println();
        }
	}
}
