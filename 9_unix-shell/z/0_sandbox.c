#include <stdio.h>
#include <string.h>

int main(){
  printf("\n");
  
  char text[] = "Hello";
  char text2[100];
  
  strcpy( text , text2 );
  printf("text2 : %s\n" , text2);
  
  printf("\n");
}
