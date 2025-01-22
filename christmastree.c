/*
題目要求：

請撰寫一程式以輸出字串的方式繪出聖誕樹，層與層之間沒有空白行，樹葉及樹幹部分都是由 '' 代表，背景由 '-' (減號)代表，樹幹位於樹的中央。該程式會讀取使用者輸入的一個 3 到 13 的奇數，並檢查輸入數值是否合乎規定，若不合乎則要求重新輸入。該輸入數字代表三角形部分的高度(H)，樹幹部分的長度(L)等於(H/2)的整數。樹的寬度(W)等於(H2-1)，單位為字元。
該程式可以讓使用者一次輸入 3 棵樹的高度
當程式檢查到使用者所輸入值的其中一個值為非數值或非 3~13 的奇數時，程式會要求使用者全部重新輸入
程式必須並列印出使用者輸入的三個高度的聖誕樹樹，樹與樹之間需有空格

範例輸入 : 5 3 7
範例輸出 :
----*---- --*-- ------*------
---***--- -***- -----***-----
--*****-- ***** ----*****----
-*******- --*-- ---*******---
*********       --*********--
----*----       -***********-
----*----       *************
                ------*------
                ------*------
                ------*------
*/

#include <stdio.h>
#include <stdlib.h>

void main(){
    int mytree[3];
    int max = 0, max_height = 0, max_width = 0;

    printf("請輸入三顆聖誕葉的高度(介於3~13的奇數並用空格隔開) : ");
    scanf("%d%d%d", &mytree[0], &mytree[1], &mytree[2]);
    
    // find max
    max = mytree[0];
    for(int i=1;i<3;i++){
        if(mytree[i] > max)
            max = mytree[i];
    }
    max_height = max + max/2;

    for(int i=0;i<max_height;i++){
        for(int tree=0;tree<3;tree++){
            if(i < mytree[tree]){
                for(int j=mytree[tree]-1;j>i;j--)
                    printf("-");
                for(int j=0;j<2*i+1;j++)
                    printf("*");
                for(int j=mytree[tree]-1;j>i;j--)
                    printf("-");
            }
            else if(i < mytree[tree] + mytree[tree]/2){
                for(int j=1;j<mytree[tree];j++)
                    printf("-");
                printf("*");
                for(int j=1;j<mytree[tree];j++)
                    printf("-");
            }
            else
                for(int j=0;j<2*mytree[tree]-1;j++)
                    printf(" ");
            printf(" ");
        }
        printf("\n");
    }
}