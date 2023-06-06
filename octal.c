/*
    ----------------------------------------------------------------------
    BNF resultante:                     |REGLA SEMANTICA
    ----------------------------------------------------------------------
    OCTAL   -> ' ' SUMA                 |OCTAL.num=SUMA.num
    OCTAL   -> 0 OCTAL                  |OCTAL.num=OCTAL'.num
    OCTAL   -> 1 OCTAL                  |OCTAL.num=OCTAL'.num
    OCTAL   -> 2 OCTAL                  |OCTAL.num=OCTAL'.num
    OCTAL   -> 3 OCTAL                  |OCTAL.num=OCTAL'.num
    OCTAL   -> 4 OCTAL                  |OCTAL.num=OCTAL'.num
    OCTAL   -> 5 OCTAL                  |OCTAL.num=OCTAL'.num
    OCTAL   -> 6 OCTAL                  |OCTAL.num=OCTAL'.num
    OCTAL   -> 7 OCTAL                  |OCTAL.num=OCTAL'.num
    OCTAL   -> 8 RESTA                  |OCTAL.num=RESTA.num
    OCTAL   -> 9 RESTA                  |OCTAL.num=RESTA.num
    OCTAL   -> ϵ                        |OCTAL.num=num
    SUMA    -> OCTAL                    |SUMA.num=OCTAL.num + 1
    RESTA   -> DECIMAL                  |RESTA.num= DECIMAL.num - 1
    DECIMAL -> ' ' SUMA                 |DECIMAL.num=SUMA.num
    DECIMAL -> 0 DECIMAL                |DECIMAL.num=DECIMAL'.num
    DECIMAL -> 1 DECIMAL                |DECIMAL.num=DECIMAL'.num
    DECIMAL -> 2 DECIMAL                |DECIMAL.num=DECIMAL'.num
    DECIMAL -> 3 DECIMAL                |DECIMAL.num=DECIMAL'.num
    DECIMAL -> 4 DECIMAL                |DECIMAL.num=DECIMAL'.num
    DECIMAL -> 5 DECIMAL                |DECIMAL.num=DECIMAL'.num
    DECIMAL -> 6 DECIMAL                |DECIMAL.num=DECIMAL'.num
    DECIMAL -> 7 DECIMAL                |DECIMAL.num=DECIMAL'.num
    DECIMAL -> 8 DECIMAL                |DECIMAL.num=DECIMAL'.num
    DECIMAL -> 9 DECIMAL                |DECIMAL.num=DECIMAL'.num
    DECIMAL -> ϵ                        |DECIMAL.num=num
    ----------------------------------------------------------------------
    Conjunto primero:                   |Conjunto Siguiente
    ----------------------------------------------------------------------
    P(OCTAL)={0,1,2,3,4,5,6,7,8,9,' ',ϵ}  |S(OCTAL)={$}
    P(SUMA)={}                            |S(SUMA)={$}
    P(RESTA)={}                           |S(RESTA)={$}
    P(DECIMAL)={0,1,2,3,4,5,6,7,8,9,' ',ϵ}|S(DECIMAL)={$}
*/
#include<stdio.h>
#include<stdlib.h>
char entrada [1000];
char* punteroEntrada;
void MATCH(int t);
void ELIMINAR_VACIO();
int OCTAL(int num);
int SUMA(int num);
int RESTA(int num);
int DECIMAL(int num);
void MATCH(int t)
{
    if(t==*punteroEntrada)
    {
        punteroEntrada=punteroEntrada+1;
    }
    else
    {
        printf("Error de sintaxis.\n");
        exit(EXIT_FAILURE);
    }
}
void ELIMINAR_VACIO()
{
    if(*punteroEntrada==' ')
    {
        punteroEntrada++;
    }
}
int OCTAL(int num)
{
    int x=num;
    if(*punteroEntrada=='0')
    {
        MATCH('0');
        x=OCTAL(num);
    }
    else if(*punteroEntrada=='1')
    {
        MATCH('1');
        x=OCTAL(num);
    }
    else if(*punteroEntrada=='2')
    {
        MATCH('2');
        x=OCTAL(num);
    }
    else if(*punteroEntrada=='3')
    {
        MATCH('3');
        x=OCTAL(num);
    }
    else if(*punteroEntrada=='4')
    {
        MATCH('4');
        x=OCTAL(num);
    }
    else if(*punteroEntrada=='5')
    {
        MATCH('5');
        x=OCTAL(num);
    }
    else if(*punteroEntrada=='6')
    {
        MATCH('6');
        x=OCTAL(num);
    }
    else if(*punteroEntrada=='7')
    {
        MATCH('7');
        x=OCTAL(num);
    }
    else if(*punteroEntrada=='8')
    {
        MATCH('8');
        x=RESTA(num);
    }
    else if(*punteroEntrada=='9')
    {
        MATCH('9');
        x=RESTA(num);
    }
    else if(*punteroEntrada==' ')
    {
        MATCH(' ');
        ELIMINAR_VACIO();
        x=SUMA(num);
    }
    else
    {

    }
    return x;
}
int SUMA(int num)
{
    int x=num;
    num=num+1;
    x=OCTAL(num);
    return x;
}
int RESTA(int num)
{
    int x=num;
    num=num-1;
    x=DECIMAL(num);
    return x;
}
int DECIMAL(int num)
{
    int x=num;
    if(*punteroEntrada=='0')
    {
        MATCH('0');
        x=DECIMAL(num);
    }
    else if(*punteroEntrada=='1')
    {
        MATCH('1');
        x=DECIMAL(num);
    }
    else if(*punteroEntrada=='2')
    {
        MATCH('2');
        x=DECIMAL(num);
    }
    else if(*punteroEntrada=='3')
    {
        MATCH('3');
        x=DECIMAL(num);
    }
    else if(*punteroEntrada=='4')
    {
        MATCH('4');
        x=DECIMAL(num);
    }
    else if(*punteroEntrada=='5')
    {
        MATCH('5');
        x=DECIMAL(num);
    }
    else if(*punteroEntrada=='6')
    {
        MATCH('6');
        x=DECIMAL(num);
    }
    else if(*punteroEntrada=='7')
    {
        MATCH('7');
        x=DECIMAL(num);
    }
    else if(*punteroEntrada=='8')
    {
        MATCH('8');
        x=DECIMAL(num);
    }
    else if(*punteroEntrada=='9')
    {
        MATCH('9');
        x=DECIMAL(num);
    }
    else if(*punteroEntrada==' ')
    {
        MATCH(' ');
        ELIMINAR_VACIO();
        x=SUMA(num);
    }
    else
    {

    }
    return x;
}
int main()
{
    int num=0;
    char caracter = getc(stdin);
    char finalEntrada = '\0';
    int posicionCaracter = 0;
    while(caracter != '\n')
    {
        entrada[posicionCaracter]=caracter;
        posicionCaracter++;
        entrada[posicionCaracter]=finalEntrada;
        caracter = getc(stdin);
        if( posicionCaracter >= 1000)
        {
            printf("Cadena de entrada supera el limite excedido.");
            exit(EXIT_FAILURE);
        }
    }
    entrada[posicionCaracter]=' ';
    posicionCaracter++;
    entrada[posicionCaracter]=caracter;
    punteroEntrada = entrada;
    num = OCTAL(num);
    printf("%d\n",num);
    return 0;
}
