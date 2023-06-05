/*
    BNF resultante:
    DECIMAL -> _ INCREMENTO | 0 DECIMAL | 1 DECIMAL | 2 DECIMAL | 3 DECIMAL | 4 DECIMAL | 5 DECIMAL | 6 DECIMAL | 7 DECIMAL | 8 C | 9 C | ϵ
    INCREMENTO -> + DECIMAL
    C -> - D
    D -> _ INCREMENTO | 0 D | 1 D | 2 D | 3 D | 4 D | 5 D | 6 D | 7 D | 8 D | 9 D | ϵ
    P(DECIMAL)={0,1,2,3,4,5,6,7,8,9,_,ϵ}              S(DECIMAL)={$}
    P(INCREMENTO)={+}                                    S(INCREMENTO)={$}
    P(C)={-}                                    S(C)={$}
    P(D)={0,1,2,3,4,5,6,7,8,9,_,ϵ}              S(D)={$}
*/
#include<stdio.h>
