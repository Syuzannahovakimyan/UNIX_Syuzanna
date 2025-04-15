 #include <stdio.h>
 #include <unistd.h>
 
 int main(){
     int length = 9 ;
     int width = 3;
     /*printf("input rectangle params\n");*/
     /**/
     /*scanf("rectangle length = %d\n", &length);*/
     /*scanf("rectangle width = %d\n", &width);*/
 
     int S = length * width;
     printf("S of rectangle: %d\n", S);
 
     return S;
 }
