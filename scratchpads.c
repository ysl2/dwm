#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char **injectNumbersIntoStringArray(char *strArr[], int number1, int number2) {
    int currentIndex = 0;
    int count = 0;

    // 计算字符串数组的长度
    while (strArr[currentIndex] != NULL) {
        count++;
        currentIndex++;
    }

    // 分配足够的内存来存储结果字符串数组
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    currentIndex = 0;

    // 将数字注入到字符串中
    while (strArr[currentIndex] != NULL) {
        int length = snprintf(NULL, 0, strArr[currentIndex], number1, number2);
        result[currentIndex] = (char *)malloc(length + 1);
        snprintf(result[currentIndex], length + 1, strArr[currentIndex], number1, number2);
        currentIndex++;
    }

    result[count] = NULL;  // 结束字符串数组

    return (const char**)result;
}

// void freeStringArray(char **strArr) {
//     if (strArr == NULL)
//         return;
//
//     int currentIndex = 0;
//     while (strArr[currentIndex] != NULL) {
//         free(strArr[currentIndex]);
//         currentIndex++;
//     }
//
//     free(strArr);
// }
//
// int main() {
//     const char *spcmd1[] = {
//         "alacritty",
//         "--class",
//         "spterm",
//         "-o",
//         "window.dimensions.lines=%d",
//         "-o",
//         "window.dimensions.columns=%d",
//         NULL
//     };
//
//     int number1 = 100;
//     int number2 = 200;
//
//     char **result = injectNumbersIntoStringArray(spcmd1, number1, number2);
//     int currentIndex = 0;
//
//     // 打印结果字符串数组
//     while (result[currentIndex] != NULL) {
//         printf("Result %d: %s\n", currentIndex, result[currentIndex]);
//         currentIndex++;
//     }
//
//     freeStringArray(result);
//
//     return 0;
// }
//
