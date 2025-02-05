  //console.log(document.getElementById('nome'))
/*function distribuirCaractere() {
	var valor = document.getElementById('caracter').value
	document.getElementById('caracter').value= ' '
	valor = valor.trim();	

	if (isNaN(valor)) {
	 document.getElementById('letra').value= valor
	} else {
	 document.getElementById('numero').value=valor
	
}



let el= document.getElementById('caracter')

function modificarS(){
	el.style.backgroundColor= 'yellow'
}
function modificarS2() {
	var valor = el.value
	if (valor.length<3) {
		el.style.backgroundColor='red'
	} else {
		el.style.backgroundColor='green'

	}

}*/





  /*
  var listaFrutas= ["Banana", "Maçã", "morango","uva"]
  for (let i = 0; i < listaFrutas.length; i++) {
      document.write(listaFrutas[i]+ '<br/> ')
            
       }
   for (let i = 1; i < 11; i++) {
            for (let j = 1; j < 11; j++) {
              document.write(i +' x '+ j + ' = '+ (i*j)+'<br/>')  
                
            }
            document.write("<hr/>")
        }

        var listaConvidados= Array()
        listaConvidados['a']= 'Jorge'
        listaConvidados[10]= 'Jamilton'
        listaConvidados['zebra']= "José"
        listaConvidados[-1]="Ana"
        listaConvidados[true]= "Maria"

        console.log(listaConvidados)

        for (let i in listaConvidados) {
            console.log("Indice " + i +" valor  " + listaConvidados[i])
        }

        var i = 1;
        var j= 1;

        while(i<11) {
            document.write("Tabuada do "+ i +"<br/>")
            while (j < 11) {
                document.write(i +" x " + j+ " = " +(i*j)+"<br/>")
                j++
            }
            j=1
            i++




        

        //Trabalhando com parâmetros variáveis
        function soma() {
            var result = 0
            for (let i in arguments) {
                result += arguments[i]
            }
            return result
        }
        console.log(soma(7,5,4, "texto"))*/






        //BROWSER OBJECT MODEL 

/*        
var janela;

function abrirPopUp() {
    janela= window.open("http://www.google.com", "nova_janela", 'width= 300 , height = 300')
}
function fecharPopUp() {
    janela.close();
}

var alturaTela = window.screen.availHeight
var larguraTela = window.screen.availWidth

if (larguraTela <500) {
    document.write(" Você está em um celular")
} else {
    document.write(" Você está em um tablet ou pc")

}*/



var i = 5

var x = setInterval(() => { 
    document.write(i)
    i--    
    if (i==0) {
        clearInterval(x)
    }
}, 1000);




    