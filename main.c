#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <dirent.h>


char input[1024];
char text[1024];
char clipBoard[1000000];
char currentPath[4096] = "C:\\root";
char startingPosition[4096];
char result[1024];
//for strings without * at the end:
char nextSubstring[1024];


int countAndDelete(char *sText)
{
    int length = (int)strlen(sText);
    int i = 0;
    for (; i < length; ++i)
    {
        if(sText[i] == ' ' || sText[i] == '\n' || sText[i] == EOF)
        {
            for (int j = 0; j < length - i + 1; ++j)
            {
                sText[j] = sText[j + i + 1];
            }
            return i + 1;
        }
    }
    sText[0] = '\0';
    return length;
}


int deleteFirst(char *sText)
{
    sText[0] = ' ';
    return countAndDelete(sText);
}


char *substring(int index, char *stringToChange)
{
    memset(result, 0, 1024);

    int length = (int)strlen(stringToChange) - index + 1;
    for (int i = 0; i < length; ++i) {
        result[i] = stringToChange[i + index];
    }
    return result;
}


int counter = 0;
int indexToReach = 1;



void backToStartingPoint()
{
    _chdir("C:\\root");
}


int wizard(char *checkingString, char *mainString)
{
    //If checking string exists at the beginning of the input deletes it and returns 0
    //1 means there was not such part in the input given
    int size = (int)strlen(checkingString);
    int inputSize = (int)strlen(mainString);
    if(inputSize < size)
        return 1;
    for (int i = 0; i < size; ++i)
    {
        if(checkingString[i] != mainString[i])
            return 1;
    }
    for (int i = 0; i < inputSize - size; ++i)
    {
        mainString[i] = mainString[i + size];
    }
    mainString[inputSize - size] = '\0';
    return 0;
}


int barber(char *changingText)
{
    int validFormat = 1;
    int flag_BS = 0;
//    char result[1024];
    int textPrimalLength = (int)strlen(changingText);
    int textIndex = 0;
    for (int i = 0; i < textPrimalLength; ++i)
    {
        if(changingText[i] == '\\' && !flag_BS)
        {
            i++;
            switch (changingText[i])
            {
                case 'n':
                    changingText[textIndex] = '\n';
                    break;
                case '?':
                    changingText[textIndex] = '\?';
                    break;
                case 'a':
                    changingText[textIndex] = '\a';
                    break;
                case 'b':
                    changingText[textIndex] = '\b';
                    break;
                case 'f':
                    changingText[textIndex] = '\f';
                    break;
                case 'r':
                    changingText[textIndex] = '\r';
                    break;
                case 't':
                    changingText[textIndex] = '\t';
                    break;
                case 'v':
                    changingText[textIndex] = '\v';
                    break;
                case '\\':
                    changingText[textIndex] = '\\';
                    flag_BS = 1;
                    break;
                case '\'':
                    changingText[textIndex] = '\'';
                    break;
                case '\"':
                    changingText[textIndex] = '\"';
                    break;
                default:
                    changingText[textIndex] = '\\';
                    i--;
                    validFormat = 0;
                    flag_BS = 1;
                    break;
            }
        }
        else
        {
            flag_BS = changingText[i] == '\\';
            changingText[textIndex] = changingText[i];
        }
        textIndex++;
    }
    changingText[textIndex] = '\0';
    return !validFormat;
}

int calculatedLine, calculatedIndex;
int positionFinder(int charIndex, char *path1)
{
    char path[1024];
    strcpy(path, path1);
    char part[1024];
    if(wizard("/root/", path))
    {
        return 1;
    }
    int z = 0;
    for (int i = 0; ; ++i, z++)
    {
        char currentChar = path[z];
        if(currentChar == '/')
        {
            part[i] = '\0';
            if(0 - _chdir(part))
            {
                backToStartingPoint();
                return 1;
            }
            strcat(currentPath, "\\");
            strcat(currentPath, part);
            i = -1;
        }
        else if(currentChar == '\0')
        {
            part[i] = '\0';
            FILE *fp = fopen(part, "r");
            if(fp == NULL)
            {
                backToStartingPoint();
                return 1;
            }
            else
            {
                calculatedLine = 1;
                calculatedIndex = 0;
                for (int j = 0; j < charIndex; ++j) {
                    char c;
                    c = (char)fgetc(fp);
                    if(c == '\n')
                    {
                        calculatedLine++;
                        calculatedIndex = 0;
                    }
                    else
                        calculatedIndex++;
                }
            }
            fclose(fp);
            return 0;
        }
        else
        {
            part[i] = currentChar;
        }
    }

}


int extractString()
{
    char temp[1024];
    int tempIndex = 0;
    if(input[0] == '\"')
    {
        wizard("\"", input);
        int a = 0;
        for (int i = 0; ; ++i)
        {
            if(input[i] == '\n')
                return 1;
            if(input[i] == '\"' && (i == 0 || input[i - 1] != '\\'))
            {
                text[i - a] = '\0';
                temp[tempIndex] = '\0';
                break;
            }
            if(input[i] == '\"' && input[i - 1] == '\\')
            {
                a++;
                text[i - a] = '\"';
            }
            text[i - a] = input[i];
            temp[tempIndex] = input[i];
            tempIndex++;
        }
        wizard(temp, input);
        wizard("\"", input);
        return 0;
    }
    else
    {
        for (int i = 0; ; ++i)
        {
            if(input[i] == ' ' || input[i] == '\n')
            {
                text[i] = '\0';
                break;
            }
            text[i] = input[i];
        }
        wizard(text, input);
        return 0;
    }
}


int createFile(char *path)
{
    if(wizard("/root/", path))
    {
        return -1;
    }
    int size = (int )strlen(path);
    for (int i = 0; i < size; ++i)
    {
        if(path[i] == '"' || path[i] == '<' || path[i] == '?' || path[i] == '*' || path[i] == ':' || path[i] == '|' || path[i] == '\\' || path[i] == '>')
        {
            return -2;
        }
    }
    char part[1024];
    int z = 0;
    for (int i = 0; ; ++i, z++) {
        char currentChar = path[z];
        if(currentChar == '/')
        {
            part[i] = '\0';
            mkdir(part);
            strcat(currentPath, "\\");
            strcat(currentPath, part);
            _chdir(currentPath);
            i = -1;
        }
        else if(currentChar == '\0')
        {
            part[i] = '\0';
            FILE *fp = fopen(part, "r");
            if(fp == NULL)
            {
                fp = fopen(part, "w");
                fclose(fp);
                backToStartingPoint();
                return 0;
            }
            else
            {
                fclose(fp);
                backToStartingPoint();
                return -3;
            }
        }
        else
        {
            part[i] = currentChar;
        }
    }
}


int numExtract()
{
    int finalSign = 1;
    int res = 0;
    char sample = input[0];
    if(sample == '-')
    {
        finalSign = -1;
        wizard("-", input);
    }
    while (1)
    {
        if(input[0] >= '0' && input[0] <= '9')
        {
            res = res * 10 + (int)(input[0] - '0');
            char c[2];
            c[0] = input[0];
            c[1] = '\0';
            wizard(c, input);
        }
        else
            return res * finalSign;
    }
}


int cat(char *path)
{
    char part[1024];
    if(wizard("/root/", path))
    {
        printf("Invalid path!\n");
        return 1;
    }
    int z = 0;
    for (int i = 0; ; ++i, z++)
    {
        char currentChar = path[z];
        if(currentChar == '/')
        {
            part[i] = '\0';
            if(0 - _chdir(part))
            {
                printf("Invalid file or directory!\n");
                backToStartingPoint();
                return 1;
            }
            strcat(currentPath, "\\");
            strcat(currentPath, part);
            i = -1;
        }
        else if(currentChar == '\0')
        {
            part[i] = '\0';
            FILE *fp = fopen(part, "r");
            if(fp == NULL)
            {
                printf("Invalid file or directory!\n");
                backToStartingPoint();
                return 1;
            }
            else
            {
                int n = (int)strlen(part);
                for (int j = n + 4; j < 50; ++j)
                {
                    printf("-");
                }
                printf(" %s --", part);
                printf("\n");
                while (1)
                {
                    char lines[1024];
                    lines[0] = '\0';
                    fgets(lines, 1024, fp);
                    if(lines[0] == '\0')
                        break;
                    printf("%s", lines);
                }
                printf("\n--------------------------------------------------\n");//50-
                fclose(fp);
                backToStartingPoint();
                return 0;
            }
        }
        else
        {
            part[i] = currentChar;
        }
    }
}


int catLine(char *path1, int line)
{
    char path[1024];
    strcpy(path, path1);
    char part[1024];
    if(wizard("/root/", path))
    {
        printf("Invalid path!\n");
        return 1;
    }
    int z = 0;
    for (int i = 0; ; ++i, z++)
    {
        char currentChar = path[z];
        if(currentChar == '/')
        {
            part[i] = '\0';
            if(0 - _chdir(part))
            {
                printf("Invalid file or directory!\n");
                backToStartingPoint();
                return 1;
            }
            strcat(currentPath, "\\");
            strcat(currentPath, part);
            i = -1;
        }
        else if(currentChar == '\0')
        {
            part[i] = '\0';
            FILE *fp = fopen(part, "r");
            if(fp == NULL)
            {
                printf("Invalid file or directory!\n");
                backToStartingPoint();
                return 1;
            }
            else
            {
                char sth[1024];
                for (int j = 0; j < line; ++j) {
                    fgets(sth, 1024, fp);
                }
                printf("%s", sth);
                if(sth[strlen(sth) - 1] != '\n')
                    printf("\n");
                fclose(fp);
                backToStartingPoint();
                return 0;
            }
        }
        else
        {
            part[i] = currentChar;
        }
    }
}


int copyForward(char *path1, int line, int charIndex, int length)
{
    char path[1024];
    strcpy(path, path1);
    char part[1024];
    if(wizard("/root/", path))
    {
        printf("Invalid path!\n");
        return 1;
    }
    int z = 0;
    for (int i = 0; ; ++i, z++) {
        char currentChar = path[z];
        if(currentChar == '/')
        {
            part[i] = '\0';
            if(0 - _chdir(part))
            {
                printf("Invalid file or directory!\n");
                backToStartingPoint();
                return 1;
            }
            strcat(currentPath, "\\");
            strcat(currentPath, part);
            i = -1;
        }
        else if(currentChar == '\0')
        {
            part[i] = '\0';
            FILE *fp = fopen(part, "r");
            if(fp == NULL)
            {
                printf("Invalid file or directory!\n");
                backToStartingPoint();
                return 1;
            }
            else
            {
                for (int j = 1; j < line; ++j)
                {
                    char lines[1024];
                    lines[0] = '\0';
                    fgets(lines, 1024, fp);
                    if(lines[0] == '\0')
                    {
                        printf("Out of bounds!\n");
                        fclose(fp);
                        remove("temp.txt");
                        backToStartingPoint();
                        return 1;
                    }
                }
                fgets(input, 1024, fp);
                char c;
                for (int j = 0; j < charIndex; ++j)
                {
                    c = input[0];
                    if(c == '\n' || c == '\0')
                    {
                        printf("Out of bounds!\n");
                        fclose(fp);
                        remove("temp.txt");
                        backToStartingPoint();
                        return 1;
                    }
                    char tmp[2];
                    tmp[0] = c;
                    tmp[1] = '\0';
                    wizard(tmp, input);
                }
                memset(clipBoard, 0, 1000000);
                for (int j = 0; j < length; ++j)
                {
//                    printf("<%c>", input[0]);
                    if(input[0] == '\0')
                    {
                        fgets(input, 1024, fp);
                        if(input[0] == '\0')
                        {
                            printf("Out of bounds!\n");
                            fclose(fp);
                            remove("temp.txt");
                            backToStartingPoint();
                            return 1;
                        }
                    }
                    char tmp[2];
                    tmp[0] = input[0];
                    tmp[1] = '\0';
                    clipBoard[j] = tmp[0];
                    wizard(tmp, input);

                }
                input[0] = '\0';
                fclose(fp);
                backToStartingPoint();
                printf("Success!\n");
                return 0;
            }
        }
        else
        {
            part[i] = currentChar;
        }
    }
}


int copyBackward(char *path, int line, int charIndex, int length)
{
    char pathCopy[1024];
    strcpy(pathCopy, path);
    char part[1024];
    if(wizard("/root/", path))
    {
        printf("Invalid path!\n");
        return 1;
    }
    int z = 0;
    for (int i = 0; ; ++i, z++)
    {
        char currentChar = path[z];
        if(currentChar == '/')
        {
            part[i] = '\0';
            if(0 - _chdir(part))
            {
                printf("Invalid file or directory!\n");
                backToStartingPoint();
                return 1;
            }
            strcat(currentPath, "\\");
            strcat(currentPath, part);
            i = -1;
        }
        else if(currentChar == '\0')
        {
            part[i] = '\0';
            FILE *fp = fopen(part, "r");
            if(fp == NULL)
            {
                printf("Invalid file or directory!\n");
                backToStartingPoint();
                return 1;
            }
            else
            {
                int sizes[line + 1];
                for (int j = 1; j < line; ++j)
                {
                    char lines[1024];
                    lines[0] = '\0';
                    fgets(lines, 1024, fp);
                    if(lines[0] == '\0')
                    {
                        printf("Out of bounds!\n");
                        fclose(fp);
                        backToStartingPoint();
                        return 1;
                    }
                    sizes[j] = (int) strlen(lines);
                }
                sizes[line] = charIndex;
                fclose(fp);
                backToStartingPoint();
                int templen = length;
                for (int j = line; j > 0; --j)
                {
                    if(sizes[j] >= templen)
                    {
                        return copyForward(pathCopy, j, sizes[j] - templen, length);
                    }
                    else
                    {
                        templen -= sizes[j];
                    }
                }
                printf("Out of bounds!\n");
                return 1;
            }
        }
        else
        {
            part[i] = currentChar;
        }
    }
}


void insertStr(char *path1, char *content, int line, int charIndex)
{
    char path[1024];
    strcpy(path, path1);
    char part[1024];
    if(wizard("/root/", path))
    {
        printf("Invalid path!\n");
        return;
    }
    int z = 0;
    for (int i = 0; ; ++i, z++) {
        char currentChar = path[z];
        if(currentChar == '/')
        {
            part[i] = '\0';
            if(0 - _chdir(part))
            {
                printf("Invalid file or directory!\n");
                backToStartingPoint();
                return;
            }
            strcat(currentPath, "\\");
            strcat(currentPath, part);
            i = -1;
        }
        else if(currentChar == '\0')
        {
            part[i] = '\0';
            FILE *fp = fopen(part, "r");
            if(fp == NULL)
            {
                printf("Invalid file or directory!\n");
                backToStartingPoint();
                return;
            }
            else
            {
                FILE *newOne = fopen("temp.txt", "w");
                for (int j = 1; j < line; ++j)
                {
                    char lines[1024];
                    lines[0] = '\0';
                    fgets(lines, 1024, fp);
                    if(lines[0] == '\0')
                    {
                        printf("Out of bounds!\n");
                        fclose(newOne);
                        fclose(fp);
                        remove("temp.txt");
                        backToStartingPoint();
                        return;
                    }
                    fputs(lines, newOne);
                }
                fgets(input, 1024, fp);
                char c;
                for (int j = 0; j < charIndex; ++j)
                {
                    c = input[0];
                    if(c == '\n' || c == '\0')
                    {
                        printf("Out of bounds!\n");
                        fclose(newOne);
                        fclose(fp);
                        remove("temp.txt");
                        backToStartingPoint();
                        return;
                    }
                    fputc(c, newOne);
                    char tmp[2];
                    tmp[0] = c;
                    tmp[1] = '\0';
                    wizard(tmp, input);
                }
                fputs(content, newOne);
                fputs(input, newOne);
                input[0] = '\0';
                while (1)
                {
                    fgets(input, 1024, fp);
                    fputs(input, newOne);
                    if(input[0] == '\0')
                        break;
                    input[0] = '\0';
                }
                fclose(fp);
                fclose(newOne);
                remove(part);
                rename("temp.txt", part);
                backToStartingPoint();
                printf("Success!\n");
                return;
            }
        }
        else
        {
            part[i] = currentChar;
        }
    }
//    FILE *newFile = fopen("tempFile.txt", "w");
//    char c;
//    int zIndex = 0;
//    for (int i = 1; i < line; zIndex++)
//    {
//
//        fscanf(path, "%c", &c);
//    }
//    remove(path);
//    rename("tempFile.txt", path);
}


int removeForward(char *path1, int line, int charIndex, int length)
{
    char path[1024];
    strcpy(path, path1);
    char part[1024];
    if(wizard("/root/", path))
    {
        return 1;
    }
    int z = 0;
    for (int i = 0; ; ++i, z++) {
        char currentChar = path[z];
        if(currentChar == '/')
        {
            part[i] = '\0';
            if(0 - _chdir(part))
            {
                backToStartingPoint();
                return 1;
            }
            strcat(currentPath, "\\");
            strcat(currentPath, part);
            i = -1;
        }
        else if(currentChar == '\0')
        {
            part[i] = '\0';
            FILE *fp = fopen(part, "r");
            if(fp == NULL)
            {
                backToStartingPoint();
                return 1;
            }
            else
            {
                FILE *newOne = fopen("temp.txt", "w");
                for (int j = 1; j < line; ++j)
                {
                    char lines[1024];
                    lines[0] = '\0';
                    fgets(lines, 1024, fp);
                    if(lines[0] == '\0')
                    {
                        fclose(newOne);
                        fclose(fp);
                        remove("temp.txt");
                        backToStartingPoint();
                        return 2;
                    }
                    fputs(lines, newOne);
                }
                fgets(input, 1024, fp);
                char c;
                for (int j = 0; j < charIndex; ++j)
                {
                    c = input[0];
                    if(c == '\n' || c == '\0')
                    {
                        fclose(newOne);
                        fclose(fp);
                        remove("temp.txt");
                        backToStartingPoint();
                        return 2;
                    }
                    fputc(c, newOne);
                    char tmp[2];
                    tmp[0] = c;
                    tmp[1] = '\0';
                    wizard(tmp, input);
                }
                for (int j = 0; j < length; ++j)
                {
//                    printf("<%c>", input[0]);
                    if(input[0] == '\0')
                    {
                        fgets(input, 1024, fp);
                        if(input[0] == '\0')
                        {
                            fclose(newOne);
                            fclose(fp);
                            remove("temp.txt");
                            backToStartingPoint();
                            return 2;
                        }
                    }
                    char tmp[2];
                    tmp[0] = input[0];
                    tmp[1] = '\0';
                    wizard(tmp, input);
                }
                fputs(input, newOne);
                input[0] = '\0';
                while (1)
                {
                    fgets(input, 1024, fp);
                    fputs(input, newOne);
                    if(input[0] == '\0')
                        break;
                    input[0] = '\0';
                }
                fclose(fp);
                fclose(newOne);
                remove(part);

                rename("temp.txt", part);
                backToStartingPoint();
                return 0;
            }
        }
        else
        {
            part[i] = currentChar;
        }
    }
}


int removeBackward(char *path, int line, int charIndex, int length)
{
    char pathCopy[1024];
    strcpy(pathCopy, path);
    char part[1024];
    if(wizard("/root/", path))
    {
        return 1;
    }
    int z = 0;
    for (int i = 0; ; ++i, z++)
    {
        char currentChar = path[z];
        if(currentChar == '/')
        {
            part[i] = '\0';
            if(0 - _chdir(part))
            {
                backToStartingPoint();
                return 1;
            }
            strcat(currentPath, "\\");
            strcat(currentPath, part);
            i = -1;
        }
        else if(currentChar == '\0')
        {
            part[i] = '\0';
            FILE *fp = fopen(part, "r");
            if(fp == NULL)
            {
                backToStartingPoint();
                return 1;
            }
            else
            {
                int sizes[line + 1];
                for (int j = 1; j < line; ++j)
                {
                    char lines[1024];
                    lines[0] = '\0';
                    fgets(lines, 1024, fp);
                    if(lines[0] == '\0')
                    {
                        fclose(fp);
                        backToStartingPoint();
                        return 2;
                    }
                    sizes[j] = (int) strlen(lines);
                }
                sizes[line] = charIndex;
                fclose(fp);
                backToStartingPoint();
                int templen = length;
                for (int j = line; j > 0; --j)
                {
                    if(sizes[j] >= templen)
                    {
                        return removeForward(pathCopy, j, sizes[j] - templen, length);
                    }
                    else
                    {
                        templen -= sizes[j];
                    }
                }
                return 2;
            }
        }
        else
        {
            part[i] = currentChar;
        }
    }
}


int superWizard(char *first, char *second2)
{
    char second[1024];
    strcpy(second, second2);
//    if((int)strlen(second) > 0 && (second[(int)strlen(second) - 1] == ' ' || second[(int)strlen(second) - 1] == '\n'))
//        for (int i = (int)strlen(second) - 1; i >= 0; --i)
//        {
//            if(second[i] == ' ' || second[i] == '\n')
//                second[i] = '\0';
//            else
//                break;
//        }

    if (!strlen(first))
    {
//        printf("counter: %d, ind: %d\n", counter, indexToReach);
        strcpy(nextSubstring, second);
        return ++counter == indexToReach;
    }

    if (strlen(first) == 0 && (second[0] == '\0' || second[0] == '\n' || second[0] == ' '))
    {
//        printf("counter: %d, ind: %d\n", counter, indexToReach);
        return ++counter == indexToReach;
    }


    if (strlen(first) > 1 && first[0] == '*') {
        int i = 0;
        while (i + 1 < strlen(first) && first[i + 1] == '*')
            i++;
        strcpy(first, substring(i, first));
    }

    if (strlen(first) > 1 && first[0] == '*'
        && strlen(second) == 0)
        return 0;

    if (strlen(first) > 1 && first[0] == '\\' && first[1] == '*')
    {
        if(second[0] == '*')
        {
            char firstSub[1024];
            strcpy(firstSub, substring(2, first));
            char secondSub[1024];
            strcpy(secondSub, substring(1, second));
            return superWizard(firstSub, secondSub);
        }
        else
            return 0;
    }


    if (strlen(first) > 0 && first[0] == '*')
    {
        char firstSub[1024];
        strcpy(firstSub, substring(1, first));
        if(second[0] == ' ' || second[0] == '\0')
            return superWizard(firstSub, second);
        char secondSub[1024];
        strcpy(secondSub, substring(1, second));
        return (superWizard(firstSub, second) || superWizard(first, secondSub));
    }


    if (strlen(first) != 0 && strlen(second) != 0 && first[0] == second[0])
    {
        char firstSub[1024];
        strcpy(firstSub, substring(1, first));
        char secondSub[1024];
        strcpy(secondSub, substring(1, second));
        return superWizard(firstSub, secondSub);
    }
    return 0;
}


int findAt(int indexToFind, char *first, char *path1)
{
    char path[1024];
    strcpy(path, path1);
    indexToReach = indexToFind;
    counter = 0;
    int index = 0;
    int flag = 1;
    char second[1024];
    char part[1024];
    if(wizard("/root/", path))
    {
        printf("Invalid path!\n");
        return -2;
    }
    int z = 0;
    for (int i = 0; ; ++i, z++) {
        char currentChar = path[z];
        if(currentChar == '/')
        {
            part[i] = '\0';
            if(0 - _chdir(part))
            {
                printf("Invalid file or directory!\n");
                backToStartingPoint();
                return -2;
            }
            strcat(currentPath, "\\");
            strcat(currentPath, part);
            i = -1;
        }
        else if(currentChar == '\0')
        {
            part[i] = '\0';
            FILE *fp = fopen(part, "r");
            if(fp == NULL)
            {
                printf("Invalid file or directory!\n");
                backToStartingPoint();
                return -2;
            }
            else
            {
                int finalResult = -1;
                int flag = first[0] == '*';
                fgets(second, 1024, fp);
                while (1)
                {
                    if (superWizard(first, second)) {
                        finalResult = index;
                        flag = 0;
                        break;
                    }
                    if(flag)
                        index += countAndDelete(second);
                    else
                        index += deleteFirst(second);
                    if (second[0] == '\0')
                    {
                        fgets(second, 1024, fp);
                        if(second[0] == '\0')
                            break;
                    }
                }
                fclose(fp);
                backToStartingPoint();
                return finalResult;
            }
        }
        else
        {
            part[i] = currentChar;
        }
    }
}


int findAtByWord(int indexToFind, char *first, char *path1)
{
    char path[1024];
    strcpy(path, path1);
    indexToReach = indexToFind;
    counter = 0;
    int index = 0;
    int flag = 1;
    char second[1024];
    char part[1024];
    if(wizard("/root/", path))
    {
        printf("Invalid path!\n");
        return 1;
    }
    int z = 0;
    for (int i = 0; ; ++i, z++) {
        char currentChar = path[z];
        if(currentChar == '/')
        {
            part[i] = '\0';
            if(0 - _chdir(part))
            {
                printf("Invalid file or directory!\n");
                backToStartingPoint();
                return 1;
            }
            strcat(currentPath, "\\");
            strcat(currentPath, part);
            i = -1;
        }
        else if(currentChar == '\0')
        {
            part[i] = '\0';
            FILE *fp = fopen(part, "r");
            if(fp == NULL)
            {
                printf("Invalid file or directory!\n");
                backToStartingPoint();
                return 1;
            }
            else
            {
                int finalResult = -1;
                fgets(second, 1024, fp);
                while (1)
                {
                    if (superWizard(first, second)) {
                        finalResult = index;
                        flag = 0;
                        break;
                    }
                    index += second[0] != ' ' && second[0] != '\n' && second[0] != '\0';
                    countAndDelete(second);
                    if (second[0] == '\0')
                    {
                        fgets(second, 1024, fp);
                        if(second[0] == '\0')
                            break;
                    }
                }
                fclose(fp);
                backToStartingPoint();
                return finalResult;
            }
        }
        else
        {
            part[i] = currentChar;
        }
    }
}


int replace(char *from, char *to, int index, char *path)
{
    int b = findAt(index, from, path);
    if(b == -2)
        return -2;
    if(b == -1)
    {
        printf("Out of bounds!\n");
        return 1;
    }
    if(from[strlen(from) - 1] == '*')
    {
        int start = b;
        int finalIndex;
        int a;
        for (int i = 1; ; ++i) {
            finalIndex = findAt(i, " ", path);
            if(finalIndex >= start || finalIndex == -1)
                break;
        }
        for (int i = 1; ; ++i) {
            a = findAt(i, "\n", path);
            if(a >= start || a == -1)
                break;
        }
        if((a < finalIndex || finalIndex == -1) && a != -1)
            finalIndex = a;
        if(finalIndex == -1)
            finalIndex = findAt(1, "\0", path);
        int length = finalIndex - start;
        positionFinder(start, path);
        removeForward(path, calculatedLine, calculatedIndex, length);
        insertStr(path, to, calculatedLine, calculatedIndex);
    }
    else
    {
        int start = findAt(index, from, path);
        char copyNext[1024];
        int finalIndex;
        strcpy(copyNext, nextSubstring);
        for (int i = 1; ; ++i) {
            finalIndex = findAt(i, copyNext, path);
            if(finalIndex >= start)
                break;
        }
        int length = finalIndex - start;
        positionFinder(start, path);
        removeForward(path, calculatedLine, calculatedIndex, length);
        insertStr(path, to, calculatedLine, calculatedIndex);
    }
}


int tree(int depth, int bitmask, int limit)
{
    if(depth > limit)
        return 1;
    int count = 0;
    DIR *directory;
    struct dirent *entry;
    directory = opendir(".");
    if(directory == NULL)
        return 1;
    while ((entry = readdir(directory)) != NULL)
        count++;
    closedir(directory);
    directory = opendir(".");
    entry = readdir(directory);
    entry = readdir(directory);
    if(count > 3)
        bitmask |= (1 << (depth - 1));
    for (int i = 2; i < count; ++i) {
        entry = readdir(directory);
        for (int j = 1; j < depth; ++j) {
            if(1 & (bitmask >> (j - 1)))
                printf("%c", (char)179);
            else
                printf(" ");
        }
        if(i == count - 1)
        {
            bitmask &= ~(1 << (depth - 1));
            printf("%c", (char)192);
        }
        else
        {
            printf("%c", (char)195);
        }
        printf("%s\n", entry->d_name);
        char currentPath1[1024];
        getcwd(currentPath1, 1024);
        if(!(0 - chdir(entry->d_name)))
        {
            tree(depth + 1, bitmask, limit);
            chdir(currentPath1);
        }

    }
    closedir(directory);
}


int compare(char *path, char *path2)
{
    char part[1024];
    if(wizard("/root/", path))
    {
        printf("Invalid path!\n");
        return 1;
    }
    int z = 0;
    for (int i = 0; ; ++i, z++)
    {
        char currentChar = path[z];
        if(currentChar == '/')
        {
            part[i] = '\0';
            if(0 - _chdir(part))
            {
                printf("Invalid file or directory!\n");
                backToStartingPoint();
                return 1;
            }
            strcat(currentPath, "\\");
            strcat(currentPath, part);
            i = -1;
        }
        else if(currentChar == '\0')
        {
            
            
            
            part[i] = '\0';
            FILE *fp = fopen(part, "r");
            char file1Name[1024];
            strcpy(file1Name, part);
            if(fp == NULL)
            {
                printf("Invalid file or directory!\n");
                backToStartingPoint();
                return 1;
            }
            else
            {
                backToStartingPoint();
                char part[1024];
                if(wizard("/root/", path2))
                {
                    printf("Invalid path!\n");
                    return 1;
                }
                z = 0;
                for (int i = 0; ; ++i, z++)
                {
                    currentChar = path2[z];
                    if(currentChar == '/')
                    {
                        part[i] = '\0';
                        if(0 - _chdir(part))
                        {
                            printf("Invalid file or directory!\n");
                            backToStartingPoint();
                            return 1;
                        }
                        strcat(currentPath, "\\");
                        strcat(currentPath, part);
                        i = -1;
                    }
                    else if(currentChar == '\0')
                    {
                        part[i] = '\0';
                        FILE *fp2 = fopen(part, "r");
                        if(fp2 == NULL)
                        {
                            printf("Invalid file or directory!\n");
                            backToStartingPoint();
                            return 1;
                        }
                        else
                        {
                            int flag = 1;
                            for (int j = 1; ; ++j)
                            {
                                char line1[1024] = "", line2[1024] = "";
                                fgets(line1, 1024, fp);
                                fgets(line2, 1024, fp2);
                                if(line1[0] == '\0' && line2[0] == '\0')
                                    break;
                                if(flag && ((line2[0] == '\0') ^ (line1[0] == '\0')))
                                {
                                    printf(">>>>>>>>>>>> #%d >>>>>>>>>>>>\n", j);
                                    if(line1[0] == '\0')
                                        printf("%s:\n", part);
                                    else
                                        printf("%s:\n", file1Name);
                                    flag = 0;
                                }
                                if((line2[0] == '\0') ^ (line1[0] == '\0'))
                                {
                                    if(line1[strlen(line1) - 1] == '\n')
                                        line1[strlen(line1) - 1] = '\0';
                                    if(line2[strlen(line2) - 1] == '\n')
                                        line2[strlen(line2) - 1] = '\0';
                                    printf("%s%s\n", line1, line2);
                                    continue;
                                }
                                else if(strcmp(line1, line2) != 0) {
                                    if(line1[strlen(line1) - 1] == '\n')
                                        line1[strlen(line1) - 1] = '\0';
                                    if(line2[strlen(line2) - 1] == '\n')
                                        line2[strlen(line2) - 1] = '\0';
                                    printf("============ #%d ============\n%s\n----------------------------\n%s\n", j, line1, line2);
                                }
                            }
                            backToStartingPoint();
                            fclose(fp2);
                            fclose(fp);
                            return 0;
                        }
                    }
                    else
                    {
                        part[i] = currentChar;
                    }
                }





                fclose(fp);
                return 0;
            }
        }
        else
        {
            part[i] = currentChar;
        }
    }
}


int autoIndent(char *path)
{
    char part[1024];
    if(wizard("/root/", path))
    {
        printf("Invalid path!\n");
        return 1;
    }
    int z = 0;
    for (int i = 0; ; ++i, z++)
    {
        char currentChar = path[z];
        if(currentChar == '/')
        {
            part[i] = '\0';
            if(0 - _chdir(part))
            {
                printf("Invalid file or directory!\n");
                backToStartingPoint();
                return 1;
            }
            strcat(currentPath, "\\");
            strcat(currentPath, part);
            i = -1;
        }
        else if(currentChar == '\0')
        {
            part[i] = '\0';
            FILE *fp = fopen(part, "r");
            if(fp == NULL)
            {
                printf("Invalid file or directory!\n");
                backToStartingPoint();
                return 1;
            }
            else
            {
                FILE *newOne = fopen("temp.txt", "w");
                char c;
                int newlineFlag = 1;
                int spaceFlag = 1;
                int indentation = 0;
                int createNewLine = 0;
                while((c = fgetc(fp)) != EOF)
                {
                    if(indentation < 0)
                        indentation = 0;

                    if(createNewLine)
                    {
                        fputc('\n', newOne);
                        createNewLine = 0;
                    }
                    if(c == ' ' && spaceFlag)
                        continue;
                    if(c == '\n' && newlineFlag)
                        continue;

                    if(c == '\n')
                    {
                        fputc('\n', newOne);
                        newlineFlag = 1;
                        spaceFlag = 1;
                        continue;
                    }

                    else if(c == ' ')
                    {
                        fputc(' ', newOne);
                        spaceFlag = 1;
                        newlineFlag = 0;
                        continue;
                    }


                    else if(c == '{')
                    {
                        if(!newlineFlag)
                            fputc('\n', newOne);
                        for (int j = 0; j < indentation; ++j)
                            fputs("    ", newOne);
                        fputc('{', newOne);
                        indentation++;
                        createNewLine = 1;
                        newlineFlag = 1;
                        spaceFlag = 1;
                        continue;
                    }

                    else if(c == '}')
                    {
                        indentation--;
                        if(!newlineFlag)
                            fputc('\n', newOne);
                        for (int j = 0; j < indentation; ++j)
                            fputs("    ", newOne);
                        fputs("}", newOne);
                        createNewLine = 1;
                        newlineFlag = 1;
                        spaceFlag = 1;
                        continue;
                    }

                    else
                    {
                        if(newlineFlag)
                            for (int j = 0; j < indentation; ++j) {
                                fputs("    ", newOne);
                            }
                        spaceFlag = 0;
                        newlineFlag = 0;
                        fputc(c, newOne);
                    }

                }
                fclose(fp);
                fclose(newOne);
                remove(part);
                rename("temp.txt", part);
                backToStartingPoint();
                printf("Success!\n");
                return 0;
            }
        }
        else
        {
            part[i] = currentChar;
        }
    }
}


int history(char *path)
{
    char path1[1024];
    strcpy(path1, path);
    if(wizard("/root/", path1))
    {
        return 1;
    }
    FILE *fp = fopen(path1, "r");
    if(fp == NULL)
        return 2;
    _chdir("C:\\parallel_root");
    strcpy(path1, path);
    createFile(path1);
    strcpy(path1, path);
    wizard("/root/", path1);
    _chdir("C:\\parallel_root");
    FILE *fp1 = fopen(path1, "w");
    char c;
    while ((c = fgetc(fp)) != EOF)
        fputc(c, fp1);
    fclose(fp);
    fclose(fp1);

    _chdir("C:\\root");
    return 0;
}


int undo(char *path)
{
    char path1[1024];
    strcpy(path1, path);
    if(wizard("/root/", path1))
    {
        return -1;
    }
    FILE *fp = fopen(path1, "r");
    if(fp == NULL)
        //Invalid file
        return -2;

    _chdir("C:\\parallel_root");
    FILE *temp = fopen("temp.temp", "w");
    char c;
    while ((c = fgetc(fp)) != EOF) {
        fputc(c, temp);
    }
    fclose(temp);

    _chdir("C:\\root");
    fclose(fp);
    fp = fopen(path1, "w");
    _chdir("C:\\parallel_root");
    strcpy(path1, path);
    createFile(path1);
    strcpy(path1, path);
    wizard("/root/", path1);
    _chdir("C:\\parallel_root");
    FILE *fp1 = fopen(path1, "r");
    if(fp1 == NULL)
    {
        //no previous backup
        return -3;
    }
    temp = fopen("temp.temp", "r");

    while ((c = fgetc(fp1)) != EOF)
        fputc(c, fp);

    fclose(fp1);
    fp1 = fopen(path1, "w");
    while ((c = fgetc(temp)) != EOF)
        fputc(c, fp1);
    fclose(temp);
    remove("temp.temp");
    fclose(fp);
    fclose(fp1);

    _chdir("C:\\root");
    return 0;
}


int main()
{
    if(1)
    {
        _chdir("C:\\");
        mkdir("root");
        mkdir("parallel_root");
        _chdir("C:\\root");
    }
    while (1)
    {
        fgets(input, 1024, stdin);

        if(!wizard("createfile ", input))
        {
            if(wizard("--file ", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            if(extractString())
            {
                printf("Invalid input!\n");
                continue;
            }
            if(wizard("\n", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            int a = createFile(text);
            if(a == -1)
                printf("Invalid path!\n");
            else if(a == -2)
                printf("Can't use the following characters: \", \', <, >, \?, *, :, |, \\");
            else if(a == -3)
                printf("There already exists a file with the given name!\n");
            else
                printf("Success!\n");

        }

        else if(!wizard("insertstr ", input))
        {
            if(wizard("--file ", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            if(extractString())
            {
                printf("Invalid input!\n");
                continue;
            }
            char path[1024];
            strcpy(path, text);
            if(wizard(" --str ", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            if(extractString())
            {
                printf("Invalid input!\n");
                continue;
            }
            if(wizard(" --pos ", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            if(barber(text))
            {
                printf("Invalid escape sequence!\n");
                continue;
            }
            char inputSTR[1024];
            strcpy(inputSTR, text);
            int lineNum = numExtract();
            if(wizard(":", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            int startPosition = numExtract();
            if(wizard("\n", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            history(path);
            insertStr(path, inputSTR, lineNum, startPosition);

        }

        else if(!wizard("cat ", input))
        {
            if(wizard("--file ", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            if(extractString())
            {
                printf("Invalid input!\n");
                continue;
            }
            if(wizard("\n", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            cat(text);
        }

        else if(!wizard("removestr ", input))
        {
            if(wizard("--file ", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            if(extractString())
            {
                printf("Invalid input!\n");
                continue;
            }
            if(wizard(" --pos ", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            int line = numExtract();
            if(wizard(":", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            int index = numExtract();
            if(wizard(" --size ", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            int length = numExtract();
            if(!wizard(" --b\n", input))
            {
                history(text);
                int res = removeBackward(text, line, index, length);
                if(!res)
                    printf("Success!\n");
                else if(res == 1)
                    printf("Invalid file or directory!\n");
                else if(res == 2)
                    printf("Out of bounds!\n");
            }
            else if(!wizard(" --f\n", input))
            {
                history(text);
                int res = removeForward(text, line, index, length);
                if(!res)
                    printf("Success!\n");
                else if(res == 1)
                    printf("Invalid file or directory!\n");
                else if(res == 2)
                    printf("Out of bounds!\n");
            }
            else
            {
                printf("Invalid input!\n");
                continue;
            }
        }

        else if(!wizard("copystr ", input))
        {
            if(wizard("--file ", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            if(extractString())
            {
                printf("Invalid input!\n");
                continue;
            }
            if(wizard(" --pos ", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            int line = numExtract();
            if(wizard(":", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            int index = numExtract();
            if(wizard(" --size ", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            int length = numExtract();
            if(!wizard(" --b\n", input))
            {
                copyBackward(text, line, index, length);
                printf("<%s>", clipBoard);
            }
            else if(!wizard(" --f\n", input))
            {
                copyForward(text, line, index, length);
            }
            else
            {
                printf("Invalid input!\n");
                continue;
            }
        }

        else if(!wizard("cutstr ", input))
        {
            if(wizard("--file ", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            if(extractString())
            {
                printf("Invalid input!\n");
                continue;
            }
            if(wizard(" --pos ", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            int line = numExtract();
            if(wizard(":", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            int index = numExtract();
            if(wizard(" --size ", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            int length = numExtract();
            if(!wizard(" --b\n", input))
            {
                history(text);
                copyBackward(text, line, index, length);
                removeBackward(text, line, index, length);
            }
            else if(!wizard(" --f\n", input))
            {
                history(text);
                copyForward(text, line, index, length);
                removeForward(text, line, index, length);
            }
            else
            {
                printf("Invalid input!\n");
                continue;
            }
        }

        else if(!wizard("pastestr ", input))
        {
            if(wizard("--file ", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            if(extractString())
            {
                printf("Invalid input!\n");
                continue;
            }
            if(wizard(" --pos ", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            int line = numExtract();
            if(wizard(":", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            int index = numExtract();
            if(wizard("\n", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            history(text);
            insertStr(text, clipBoard, line, index);
        }

        else if(!wizard("find ", input))
        {
            if(wizard("--str ", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            if(extractString())
            {
                printf("Invalid input!\n");
                continue;
            }

            char findingStr[1024];
            strcpy(findingStr, text);

            if(wizard(" --file ", input))
            {
                printf("Invalid input!\n");
                continue;
            }

            if(extractString())
            {
                printf("Invalid input!\n");
                continue;
            }

            if(!wizard("\n", input))
            {
                printf("%d\n", findAt(1, findingStr, text));
                continue;
            }
            else if(!wizard(" --count\n", input))
            {
                for (int i = 0; ; ++i) {
                    if(findAt(i + 1, findingStr, text) == -1)
                    {
                        printf("%d\n", i);
                        break;
                    }
                }
                continue;
            }
            else if(!wizard(" --at ", input))
            {
                int index = numExtract();
                if(wizard("\n", input))
                {
                    printf("Invalid input!\n");
                    continue;
                }
                printf("%d\n", findAt(index, findingStr, text));
                printf("next substring: <%s>\n", nextSubstring);
            }
            else if(!wizard(" --byword\n", input))
            {
                printf("%d\n", findAtByWord(1, findingStr, text));
            }
            else if(!wizard(" --byword --at ", input))
            {
                int index = numExtract();
                if(wizard("\n", input))
                {
                    printf("Invalid input!\n");
                    continue;
                }
                printf("%d\n", findAtByWord(index, findingStr, text));
            }
            else if(!wizard(" --all --byword\n", input))
            {
                for (int i = 1; ; ++i)
                {
                    int res;
                    if(-1 != (res = findAtByWord(i, findingStr, text)))
                    {
                        if(i != 1)
                            printf(", ");
                        printf("%d", res);
                    }
                    else
                        break;
                }
                printf("\n");
            }
            else if(!wizard(" --all\n", input))
            {
                for (int i = 1; ; ++i)
                {
                    int res;
                    if(-1 != (res = findAt(i, findingStr, text)))
                    {
                        if(i != 1)
                            printf(", ");
                        printf("%d", res);
                    }
                    else
                        break;
                }
                printf("\n");
            }
            else
                printf("Invalid input!\n");
        }

        else if(!wizard("replace ", input))
        {

            if(wizard("--str1 ", input))
            {
                printf("Invalid input!\n");
                continue;
            }

            if(extractString())
            {
                printf("Invalid input!\n");
                continue;
            }

            char replacing[1024];
            strcpy(replacing, text);

            if(wizard(" --str2 ", input))
            {
                printf("Invalid input!\n");
                continue;
            }

            if(extractString())
            {
                printf("Invalid input!\n");
                continue;
            }

            char replacement[1024];
            strcpy(replacement, text);


            if(wizard(" --file ", input))
            {
                printf("Invalid input!\n");
                continue;
            }

            if(extractString())
            {
                printf("Invalid input!\n");
                continue;
            }

            if(!wizard("\n", input))
            {
                history(text);
                replace(replacing, replacement, 1, text);
            }


        }

        else if(!wizard("tree ", input))
        {
            int depth = numExtract();
            if(wizard("\n", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            printf("root\n");
            tree(1, 0, depth);
        }

        else if(!wizard("grep ", input))
        {
            if(!wizard("--c --str ", input))
            {
                extractString();
                char searchingSTR[1024];
                strcpy(searchingSTR, text);
                if(!wizard(" --file ", input))
                {
                    extractString();
                    if(wizard("\n", input))
                    {
                        printf("Invalid input!\n");
                        continue;
                    }
                    for (int i = 0; ; ++i) {
                        if(findAt(i + 1, searchingSTR, text) == -1)
                        {
                            printf("%d\n", i);
                            break;
                        }
                    }
                }
                else if(!wizard(" --files ", input))
                {
                    int result = 0;
                    while (1) {
                        extractString();
                        for (int i = 0;; ++i) {
                            int a;
                            if ((a = findAt(i + 1, searchingSTR, text)) == -1) {
                                result += i;
                                break;
                            } else if (a == -2)
                                goto end;
                        }
                        if(!wizard("\n", input))
                        {
                            printf("%d\n", result);
                            break;
                        }
                        wizard(" ", input);
                    }
                    end:;
                }
                else
                {
                    printf("Invalid input!\n");
                }
            }
            else if(!wizard("--l --str ", input))
            {
                extractString();
                char searchingSTR[1024];
                strcpy(searchingSTR, text);
                if(!wizard(" --file ", input))
                {
                    extractString();
                    char currentFile[1024];
                    strcpy(currentFile, text);
                    if(wizard("\n", input))
                    {
                        printf("Invalid input!\n");
                        continue;
                    }
                    for (int i = 0; ; ++i) {
                        if(findAt(i + 1, searchingSTR, text) == -1)
                        {
                            if(i != 0)
                                printf("%s\n", currentFile);
                            break;
                        }
                    }
                }
                else if(!wizard(" --files ", input))
                {
                    int result = 0;
                    while (1) {
                        extractString();
                        char currentFile[1024];
                        strcpy(currentFile, text);
                        for (int i = 0;; ++i) {
                            int a;
                            if ((a = findAt(i + 1, searchingSTR, text)) == -1) {
                                if(i != 0)
                                    printf("%s\n", currentFile);
                                break;
                            } else if (a == -2)
                                goto end1;
                        }
                        if(!wizard("\n", input))
                            break;
                        wizard(" ", input);
                    }
                    end1:;
                }
                else
                {
                    printf("Invalid input!\n");
                }
            }
            else if(!wizard("--str ", input))
            {
                extractString();
                char searchingSTR[1024];
                strcpy(searchingSTR, text);
                if(!wizard(" --file ", input))
                {
                    extractString();
                    if(wizard("\n", input))
                    {
                        printf("Invalid input!\n");
                        continue;
                    }
                    for (int i = 0; ; ++i) {
                        int a;
                        if((a = findAt(i + 1, searchingSTR, text)) >= 0)
                        {

                            positionFinder(a, text);
                            catLine(text, calculatedLine);
                            break;
                        }
                    }
                }
                else if(!wizard(" --files ", input))
                {
                    int result = 0;
                    while (1) {
                        extractString();
                        char currentFile[1024];
                        strcpy(currentFile, text);
                        for (int i = 0;; ++i) {
                            int a;
                            if ((a = findAt(i + 1, searchingSTR, text)) != -1 && a != -2) {
                                printf("%s: ", currentFile);
                                positionFinder(a, text);
                                catLine(text, calculatedLine);
                            } 
                            else
                                break;
                        }
                        if(!wizard("\n", input))
                        {
                            break;
                        }

                        wizard(" ", input);
                    }
                    end2:;
                }
                else
                {
                    printf("Invalid input!\n");
                }
            }
        }

        else if(!wizard("auto-indent ", input))
        {
            extractString();
            if(wizard("\n", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            history(text);
            autoIndent(text);
        }

        else if(!wizard("undo ", input))
        {
            extractString();
            if(wizard("\n", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            int a = undo(text);
            if(a == -1)
                printf("Invalid path!\n");
            else if(a == -2)
                printf("The file you are looking for does not exist!\n");
            else if(a == -3)
                printf("There is no history for the file given!\n");
            else
                printf("Success!\n");
        }

        else if(!wizard("compare ", input))
        {
            extractString();
            char file1[1024];
            strcpy(file1, text);
            if(wizard(" ", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            extractString();
            if(wizard("\n", input))
            {
                printf("Invalid input!\n");
                continue;
            }
            compare(file1, text);
        }

        else
        {
            printf("Invalid input!\n");
        }
    }
}