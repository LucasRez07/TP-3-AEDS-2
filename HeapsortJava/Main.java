import java.util.*;
import java.io.*; //Importo para utilizar os comandos necessários para ler arquivos

class LeitorCsv{
    public static Veiculo[] ler(String caminho) throws FileNotFoundException{ //Crio a classe do Leitor csv com o comando static ler da classe para ler o arquivo e no fim devolver o array de veículos
        File arquivo = new File(caminho);
        int cont=0;
        int i=0;

        Scanner scCont = new Scanner(arquivo, "UTF-8"); //Crio um scanner para ler o arquivo uma vez e determinar a quantidade de linhas que ele tem, antes eu estava fazendo direto com 500, mas como não funcionaria para qualquer arquivo, decidi mudar

        while(scCont.hasNextLine()){
            scCont.nextLine();
            cont++; //Conto as linhas
        }

        scCont.close(); 
        cont--; //Retiro a linha com as informações que não são necessárias

        Veiculo[] carros = new Veiculo[cont];

        Scanner sc = new Scanner(arquivo, "UTF-8"); //Abro o scanner definitivo

        sc.nextLine(); //Dou essa quebra de linha para pular a linha do arquivo que contém o que são cada uma das informações 

        while(sc.hasNextLine()){
            String linha = sc.nextLine();
            Veiculo car = Veiculo.parseVeiculo(linha); //Até o fim do arquivo leio cada linha como string, mando para o método parse para que um objeto veículo seja retornado
            carros[i] = car; //Adiciono este veículo retornado no vetor de veículos
            i++;
        }
        
        sc.close();
        return carros;  
   }
}

class Data{ //Crio a classe data para trabalhar com objetos da mesma
    private int ano, mes, dia; //Encapsulamento 

    Data(int ano, int mes, int dia){ 
        this.dia = dia;
        this.mes = mes;
        this.ano = ano;
    }

    public int getDia(){ //Métodos gets para retornar o atributo necessário caso eu precise
        return dia;
    }

    public int getMes(){
        return mes;
    }

    public int getAno(){
        return ano;
    }

    public String format(){ //Pego os atributos do objeto e elaboro uma string da forma exata que é pedido
        String aux;
        aux = String.format("%02d/%02d/%04d", dia, mes, ano);
        return aux;
    }

    public static Data parseData(String s){ //Recebo a string da data e transformo em um objeto Data
        String[] separado = s.split("-");

        int[] num = new int[separado.length];
        
        for(int i=0; i<num.length; i++){
            num[i] = Integer.parseInt(separado[i]);
        }

        Data aux = new Data(num[0], num[1], num[2]);
        return aux;
    }
}

 class Veiculo{ //Crio a classe veículo para trabalhar com objetos da mesma
    
    private int id, ano, cilindros;
    private double cilindrada, consumoCidade, consumoEstrada, co2;
    private String marca, modelo, categoria, transmissao, tracao, combustivel[];
    private boolean turbo;
    private Data dataRegistro; //Coloco todos os atributos que estão no enunciado e no arquivo

         Veiculo(int id, String marca, String modelo, int ano, String categoria, String[] combustivel, int cilindros, double cilindrada, 
            String transmissao, String tracao, double consumoCidade, double consumoEstrada, double co2, boolean turbo, Data dataRegistro){ 
            this.id = id;
            this.marca = marca;
            this.modelo = modelo;
            this.ano = ano;
            this.categoria = categoria;
            this.combustivel = combustivel;
            this.cilindros = cilindros;
            this.cilindrada = cilindrada;
            this.transmissao = transmissao;
            this.tracao = tracao;
            this.consumoCidade = consumoCidade;
            this.consumoEstrada = consumoEstrada;
            this.co2 = co2;
            this.turbo = turbo;
            this.dataRegistro = dataRegistro;
        }

    
        public int getId(){ //Métodos gets caso eu precise do valor de um dos atributos
            return id;
        }

        public int getAno(){
            return ano;
        }

        public int getCilindros(){
            return cilindros;
        }

        public double getCilindrada(){
            return cilindrada;
        }

        public double getConsumoCidade(){
            return consumoCidade;
        }

        public double getConsumoEstrada(){
            return consumoEstrada;
        }

        public double getCo2(){
            return co2;
        }

        public String getMarca(){
            return marca;
        }

        public String getModelo(){
            return modelo;
        }

        public String getCategoria(){
            return categoria;
        }

        public String getTransmissao(){
            return transmissao;
        }

        public String getTracao(){
            return tracao;
        }

        public String[] getCombustivel(){
            return combustivel;
        }

        public boolean getTurbo(){
            return turbo;
        }

        public static Veiculo parseVeiculo(String s){ //Método que recebe a string do arquivo e transforma em um objeto de veículo
            String[] dados = s.split(","); //Uso o split para dividir a String em um array de String com o separador ","

            int idAux = Integer.parseInt(dados[0]);
            String marcaAux = dados[1];
            String modeloAux = dados[2];
            int anoAux = Integer.parseInt(dados[3]);
            String categoriaAux = dados[4];
            String[] combustivelAux = dados[5].split(";"); //Divido o combustível em um array de String
            int cilindroAux = Integer.parseInt(dados[6]);
            double cilindradaAux = Double.parseDouble(dados[7]);
            String transmissaoAux = dados[8];
            String tracaoAux = dados[9];
            double consumoCidadeAux = Double.parseDouble(dados[10]);
            double consumoEstradaAux = Double.parseDouble(dados[11]);
            double co2Aux = Double.parseDouble(dados[12]);
            boolean turboAux = Boolean.parseBoolean(dados[13]);
            Data dataAux = Data.parseData(dados[14]);

            Veiculo carro = new Veiculo(idAux, marcaAux, modeloAux, anoAux, categoriaAux, combustivelAux, cilindroAux, cilindradaAux, transmissaoAux, tracaoAux, consumoCidadeAux, consumoEstradaAux, co2Aux, turboAux, dataAux);

            return carro;
        }

        public String combustivelFormat(){ //Crio um format específico para o combustível pois o mesmo é um array de String e precisa aparecer na tela de uma forma específica
            String aux = "";
                for(int i=0; i<combustivel.length; i++){
                    aux += combustivel[i];
                    if(i < combustivel.length - 1){
                         aux += ",";
                    }
                }
            return aux;
            
        }

        public String format(){ //Format do veiculo para aparecer na tela do jeito que é requisitado
            String aux;
            aux = String.format(Locale.US, "[%d ## %s ## %s ## %d ## %s ## [%s] ## %d ## %s ## %s ## %s ## %.2f ## %.2f ## %s ## %b ## %s]", id, marca, modelo, ano, categoria, combustivelFormat(), cilindros, cilindrada, transmissao, tracao, consumoCidade, consumoEstrada, co2, turbo, dataRegistro.format());
            return aux; //Uso o Locale.US para os números reais utilizarem "." em vez de "," como separador
        }

}

public class Main{

    public static int  maiorFilho(Veiculo[] vet, int pos, int m){ //Aqui procuro a posição do maior filho e a retorno
	    int posicao;
    		if((2*pos)+1 > m || vet[2*pos].getCo2() > vet[(2*pos)+1].getCo2() || (vet[2*pos].getCo2() == vet[(2*pos)+1].getCo2() && vet[2*pos].getTransmissao().compareToIgnoreCase(vet[(2*pos)+1].getTransmissao()) >= 0)){
			posicao = 2*pos;
		}

		else{
			posicao = (2*pos) +1;
		}

	    return posicao;
   }

    public static boolean temFilho(Veiculo[] f, int pos, int m){ //Vejo se o nó tem um filho
    	if(2*pos <= m){
		return true;
	}

	else{
		return false;
	}
    }
 

    public static void reconstruir(Veiculo[] vet, int n){ //Reconstruo o heap para continuar a ordenação
    	int i = 1;
	while(temFilho(vet, i, n) == true){ //Vejo se o nó do heap tem filho
		int posicaoFilho = maiorFilho(vet, i, n); //Se sim verifico a posição do maior para ver qual o maior filho para realizar o swap
		Veiculo filho = vet[posicaoFilho];
		if((filho.getCo2() > vet[i].getCo2() || (filho.getCo2() == vet[i].getCo2() && filho.getTransmissao().compareToIgnoreCase(vet[i].getTransmissao()) > 0))){
			Veiculo aux = vet[i];
			vet[i] = filho;
			vet[posicaoFilho] = aux;

			i = posicaoFilho;
		}

		else{//Se não pulo para o próximo elemento de uma vez
			i = n;
		}
	}
    }

    public static void construir(Veiculo[] vet, int t){ //Construo o heap de forma ordenada
    	for(int i=t; i>1 && (vet[i].getCo2() > vet[i/2].getCo2() || (vet[i].getCo2() == vet[i/2].getCo2() && vet[i].getTransmissao().compareToIgnoreCase(vet[i/2].getTransmissao()) > 0)); i/=2){
		Veiculo aux = vet[i];
		vet[i] = vet[i/2];
		vet[i/2] = aux;
	}
    }

    public static void heapsort(Veiculo[] c, int n){ //Faço o algoritmo de ordenação heapsort
    	for(int tam=2; tam<=n; tam++){ //Construo o heap(heap máximo) primeiro
		construir(c, tam);
	}

	int tam = n;

	while(tam > 1){ //Aqui que começa a ordenação
		Veiculo aux = c[1]; //Troco o primeiro elemento com o último para ordená-lo
		c[1] = c[tam];
		c[tam] = aux;

		tam--;

		reconstruir(c, tam); //Reconstruo o heap 

	}
    }

    public static void main(String[] args){
        int id = 0, cont = 0;
        Veiculo[] carros = new Veiculo[300];
        Scanner sc = new Scanner(System.in);
        try{ //Tento ler o arquivo
           // Veiculo[] cars = LeitorCsv.ler("veiculos.csv"); //Windows
            Veiculo[] cars = LeitorCsv.ler("/tmp/veiculos.csv"); //Linux //Um caminho de arquivo para o linux/verde e o outro pra minha máquina

            while(id != -1){ //Leio os ids até -1
                id = sc.nextInt();

                for(int j=0; j<cars.length; j++){

                    if(id == cars[j].getId()){ //Se o id bater com o de algum carro do array adiciono no vetor que será ordenado
                        cont++;
		        carros[cont] = cars[j];
                        j = cars.length; //Se achar dou j=cars.length para sair imediatamente do for e pular para o próximo
                    }
                }
            }

	    heapsort(carros, cont); //Chamo o método heapsort para ordenar o vetor

	    String resposta = "";

	    for(int a=1; a<=cont; a++){ //Printo o vetor ordenado
	    	resposta = carros[a].format();
	        System.out.println("" + resposta);	
	    }
        }

        catch(FileNotFoundException e){ //Se não encontrar no arquivo cai na exceção
            System.out.println("Arquivo nao encontrado");
        }
        
       

        sc.close();
    }

}
