首先，我的建議是把輸入輸出關係定義清楚，輸入行數產生菱形這件事情本身比較奇怪，建議定義為:`輸入菱形的大小，最小為1倍`，範例可以看下面。

## Sample input and output
* 1

```
 *
* *
 *
```

* 2

```
  *
 * *
*   *
 * *
  *
```

* 3

```
   *
  * *
 *   *
*     *
 *   *
  * *
   *
```

## How to solve it
第一件事情，先把需要輸出的空白變成其他文字，以方便觀察(此處以`O`代替空白)。
從輸出結果去思考，分成兩個部分，用兩個for loop去解決(最長的那行可以挑遞增或遞減部分則一輸出):
1. 遞增(上半)部分
2. 遞減(下半)部分

而兩個部分中，又各可以拆解成以下:
1. 輸出第一個星號左邊空白
2. 輸出第一個星號右邊空白

完成以上幾個區塊後，和在一起就完成了。

* 1

```
O*
*O*
O*
```

* 2

```
OO*
O*O*
*OOO*
O*O*
OO*
```

* 3

```
OOO*
OO*O*
O*OOO*
*OOOOO*
O*OOO*
OO*O*
OOO*
```

## Sample Code
```c
#include <stdio.h>
void main() {
   int a = 3;
   for(int i=0;i<=a;i++){
      for(int j=a-i;j>0;j--)
         printf(" ");
      printf("*");
      for(int j=0;j<i*2-1;j++)
         printf(" ");
      if(i!=0)
         printf("*");
      printf("\n");
   }

   for(int i=a-1;i>=0;i--){
      for(int j=a-i;j>0;j--)
         printf(" ");
      printf("*");

      for(int j=0;j<i*2-1;j++)
         printf(" ");
      if(i!=0)
         printf("*");

      printf("\n");
   }

}
```