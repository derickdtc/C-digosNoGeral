/*
const units = ['','one','two','three','four','five','six','seven','eight','nine']
const specials=['','eleven','twelve','thirteen','fourteen','fifteen','sixteen','seventeen','eighteen','nineteen']
const hundreds= ['','one hundred','two hundred','three hundred','four hundred','five hundred','six hundred','seven hundred','eight hundred','nine hundred']
const dozens=['','ten','twenty','thirty','forty','fifty','sixty','seventy','eight','ninety']
function fullNumber(num) {
    
    if (n==0) {
        return 'zero'
    } else if(n>=1&& n<10) {
      switch (n) {
        case 1:
            return units[1]
            break;
            case 2:
            return units[2]
            break;
            case 3:
            return units[3]
            break;
            case 4:
            return units[4]
            break;
            case 5:
            return units[5]
            break;
            case 6:
            return units[6]
            break;
            case 7:
            return units[7]
            break;
        default:
            break;
      }               

    }
}
console.log(fullNumber(0))*/
//EX10
const lista= [1,2,3,4,5,6,7] 
function replicarI4x(list) {
    return list.map((x)=>x*4)
}
replicarI4x(lista)