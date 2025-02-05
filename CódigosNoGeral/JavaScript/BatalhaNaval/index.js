 let errou = 'X';
let acertou =  'Y';
let ptsJogador;
let ptsPC;

function criarTabuleiro() {
    const tabuleiro = document.getElementById('tabuleiro');
    for (let index = 1; index < 101; index++) {
        const div = document.createElement('div')
        div.classList.add('unidade')
        div.setAttribute('data-posicao' , index )
        div.addEventListener('click', jogadorJoga);
        tabuleiro.appendChild(div);
        
    }
}

function posicionarNavioJogador(nomeNavio , tamanhoNavio) {
    const posicoes = window.prompt(`Posicione o ${nomeNavio} que tem tamanho ${tamanhoNavio} (ex: Se o tamanho fosse 3 , você deveria digitar: 1,2,3)`);
    if (posicoes != "" ) {
        const posicoesArray = posicoes.split(',').map(Number);
        posicoesArray.forEach(posicao => {
            const unidade = document.querySelector(`.unidade[data-posicao='${posicao}']`);
            if (unidade && !(unidade.classList.contains('navio')) && !(unidade.classList.contains('navioPC'))) {
                unidade.classList.add('navio');
                //unidade.classList.add(nomeNavio);
            }else{
                alert("Os navios se sobrepõem , o jogo reiniciará após o término dos cadastros")
                window.location.href= 'file:///C:/CODING/JavaScript/BatalhaNaval/index.html'
    
            }
        });
    }else{
        alert("Você não informou a posição do navio, o jogo reiniciará após o término dos cadastros")
        window.location.href= 'file:///C:/CODING/JavaScript/BatalhaNaval/index.html'
    }
}


function posicionarNavioPC(arrayNumero) {
    if (arrayNumero && arrayNumero.length > 0) {
        arrayNumero.forEach(posicao => {
            const unidade = document.querySelector(`.unidade[data-posicao='${posicao}']`);
            if (unidade && !(unidade.classList.contains('navio')) && !(unidade.classList.contains('navioPC'))) {
                unidade.classList.add('navioPC');
                // unidade.classList.add(nomeNavio);
            } else {
              return posicionarNavioPC()
            }
        });
    } 
}

    




function posicionarNaviosJogador(){
    const navios = ['submarino', 'cruzador', 'destroyer', 'porta-aviões'];
    const tamanhos = [1,2,3,4 ];
     navios.forEach((navio, index) => posicionarNavioJogador(navio , tamanhos[index]));

}
function posicionarNaviosPC() {
    let a = Math.round(Math.random() *(19-1) +1) ;
    let b = Math.round(Math.random() *(39-21) +21) ;
    let c = Math.round(Math.random() *(65-43) +43) ;
    let d = Math.round(Math.random() *(95-66) +66) ;


    const arraysNumeros = [
        [a  ],
        [b,b+1],
        [c,c+1 , c+2 ],
        [d , d +1 ,d+2 , d+3  ]
    ];
     arraysNumeros.forEach((arrayNumero) => posicionarNavioPC(arrayNumero));

}



function jogadorJoga(Event){
    window.alert("Sua vez de jogar")
    const unidade = Event.target; 

    if (unidade.classList.contains('navioPC')) {
        unidade.classList.add('acertou');
        ptsJogador++;

        alert("Vc acertou o navio")
    } else {
        unidade.classList.add('errou')
        alert("Você errou")
    }

}

function navioJoga(params) {
    const tiroNavio = Math.round(Math.random() * (100-1) + 1)
    const unidade = document.querySelector(`.unidade[data-posicao = '${tiroNavio}']`)
    if (unidade.classList.contains('navio')) {
        unidade.classList.add('acertou');
        ptsPC++;
        alert('O computador acertou um navio seu!');
    } else {
        unidade.classList.add('errou');
        
        alert('O computador errou');
    }

}


console.log()
//Funções
criarTabuleiro()
posicionarNaviosJogador()

posicionarNaviosPC()


while (ptsJogador != 4 || ptsPC!=4) {
    jogadorJoga()
    navioJoga()   
  }

  if (ptsJogador == 4) {
    alert("Você ganhou")
  } else if(ptsPC ==4 ){
    alert("Você perdeu")
  }else{
    
  }
