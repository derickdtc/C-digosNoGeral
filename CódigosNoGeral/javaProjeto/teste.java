package Java;
import java.util.Scanner;
import java.util.Random;
public class teste{
	public static void main(String[] args){        
        Scanner scanner= new Scanner(System.in);
        Random random= new Random();
        /*
        System.out.println("Digite a nota 1");
        double n1= scanner.nextDouble();
        System.out.println("Digite a nota 2");
        double n2= scanner.nextDouble();
         calcMedia(n1,n2);
       
        System.out.print("Digite qualquer tecla e pressione Enter para parar o cronômetro: ");
        long tempoInMilli = System.currentTimeMillis();
        scanner.nextLine();
        long tempoFinalMilli= System.currentTimeMillis();
        long tempoTotalMilli= tempoFinalMilli - tempoInMilli;
        long tempoTotalSeg= tempoTotalMilli/1000;
        System.out.println("Tempo decorrido: " + tempoTotalSeg + " segundos.");
        
        System.out.println("Digite o valor inicial");
        double vi= scanner.nextDouble();
        System.out.println("Digite o valor final");
        double vf= scanner.nextDouble();
         calcAumento(vi,vf);
        */
         System.out.println("Digite n1");
        int n1= scanner.nextInt(); 
        int n1F = fatorial(n1);
          System.out.println(n1F);
          

    }
    /*
    public static void calcAumento(double vi, double vf){
        if (vi==vf) {
        System.out.println("Não houve alteração no valor do produto");
            
        }else if(vi>vf) {
            double percent= (vi/vf-1)*100;
            System.out.println("O preço do produto diminuiu "+percent+"%");
        }else{
            double percent= (vf/vi-1)*100;            
            System.out.println("O preço do produto aumentou "+percent+"%");
        }
    }
      
     public static void calcMedia(double x, double y){
        double media = (4*x + 6*y)/10;
           if (media>= 6) {
         System.out.println("A média é "+media+" e o aluno foi aprovado.");

        }else {
         System.out.println("A média é "+media+" e o aluno foi reprovado.");
            
        }        
     } */   
     public static int fatorial(int n1){
        if (n1==0 || n1==1) {
            return 1;
        }else{
         return n1* fatorial(n1-1);
           
        }
     }
}