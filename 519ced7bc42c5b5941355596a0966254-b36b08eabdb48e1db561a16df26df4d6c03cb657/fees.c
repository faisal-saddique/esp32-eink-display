// A C/C++ program for splitting a string
// using strtok()
#include <stdio.h>
#include <string.h>

int main()
{
    // unsigned char some[10] = "\x84\x43\x85\x67\x88\x22\x38\x58\x64";
    
    // unsigned char somet[10] = {0x84,0x43,0x85,0x67,0x88,0x22,0x38,0x58,0x64};
    // // const unsigned char gImage_4in01f[15] = {0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x84, 0x36, 0x36, 0x36, 0x36};
    // unsigned char a[6] = "\x20\x20\x20\x20\x20";

    // unsigned char b[6] = {0x20,0x20,0x20,0x20,0x20};
    // // printf(STR(a)": ");

    // // for (int i = 0; i < 16; ++i) {
    // //     printf("%x, ", b[i]);
    // // }

    // // for (int i = 0; i < sizeof(somet); i++) {
    // //     printf("%x, ", somet[i]);
    // // }
    // // char c[] = "ab";
    // // strcat(c, "d");
    // // printf(c);

    // printf("\b\b\n");
    // // printf("Value at third location a = %s \n",a[3]);
    // // printf("Value at third location b = %s \n",b[3]);
    
    // printf("Length of string a = %zu \n",strlen(some));
    // printf("Length of string b = %zu \n",strlen(somet));

    // if (strcmp(some,somet)== 0)
    //     printf("Both are equal");
    //     else
    //         printf("Not equal");
        // printf("%x",gImage_4in01f[0]);
    // unsigned char gImage_4in01f[10];
    // char hex[] = "6A";                          // here is the hex string
    // int num = (int)strtol(hex, NULL, 16);       // number base 16
    // printf("%c\n", num);                        // print it as a char
    // printf("%d\n", num);                        // print it as decimal
    // printf("%X\n", num);                        // print it back as hex

    // unsigned char a[16] = {' ', 0x41, 0x42, };
    // unsigned char b[16] = {"0x20"};

    // printf("Hex value is: 0x%x", a[0]);
    // char data[] = "4,0,0,0,1,0,0,0,16,0,0,0,100,58,82,80,162,77,200,183,178,32";
    // char str[] = "0x88,0x88,0x88,0x88,0x88,0x88,0x84,0x44,";
    // unsigned char save[8];
    // int rep = 0;
    // // Returns first token
    // char* token = strtok(str, ",");
 
    // // Keep printing tokens while one of the
    // // delimiters present in str[].
    // while (token != NULL) {
    //     printf("%s\n", token);
    //     save[rep] = token;
    //     token = strtok(NULL, ",");
    //     rep++;
    // }

    // for (int i = 0; i < sizeof(save);i++)
    // {
    //     printf("%s\n", save[i]);
    // }
    //     return 0;

    
}

// #include <stdio.h>
// #include <math.h>

// // int mult_return(int first, int second);
// // void mult(int first, int second);
// void table();

// void main()
// {
    
//     int a = 99;
//     // int b = 6;
//     // int c = mult_return(a, b);
//     // printf("The multiplication of %d and %d is: %d\n", a, b, c);
//     printf("The log of %d is: %f\n", a, log(a));
//     // int c = 7;
//     // int d = 9;
//     // mult(c, d);

//     // table(2);
// }


// int mult_return(int first, int second)
// {
//     int ans = first * second;
//     return ans;
//     // printf("The multiplication of %d and %d is: %d\n", first, second, ans);
// }

// void mult(int first, int second)
// {
//     int ans = first * second;
//     printf("The multiplication of %d and %d is: %d\n", first, second, ans);
// }


// void table (int num)
// {
//     for (int i=1;i<=10;i++)
//     {
//         printf("%d x %d = %d\n", num, i, num*i);
//     }
// }