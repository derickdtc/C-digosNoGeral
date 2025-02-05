package javaProjeto;
import java.util.*;
import javaProjeto.Biblioteca;
   public class Calculadora{
   	public static void main(String[] args) {
         Scanner scanner= new Scanner(System.in);

         /*
   		ArrayList<Double> resultados = new ArrayList<>();
   		System.out.print("Digite o primero número")
   		double n1 = scanner.nextDouble();
   		Sytem.out.print("Digite a operação que quer desejar: + , -, *, /")
   		System.out.print("Digite o próximo número")
   		double n2= scanner.nextDouble();
         */
         System.out.print("Digite o primero número");
         int n1 = scanner.nextInt();
         Biblioteca.fatorial(n1);

   	}
   	
   }
