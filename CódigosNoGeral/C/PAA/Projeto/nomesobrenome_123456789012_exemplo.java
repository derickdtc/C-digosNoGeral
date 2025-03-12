import java.io.*;

public class nomesobrenome_123456789012_exemplo {
	public static void main(String[] args) {
		try {
			// Ilustrando o uso de argumentos do programa
			System.out.println("#ARGS = " + args.length);
			System.out.println("ARG1 = " + args[0] + ", ARG2 = " + args[1]);
			// Abrindo os arquivos
			BufferedReader input = new BufferedReader(new FileReader(args[0]));
			BufferedWriter output = new BufferedWriter(new FileWriter(args[1]));
			// ...
			// Fechando os arquivos
			input.close();
			output.close();
		}
		catch(Exception e) {
			e.printStackTrace();
		}
	}
}
