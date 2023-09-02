#include <stdio.h>

int main() {
  printf("\n");

  int num = 42;
  // Printing with indentation
  printf("Indented output:\n");
  printf("%1s: %d\n", "Number", num); // Use %10s to specify a width of 10 characters for the label
  
  printf("\n");
  return 0;
}
