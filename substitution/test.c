#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


int main(void)
{
  // string key1 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  // string key2 = "JTREKYAVOGDXPSNCUIZLFBMWHQ";
  string key1 = "ABC";
  string key2 = "AAB";

  for (int i = 0; i < 3; i++)
  {
    for (int j = 1; j < 3; j++)
    {
      char n = key1[i];
      char m = key1[j];
      if(key1[i] == key1[j])
      {
        printf("There are duplicates\n");
        break;
      }
    }
    break;
  }
}