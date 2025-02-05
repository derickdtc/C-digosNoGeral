package Java;

import java.io.*;
import java.util.*;

class Pessoa {
    private long codigo;
    private String nome;
    private List<Bem> bens;

    public Pessoa(long codigo, String nome) {
        this.codigo = codigo;
        this.nome = nome;
        this.bens = new ArrayList<>();
    }

    public long getCodigo() {
        return codigo;
    }

    public String getNome() {
        return nome;
    }

    public List<Bem> getBens() {
        return bens;
    }

    public void adicionarBem(Bem bem) {
        bens.add(bem);
    }

    public void removerBem(int codigoBem) {
        bens.removeIf(bem -> bem.getCodigo() == codigoBem);
    }

    public double calcularValorTotalBens() {
        double total = 0;
        for (Bem bem : bens) {
            total += bem.getValor();
        }
        return total;
    }
}

class Bem {
    private int codigo;
    private String nome;
    private double valor;

    public Bem(int codigo, String nome, double valor) {
        this.codigo = codigo;
        this.nome = nome;
        this.valor = valor;
    }

    public int getCodigo() {
        return codigo;
    }

    public String getNome() {
        return nome;
    }

    public double getValor() {
        return valor;
    }
}

public class Main {
    private static final String FILENAME = "dados.txt";
    private static List<Pessoa> pessoas = new ArrayList<>();

    public static void main(String[] args) {
        carregarDados();
        Scanner scanner = new Scanner(System.in);

        int opcao;
        do {
            System.out.println("Menu:");
            System.out.println("1 => Inserir pessoa");
            System.out.println("2 => Inserir bens de uma pessoa");
            System.out.println("3 => Excluir pessoa");
            System.out.println("4 => Excluir bem de uma pessoa");
            System.out.println("5 => Listar Pessoas");
            System.out.println("6 => Consultar pessoa");
            System.out.println("7 => Sair");
            System.out.print("Escolha uma opção: ");
            opcao = scanner.nextInt();
            scanner.nextLine(); // Limpar o buffer do scanner

            switch (opcao) {
                case 1:
                    inserirPessoa(scanner);
                    break;
                case 2:
                    inserirBens(scanner);
                    break;
                case 3:
                    excluirPessoa(scanner);
                    break;
                case 4:
                    excluirBem(scanner);
                    break;
                case 5:
                    listarPessoas();
                    break;
                case 6:
                    consultarPessoa(scanner);
                    break;
                case 7:
                    salvarDados();
                    System.out.println("Programa encerrado.");
                    break;
                default:
                    System.out.println("Opção inválida.");
            }
        } while (opcao != 7);
    }

    private static void inserirPessoa(Scanner scanner) {
        System.out.print("Digite o código da pessoa: ");
        long codigo = scanner.nextLong();
        scanner.nextLine(); // Limpar o buffer do scanner
        System.out.print("Digite o nome da pessoa: ");
        String nome = scanner.nextLine();
        Pessoa pessoa = new Pessoa(codigo, nome);
        pessoas.add(pessoa);
        System.out.println("Pessoa adicionada com sucesso.");
    }

    private static void inserirBens(Scanner scanner) {
        System.out.print("Digite o código da pessoa: ");
        long codigoPessoa = scanner.nextLong();
        scanner.nextLine(); // Limpar o buffer do scanner
        Pessoa pessoa = buscarPessoaPorCodigo(codigoPessoa);
        if (pessoa == null) {
            System.out.println("Pessoa não encontrada.");
            return;
        }
        System.out.print("Digite o código do bem: ");
        int codigoBem = scanner.nextInt();
        scanner.nextLine(); // Limpar o buffer do scanner
        System.out.print("Digite o nome do bem: ");
        String nomeBem = scanner.nextLine();
        System.out.print("Digite o valor do bem: ");
        double valorBem = scanner.nextDouble();
        Bem bem = new Bem(codigoBem, nomeBem, valorBem);
        pessoa.adicionarBem(bem);
        System.out.println("Bem adicionado com sucesso.");
    }

    private static void excluirPessoa(Scanner scanner) {
        System.out.print("Digite o código da pessoa que deseja excluir: ");
        long codigoPessoa = scanner.nextLong();
        Pessoa pessoa = buscarPessoaPorCodigo(codigoPessoa);
        if (pessoa == null) {
            System.out.println("Pessoa não encontrada.");
            return;
        }
        pessoas.remove(pessoa);
        System.out.println("Pessoa removida com sucesso.");
    }

    private static void excluirBem(Scanner scanner) {
        System.out.print("Digite o código da pessoa: ");
        long codigoPessoa = scanner.nextLong();
        scanner.nextLine(); // Limpar o buffer do scanner
        Pessoa pessoa = buscarPessoaPorCodigo(codigoPessoa);
        if (pessoa == null) {
            System.out.println("Pessoa não encontrada.");
            return;
        }
        System.out.print("Digite o código do bem que deseja excluir: ");
        int codigoBem = scanner.nextInt();
        pessoa.removerBem(codigoBem);
        System.out.println("Bem removido com sucesso.");
    }

    private static void listarPessoas() {
        System.out.println("Cod Pessoa Nome da Pessoa Val Tot Bens");
        for (Pessoa pessoa : pessoas) {
            double valorTotalBens = pessoa.calcularValorTotalBens();
            System.out.printf("%3d %-30s R$ %.2f\n", pessoa.getCodigo(), pessoa.getNome(), valorTotalBens);
        }
    }

    private static void consultarPessoa(Scanner scanner) {
        System.out.print("Digite o código da pessoa: ");
        long codigoPessoa = scanner.nextLong();
        scanner.nextLine(); // Limpar o buffer do scanner
        Pessoa pessoa = buscarPessoaPorCodigo(codigoPessoa);
        if (pessoa == null) {
            System.out.println("Pessoa não encontrada.");
            return;
        }
        System.out.println("Dados da pessoa:");
        System.out.println("Código: " + pessoa.getCodigo());
        System.out.println("Nome: " + pessoa.getNome());
        System.out.println("Bens:");
        for (Bem bem : pessoa.getBens()) {
            System.out.println("  Código: " + bem.getCodigo() + ", Nome: " + bem.getNome() + ", Valor: R$ " + bem.getValor());
        }
    }

    private static Pessoa buscarPessoaPorCodigo(long codigo) {
        for (Pessoa pessoa : pessoas) {
            if (pessoa.getCodigo() == codigo) {
                return pessoa;
            }
        }
        return null;
    }

    private static void carregarDados() {
    try (ObjectInputStream ois = new ObjectInputStream(new FileInputStream(FILENAME))) {
        Object obj = ois.readObject();
        if (obj instanceof List<?>) {
            List<?> list = (List<?>) obj;
            if (!list.isEmpty() && list.get(0) instanceof Pessoa) {
                pessoas = (List<Pessoa>) obj;
                System.out.println("Dados carregados com sucesso.");
            } else {
                System.out.println("Erro ao carregar dados: formato inválido.");
            }
        } else {
            System.out.println("Erro ao carregar dados: formato inválido.");
        }
    } catch (FileNotFoundException e) {
        System.out.println("Arquivo de dados não encontrado. Será criado um novo arquivo ao salvar os dados.");
    } catch (IOException | ClassNotFoundException e) {
        System.out.println("Erro ao carregar dados: " + e.getMessage());
    }
}


    private static void salvarDados() {
        try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(FILENAME))) {
            oos.writeObject(pessoas);
            System.out.println("Dados salvos com sucesso.");
        } catch (IOException e) {
            System.out.println("Erro ao salvar dados: " + e.getMessage());
        }
    }




}