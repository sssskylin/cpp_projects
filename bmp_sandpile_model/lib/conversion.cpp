#include <iostream>
#include "conversion.h"

int StringLength(char* str){
    int count = 0;
    while (str[count] != '\0'){
        ++count;
    }
    return count;
}

uint64_t CharToUint64_t(char* a){
  uint64_t res = 0;
  while (*a != '\0'){
    res = res*10 + (*a - '0');
    ++a;
  }
  return res;
}
char* AddChar(char* a, char* b){
    int lenA = StringLength(a);
    int lenB = StringLength(b);
    int len = lenA + lenB + 1;
    char* c = new char[len];
    for (int i = 0; i < lenA; ++i) {
        c[i] = a[i];
    }
    for (int i = 0; i < lenB; ++i) {
        c[lenA + i] = b[i]; 
    }

    c[lenA + lenB] = '\0';
    
    return c;
}

uint64_t NumberLength(uint64_t a){
  uint64_t count = 0;
  while (a != 0){
    a = a/10;
    ++count;
    }
    return count;
}

char* Uint64_tToChar(uint64_t a){
  char* res = new char[NumberLength(a)];
  for  (int i = NumberLength(a) - 1; i >= 0; --i) {
    res[i] = (a % 10) + '0';
    a = a/10;
    }
  return res;
}