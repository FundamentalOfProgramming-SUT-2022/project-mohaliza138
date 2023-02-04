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
char printingResult[1000000];


void errorHandler(int err, int printSuccess)
{
    switch (err)
    {
        case 0:
            if(printSuccess)
                strcat(printingResult, "Success!\n");
            break;
        case -1:
            break;
        case -2:
            strcat(printingResult, "Invalid file or directory!\n");
            break;
        case -3:
            strcat(printingResult, "Out fo bounds!\n");
            break;
        case -4:
            strcat(printingResult, "There already exists a file with the name given!\n");
            break;
        case -5:
            strcat(printingResult, "Can't use the following characters: \", \', <, >, \?, *, :, |, \\\n");
            break;
        case -6:
            strcat(printingResult, "No previous backup!\n");
            break;
        default:
            break;
    }
}


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


int createFile(char *path1)
{
    char path[1024];
    strcpy(path, path1);
    if(wizard("/root/", path))
    {
        return -2;
    }
    int size = (int )strlen(path);
    for (int i = 0; i < size; ++i)
    {
        if(path[i] == '"' || path[i] == '<' || path[i] == '?' || path[i] == '*' || path[i] == ':' || path[i] == '|' || path[i] == '\\' || path[i] == '>')
        {
            return -5;
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
                return -4;
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


int cat(char *path1)
{
    char path[1024];
    strcpy(path, path1);
    char part[1024];
    if(wizard("/root/", path))
    {
        return -2;
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
                backToStartingPoint();
                return -2;
            }
            else
            {
                int n = (int)strlen(part);
                for (int j = n + 4; j < 50; ++j)
                {
                    strcat(printingResult, "-");
                }
                char temp[1024];
                sprintf(temp, " %s --\n", part);
                strcat(printingResult, temp);
                while (1)
                {
                    char lines[1024];
                    lines[0] = '\0';
                    fgets(lines, 1024, fp);
                    if(lines[0] == '\0')
                        break;
                    char temp[1024];
                    sprintf(temp, "%s", lines);
                    strcat(printingResult, temp);
                }
                strcat(printingResult, "\n--------------------------------------------------\n");//50-
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
        return -2;
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
                backToStartingPoint();
                return -2;
            }
            else
            {
                char sth[1024];
                for (int j = 0; j < line; ++j) {
                    fgets(sth, 1024, fp);
                }
                char temp[1024];
                sprintf(temp, "%s", sth);
                strcat(printingResult, temp);
                if(sth[strlen(sth) - 1] != '\n')
                    strcat(printingResult, "\n");
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
                backToStartingPoint();
                return -2;
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
                        fclose(fp);
                        remove("temp.txt");
                        backToStartingPoint();
                        return -3;
                    }
                }
                fgets(input, 1024, fp);
                char c;
                for (int j = 0; j < charIndex; ++j)
                {
                    c = input[0];
                    if(c == '\n' || c == '\0')
                    {
                        fclose(fp);
                        remove("temp.txt");
                        backToStartingPoint();
                        return -3;
                    }
                    char tmp[2];
                    tmp[0] = c;
                    tmp[1] = '\0';
                    wizard(tmp, input);
                }
                memset(clipBoard, 0, 1000000);
                for (int j = 0; j < length; ++j)
                {
                    if(input[0] == '\0')
                    {
                        fgets(input, 1024, fp);
                        if(input[0] == '\0')
                        {
                            fclose(fp);
                            remove("temp.txt");
                            backToStartingPoint();
                            return -3;
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
                return 0;
            }
        }
        else
        {
            part[i] = currentChar;
        }
    }
}


int copyBackward(char *path1, int line, int charIndex, int length)
{
    char path[1024];
    strcpy(path, path1);
    char part[1024];
    if(wizard("/root/", path))
    {
        return -2;
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
                backToStartingPoint();
                return -2;
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
                        return -3;
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
                        return copyForward(path1, j, sizes[j] - templen, length);
                    }
                    else
                    {
                        templen -= sizes[j];
                    }
                }
                return -3;
            }
        }
        else
        {
            part[i] = currentChar;
        }
    }
}


int insertStr(char *path1, char *content, int line, int charIndex)
{
    char path[1024];
    strcpy(path, path1);
    char part[1024];
    if(wizard("/root/", path))
    {
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
                backToStartingPoint();
                return -2;
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
                        return -3;
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
                        return -3;
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
                return 0;
            }
        }
        else
        {
            part[i] = currentChar;
        }
    }
}


int removeForward(char *path1, int line, int charIndex, int length)
{
    char path[1024];
    strcpy(path, path1);
    char part[1024];
    if(wizard("/root/", path))
    {
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
                backToStartingPoint();
                return -2;
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
                        return -3;
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
                        return -3;
                    }
                    fputc(c, newOne);
                    char tmp[2];
                    tmp[0] = c;
                    tmp[1] = '\0';
                    wizard(tmp, input);
                }
                for (int j = 0; j < length; ++j)
                {
                    if(input[0] == '\0')
                    {
                        fgets(input, 1024, fp);
                        if(input[0] == '\0')
                        {
                            fclose(newOne);
                            fclose(fp);
                            remove("temp.txt");
                            backToStartingPoint();
                            return -3;
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


int removeBackward(char *path1, int line, int charIndex, int length)
{
    char path[1024];
    strcpy(path, path1);
    char part[1024];
    if(wizard("/root/", path))
    {
        return -2;
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
                backToStartingPoint();
                return -2;
            }
            else
            {
                int sizes[line + 1];
                for (int j = 1; j <= line; ++j)
                {
                    char lines[1024];
                    lines[0] = '\0';
                    fgets(lines, 1024, fp);
                    if(lines[0] == '\0')
                    {
                        fclose(fp);
                        backToStartingPoint();
                        return -3;
                    }
                    sizes[j] = (int) strlen(lines);
                }
                if(sizes[line] < charIndex)
                {
                    fclose(fp);
                    backToStartingPoint();
                    return -3;
                }
                sizes[line] = charIndex;
                fclose(fp);
                backToStartingPoint();
                int templen = length;
                for (int j = line; j > 0; --j)
                {
                    if(sizes[j] >= templen)
                    {
                        return removeForward(path1, j, sizes[j] - templen, length);
                    }
                    else
                    {
                        templen -= sizes[j];
                    }
                }
                return -3;
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

    if (!strlen(first))
    {
        strcpy(nextSubstring, second);
        return ++counter == indexToReach;
    }

//    if (strlen(first) == 0 && (second[0] == '\0' || second[0] == '\n' || second[0] == ' '))
//    {
//        return ++counter == indexToReach;
//    }


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
        if(second[0] == ' ' || second[0] == '\0' || strlen(first) == 1)
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
    char second[1024];
    char part[1024];
    if(wizard("/root/", path))
    {
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
                backToStartingPoint();
                return -2;
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
    if(b < 0)
        return b;
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
        return insertStr(path, to, calculatedLine, calculatedIndex);
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
        return insertStr(path, to, calculatedLine, calculatedIndex);
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
            {
                char temp[1024];
                sprintf(temp, "%c", (char)179);
                strcat(printingResult, temp);
            }
            else
                strcat(printingResult, " ");
        }
        if(i == count - 1)
        {
            bitmask &= ~(1 << (depth - 1));
            char temp[1024];
            sprintf(temp, "%c", (char)192);
            strcat(printingResult, temp);
        }
        else
        {
            char temp[1024];
            sprintf(temp, "%c", (char)195);
            strcat(printingResult, temp);
        }
        char temp[1024];
        sprintf(temp, "%s\n", entry->d_name);
        strcat(printingResult, temp);
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


int compare(char *path1, char *path22)
{
    char path2[1024], path[1024];
    strcpy(path2, path22);
    strcpy(path, path1);
    char part[1024];
    if(wizard("/root/", path))
    {
        return -2;
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
            char file1Name[1024];
            strcpy(file1Name, part);
            if(fp == NULL)
            {
                backToStartingPoint();
                return -2;
            }
            else
            {
                backToStartingPoint();
                char part[1024];
                if(wizard("/root/", path2))
                {
                    fclose(fp);
                    return -2;
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
                            backToStartingPoint();
                            fclose(fp);
                            return -2;
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
                            backToStartingPoint();
                            fclose(fp);
                            return -2;
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
                                    char temp[1024];
                                    sprintf(temp, ">>>>>>>>>>>> #%d >>>>>>>>>>>>\n", j);
                                    strcat(printingResult, temp);
                                    if(line1[0] == '\0')
                                    {
                                        char temp[1024];
                                        sprintf(temp, "%s:\n", part);
                                        strcat(printingResult, temp);
                                    }
                                    else
                                    {
                                        char temp[1024];
                                        sprintf(temp, "%s:\n", file1Name);
                                        strcat(printingResult, temp);
                                    }
                                    flag = 0;
                                }
                                if((line2[0] == '\0') ^ (line1[0] == '\0'))
                                {
                                    if(line1[strlen(line1) - 1] == '\n')
                                        line1[strlen(line1) - 1] = '\0';
                                    if(line2[strlen(line2) - 1] == '\n')
                                        line2[strlen(line2) - 1] = '\0';

                                    char temp[1024];
                                    sprintf(temp, "%s%s\n", line1, line2);
                                    strcat(printingResult, temp);
                                    continue;
                                }
                                else if(strcmp(line1, line2) != 0) {
                                    if(line1[strlen(line1) - 1] == '\n')
                                        line1[strlen(line1) - 1] = '\0';
                                    if(line2[strlen(line2) - 1] == '\n')
                                        line2[strlen(line2) - 1] = '\0';

                                    char temp[1024];
                                    sprintf(temp, "============ #%d ============\n%s\n----------------------------\n%s\n", j, line1, line2);
                                    strcat(printingResult, temp);
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
            }
        }
        else
        {
            part[i] = currentChar;
        }
    }
}


int autoIndent(char *path1)
{
    char path[1024];
    strcpy(path, path1);
    char part[1024];
    if(wizard("/root/", path))
    {
        return -2;
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
                backToStartingPoint();
                return -2;
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
        return -2;
    }
    FILE *fp = fopen(path1, "r");
    if(fp == NULL)
        return -2;
    _chdir("C:\\parallel_root");
    strcpy(path1, path);
    strcat(path1, ".temp");
    createFile(path1);
    strcpy(path1, path);
    wizard("/root/", path1);
    _chdir("C:\\parallel_root");
    strcat(path1, ".temp");
    FILE *fp1 = fopen(path1, "w");
    char c;
    while ((c = fgetc(fp)) != EOF)
        fputc(c, fp1);
    fclose(fp);
    fclose(fp1);

    _chdir("C:\\root");
    return 0;
}


int submitHistory(char *path)
{
    char path1[1024];
    strcpy(path1, path);
    _chdir("C:\\parallel_root");
    strcat(path1, ".temp");
    wizard("/root/", path1);
    FILE *fp = fopen(path1, "r");
    if(fp == NULL)
        return -2;
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
        return -2;
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
        return -6;
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


void Arman()
{
    char stringGiven[1024];
    strcpy(stringGiven, printingResult);
    printingResult[0] = '\0';
    if(!wizard("insertstr --file ", input))
    {
        extractString();
        if(wizard(" --pos ", input))
        {
            printf("Invalid input!\n");
            return;
        }
        int lineNo = numExtract();
        if(wizard(":", input))
        {
            printf("Invalid input!\n");
            return;
        }
        int charNo = numExtract();
        if(wizard("\n", input))
        {
            printf("Invalid input!\n");
            return;
        }
        int a = insertStr(text, stringGiven, lineNo, charNo);
        errorHandler(a, 1);
        printf("%s", printingResult);
    }

    else if(!wizard("find --file ", input))
    {
        extractString();
        if(!wizard("\n", input))
        {
            int a = findAt(1, stringGiven, text);
            if(a >= -1)
            {
                char temp[1024];
                sprintf(temp, "%d\n", a);
                strcat(printingResult, temp);
            }
            errorHandler(a, 0);
        }
        else if(!wizard(" --count\n", input))
        {
            for (int i = 0; ; ++i) {
                int a = findAt(i + 1, stringGiven, text);
                if(a == -1)
                {
                    char temp[1024];
                    sprintf(temp, "%d\n", i);
                    strcat(printingResult, temp);
                    break;
                }
                if(a < -1)
                {
                    errorHandler(a, 0);
                    break;
                }
            }
        }
        else if(!wizard(" --at ", input))
        {
            int index = numExtract();
            if(wizard("\n", input))
            {
                strcat(printingResult, "Invalid input!\n");
                return;
            }
            int a;
            a = findAt(index, stringGiven, text);
            if(a >= -1)
            {
                char temp[1024];
                sprintf(temp, "%d\n", a);
                strcat(printingResult, temp);
            }
            errorHandler(a, 0);
        }
        else if(!wizard(" --byword\n", input))
        {
            int a;
            a = findAtByWord(1, stringGiven, text);
            errorHandler(a, 0);
            if(a >= -1)
            {
                char temp[1024];
                sprintf(temp, "%d\n", a);
                strcat(printingResult, temp);
            }
        }
        else if(!wizard(" --byword --at ", input))
        {
            int index = numExtract();
            if(wizard("\n", input))
            {
                strcat(printingResult, "Invalid input!\n");
            }
            int a = findAtByWord(index, stringGiven, text);
            errorHandler(a, 0);
            if(a >= -1)
            {
                char temp[1024];
                sprintf(temp, "%d\n", a);
                strcat(printingResult, temp);
            }
        }
        else if(!wizard(" --all --byword\n", input))
        {
            int newlineFlag;
            for (int i = 1; ; ++i)
            {
                int res = findAtByWord(i, stringGiven, text);
                errorHandler(res, 0);
                newlineFlag = res >= -1;
                if(-1 < res)
                {
                    if(i != 1)
                        strcat(printingResult, ", ");
                    char temp[1024];
                    sprintf(temp, "%d\n", res);
                    strcat(printingResult, temp);
                    res = findAtByWord(i, stringGiven, text);
                }
                else
                    break;
            }
            if(newlineFlag)
                strcat(printingResult, "\n");
        }
        else if(!wizard(" --all\n", input))
        {
            int newlineFlag;
            for (int i = 1; ; ++i)
            {
                int res = findAt(i, stringGiven, text);
                errorHandler(res, 0);
                newlineFlag = res >= -1;
                if(-1 < res)
                {
                    if(i != 1)
                        strcat(printingResult, ", ");
                    char temp[1024];
                    sprintf(temp, "%d\n", res);
                    strcat(printingResult, temp);
                    res = findAt(i, stringGiven, text);
                }
                else
                    break;
            }
            if(newlineFlag)
                strcat(printingResult, "\n");
        }
        else
            strcat(printingResult, "Invalid input!\n");
        printf("%s", printingResult);
    }

    else if(!wizard("replace ", input))
    {
        char str1[1024], str2[1024];
        if(!wizard("--str1 ", input))
        {
            strcpy(str2, stringGiven);
            extractString();
            strcpy(str1, text);
        }
        else if(!wizard("--str2 ", input))
        {
            strcpy(str1, stringGiven);
            extractString();
            strcpy(str2, text);
        }
        else
        {
            strcat(printingResult, "Invalid input!\n");
            printf("%s", printingResult);
            return;
        }
        if(wizard(" --file ", input))
        {
            printf("Invalid input!\n");
            return;
        }
        extractString();

        if(!wizard("\n", input))
        {
            history(text);
            int a;
            if((a = replace(str1, str2, 1, text)) == 0)
                submitHistory(text);
            errorHandler(a, 1);
            printf("%s", printingResult);
        }
        else if(!wizard(" --at ", input))
        {
            int index = numExtract();
            if(!wizard("\n", input))
            {
                history(text);
                int a;
                if((a = replace(str1, str2, index, text)) == 0)
                    submitHistory(text);
                errorHandler(a, 1);
                printf("%s", printingResult);
            }
            else
            {
                printf("Invalid input!\n");
                return;
            }
        }
        else if(!wizard(" --all", input)) {
            if (!wizard("\n", input)) {
                history(text);
                int count = 0;
                for (;; ++count) {
                    int a = findAt(count + 1, str1, text);
                    if (a == -1) {
                        break;
                    }
                    if (a < -1) {
                        errorHandler(a, 1);
                        printf("%s", printingResult);
                        return;
                    }
                }
                int range = findAt(count, str1, text) + 1;
                for (count; count > 0; count--) {
                    int a = findAt(count, str1, text);
                    if (a >= range || a == -1)
                        continue;
                    replace(str1, str2, count, text);
                }
                submitHistory(text);
                printf("Success!\n");
            } else {
                printf("Invalid input!\n");
                return;
            }
        }
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
                    printf( "Invalid input!\n");
                    return;
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
                        }
                        else if (a <= -2)
                        {
                            errorHandler(a, 0);
                            printf("%s", printingResult);
                            return;
                        }
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
                    return;
                }
                for (int i = 0; ; ++i) {
                    if(findAt(i + 1, searchingSTR, text) == -1)
                    {
                        if(i != 0)
                        {
                            printf("%s\n", currentFile);
                        }
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
                            if(i != 0) {
                                printf("%s\n", currentFile);
                            }
                            break;
                        } else if (a <= -2)
                        {
                            errorHandler(a, 0);
                            printf("%s", printingResult);
                        }
                    }
                    if(!wizard("\n", input))
                        break;
                    wizard(" ", input);
                }
            }
            else
            {
                strcat(printingResult, "Invalid input!\n");
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
                    return;
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
                printf("%s", printingResult);
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
                        if ((a = findAt(i + 1, searchingSTR, text)) >= 0) {
                            char temp[1024];
                            sprintf(temp, "%s: ", currentFile);
                            strcat(printingResult, temp);
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
                printf("%s", printingResult);
            }
            else
            {
                printf("Invalid input!\n");
            }
        }
        else
            printf("Invalid input!\n");
    }

    else
        printf("Invalid input!\n");
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
        restart:
        printingResult[0] = '\0';
        fgets(input, 1024, stdin);

        if(!wizard("createfile ", input))
        {
            if(wizard("--file ", input))
            {
                goto invalid;
            }
            if(extractString())
            {
                goto invalid;
            }
            if(!wizard("\n", input))
            {
                int a = createFile(text);
                errorHandler(a, 1);
                printf("%s", printingResult);
            }
            else if(!wizard(" =D ", input))
            {
                int a = createFile(text);
                errorHandler(a, 1);
                Arman();
            }
            else
            {
                goto invalid;
            }
        }

        
        else if(!wizard("insertstr ", input))
        {
            if(wizard("--file ", input))
            {
                goto invalid;
            }
            if(extractString())
            {
                goto invalid;
            }
            char path[1024];
            strcpy(path, text);
            if(wizard(" --str ", input))
            {
                goto invalid;
            }
            if(extractString())
            {
                goto invalid;
            }
            if(wizard(" --pos ", input))
            {
                goto invalid;
            }
            if(barber(text))
            {
                strcat(printingResult, "Invalid escape sequence!\n");
                continue;
            }
            char inputSTR[1024];
            strcpy(inputSTR, text);
            int lineNum = numExtract();
            if(wizard(":", input))
            {
                goto invalid;
            }
            int startPosition = numExtract();
            if(!wizard("\n", input))
            {
                history(path);
                int a;
                if((a = insertStr(path, inputSTR, lineNum, startPosition)) == 0)
                    submitHistory(path);
                errorHandler(a, 1);
                printf("%s", printingResult);
            }
            else if(!wizard(" =D ", input))
            {
                history(path);
                int a;
                if((a = insertStr(path, inputSTR, lineNum, startPosition)) == 0)
                    submitHistory(path);
                errorHandler(a, 1);
                Arman();
            }
            else
                goto invalid;
        }

        
        else if(!wizard("cat ", input))
        {
            if(wizard("--file ", input))
            {
                goto invalid;
            }
            if(extractString())
            {
                goto invalid;
            }
            if(!wizard("\n", input))
            {
                int a = cat(text);
                errorHandler(a, 0);
                printf("%s", printingResult);
            }
            else if(!wizard(" =D ", input))
            {
                int a = cat(text);
                errorHandler(a, 0);
                Arman();
            }
            else
                goto invalid;
        }

        
        else if(!wizard("removestr ", input))
        {
            if(wizard("--file ", input))
            {
                goto invalid;
            }
            if(extractString())
            {
                goto invalid;
            }
            if(wizard(" --pos ", input))
            {
                goto invalid;
            }
            int line = numExtract();
            if(wizard(":", input))
            {
                goto invalid;
            }
            int index = numExtract();
            if(wizard(" --size ", input))
            {
                goto invalid;
            }
            int length = numExtract();
            if(!wizard(" --b\n", input))
            {
                history(text);
                int res = removeBackward(text, line, index, length);
                if(res == 0)
                    submitHistory(text);
                errorHandler(res, 1);
                printf("%s", printingResult);
            }
            else if(!wizard(" --f\n", input))
            {
                history(text);
                int res = removeForward(text, line, index, length);
                if(!res)
                    submitHistory(text);
                errorHandler(res, 1);
                printf("%s", printingResult);
            }
            else if(!wizard(" --b =D ", input))
            {
                history(text);
                int res = removeBackward(text, line, index, length);
                if(res == 0)
                    submitHistory(text);
                errorHandler(res, 1);
                Arman();
            }
            else if(!wizard(" --f =D ", input))
            {
                history(text);
                int res = removeForward(text, line, index, length);
                if(!res)
                    submitHistory(text);
                errorHandler(res, 1);
                Arman();
            }
            else
            {
                goto invalid;
            }
        }

        
        else if(!wizard("copystr ", input))
        {
            if(wizard("--file ", input))
            {
                goto invalid;
            }
            if(extractString())
            {
                goto invalid;
            }
            if(wizard(" --pos ", input))
            {
                goto invalid;
            }
            int line = numExtract();
            if(wizard(":", input))
            {
                goto invalid;
            }
            int index = numExtract();
            if(wizard(" --size ", input))
            {
                goto invalid;
            }
            int length = numExtract();
            if(!wizard(" --b\n", input))
            {
                int a = copyBackward(text, line, index, length);
                errorHandler(a, 1);
                printf("%s", printingResult);
            }
            else if(!wizard(" --f\n", input))
            {
                int a = copyForward(text, line, index, length);
                errorHandler(a, 1);
                printf("%s", printingResult);
            }
            else if(!wizard(" --b =D ", input))
            {
                int a = copyBackward(text, line, index, length);
                errorHandler(a, 1);
                Arman();
            }
            else if(!wizard(" --f =D ", input))
            {
                int a = copyForward(text, line, index, length);
                errorHandler(a, 1);
                Arman();
            }
            else
            {
                goto invalid;
            }
        }

        
        else if(!wizard("cutstr ", input))
        {
            if(wizard("--file ", input))
            {
                goto invalid;
            }
            if(extractString())
            {
                goto invalid;
            }
            if(wizard(" --pos ", input))
            {
                goto invalid;
            }
            int line = numExtract();
            if(wizard(":", input))
            {
                goto invalid;
            }
            int index = numExtract();
            if(wizard(" --size ", input))
            {
                goto invalid;
            }
            int length = numExtract();
            if(!wizard(" --b\n", input))
            {
                history(text);
                copyBackward(text, line, index, length);
                int a;
                if((a = removeBackward(text, line, index, length)) == 0)
                    submitHistory(text);
                errorHandler(a, 1);
                printf("%s", printingResult);
            }
            else if(!wizard(" --f\n", input))
            {
                history(text);
                copyForward(text, line, index, length);
                int a;
                if((a = removeForward(text, line, index, length)) == 0)
                    submitHistory(text);
                errorHandler(a, 1);
                printf("%s", printingResult);
            }
            else if(!wizard(" --b =D ", input))
            {
                history(text);
                copyBackward(text, line, index, length);
                int a;
                if((a = removeBackward(text, line, index, length)) == 0)
                    submitHistory(text);
                errorHandler(a, 1);
                Arman();
            }
            else if(!wizard(" --f =D ", input))
            {
                history(text);
                copyForward(text, line, index, length);
                int a;
                if((a = removeForward(text, line, index, length)) == 0)
                    submitHistory(text);
                errorHandler(a, 1);
                Arman();
            }
            else
            {
                goto invalid;
            }
        }

        
        else if(!wizard("pastestr ", input))
        {
            if(wizard("--file ", input))
            {
                goto invalid;
            }
            if(extractString())
            {
                goto invalid;
            }
            if(wizard(" --pos ", input))
            {
                goto invalid;
            }
            int line = numExtract();
            if(wizard(":", input))
            {
                goto invalid;
            }
            int index = numExtract();
            if(!wizard("\n", input))
            {
                history(text);
                int a;
                if((a = insertStr(text, clipBoard, line, index)) == 0)
                    submitHistory(text);
                errorHandler(a, 1);
                printf("%s", printingResult);
            }
            else if(!wizard(" =D ", input))
            {
                history(text);
                int a;
                if((a = insertStr(text, clipBoard, line, index)) == 0)
                    submitHistory(text);
                errorHandler(a, 1);
                Arman();
            }
            else
                goto invalid;
        }

        
        else if(!wizard("find ", input))
        {
            if(wizard("--str ", input))
            {
                goto invalid;
            }
            if(extractString())
            {
                goto invalid;
            }

            char findingStr[1024];
            strcpy(findingStr, text);

            if(wizard(" --file ", input))
            {
                goto invalid;
            }

            if(extractString())
            {
                goto invalid;
            }

            if(!wizard("\n", input))
            {
                int a = findAt(1, findingStr, text);
                if(a >= -1)
                {
                    char temp[1024];
                    sprintf(temp, "%d\n", a);
                    strcat(printingResult, temp);
                }
                errorHandler(a, 0);
                printf("%s", printingResult);
                continue;
            }

            else if(!wizard(" =D ", input))
            {
                int a = findAt(1, findingStr, text);
                if(a >= -1)
                {
                    char temp[1024];
                    sprintf(temp, "%d\n", a);
                    strcat(printingResult, temp);
                }
                errorHandler(a, 0);
                Arman();
                continue;
            }

            else if(!wizard(" --count\n", input))
            {
                for (int i = 0; ; ++i) {
                    int a = findAt(i + 1, findingStr, text);
                    if(a == -1)
                    {
                        char temp[1024];
                        sprintf(temp, "%d\n", i);
                        strcat(printingResult, temp);
                        break;
                    }
                    if(a < -1)
                    {
                        errorHandler(a, 0);
                        break;
                    }
                }
                printf("%s", printingResult);
                continue;
            }

            else if(!wizard(" --count =D ", input))
            {
                for (int i = 0; ; ++i) {
                    int a = findAt(i + 1, findingStr, text);
                    if(a == -1)
                    {
                        char temp[1024];
                        sprintf(temp, "%d\n", i);
                        strcat(printingResult, temp);
                        break;
                    }
                    if(a < -1)
                    {
                        errorHandler(a, 0);
                        break;
                    }
                }
                Arman();
                continue;
            }

            else if(!wizard(" --at ", input))
            {
                int index = numExtract();
                if(!wizard("\n", input))
                {
                    int a;
                    a = findAt(index, findingStr, text);
                    if(a >= -1)
                    {
                        char temp[1024];
                        sprintf(temp, "%d\n", a);
                        strcat(printingResult, temp);
                    }
                    errorHandler(a, 0);
                    printf("%s", printingResult);
                }
                else if(!wizard(" =D ", input))
                {
                    int a;
                    a = findAt(index, findingStr, text);
                    if(a >= -1)
                    {
                        char temp[1024];
                        sprintf(temp, "%d\n", a);
                        strcat(printingResult, temp);
                    }
                    errorHandler(a, 0);
                    Arman();
                }
                else
                    goto invalid;
            }

            else if(!wizard(" --byword\n", input))
            {
                int a;
                a = findAtByWord(1, findingStr, text);
                errorHandler(a, 0);
                if(a >= -1)
                {
                    char temp[1024];
                    sprintf(temp, "%d\n", a);
                    strcat(printingResult, temp);
                }
                printf("%s", printingResult);
            }

            else if(!wizard(" --byword =D ", input))
            {
                int a;
                a = findAtByWord(1, findingStr, text);
                errorHandler(a, 0);
                if(a >= -1)
                {
                    char temp[1024];
                    sprintf(temp, "%d\n", a);
                    strcat(printingResult, temp);
                }
                Arman();
            }

            else if(!wizard(" --byword --at ", input))
            {
                int index = numExtract();
                if(!wizard("\n", input))
                {
                    int a = findAtByWord(index, findingStr, text);
                    errorHandler(a, 0);
                    if(a >= -1)
                    {
                        char temp[1024];
                        sprintf(temp, "%d\n", a);
                        strcat(printingResult, temp);
                    }
                    printf("%s", printingResult);
                }
                else if(!wizard(" =D ", input))
                {
                    int a = findAtByWord(index, findingStr, text);
                    errorHandler(a, 0);
                    if(a >= -1)
                    {
                        char temp[1024];
                        sprintf(temp, "%d\n", a);
                        strcat(printingResult, temp);
                    }
                    Arman();
                }
                else
                    goto invalid;
            }

            else if(!wizard(" --all --byword\n", input))
            {
                int newlineFlag;
                for (int i = 1; ; ++i)
                {
                    int res = findAtByWord(i, findingStr, text);
                    errorHandler(res, 0);
                    newlineFlag = res >= -1;
                    if(-1 < res)
                    {
                        if(i != 1)
                            strcat(printingResult, ", ");
                        char temp[1024];
                        sprintf(temp, "%d\n", res);
                        strcat(printingResult, temp);
                        res = findAtByWord(i, findingStr, text);
                    }
                    else
                        break;
                }
                if(newlineFlag)
                    strcat(printingResult, "\n");
                printf("%s", printingResult);
            }

            else if(!wizard(" --all --byword =D ", input))
            {
                int newlineFlag;
                for (int i = 1; ; ++i)
                {
                    int res = findAtByWord(i, findingStr, text);
                    errorHandler(res, 0);
                    newlineFlag = res >= -1;
                    if(-1 < res)
                    {
                        if(i != 1)
                            strcat(printingResult, ", ");
                        char temp[1024];
                        sprintf(temp, "%d\n", res);
                        strcat(printingResult, temp);
                        res = findAtByWord(i, findingStr, text);
                    }
                    else
                        break;
                }
                if(newlineFlag)
                    strcat(printingResult, "\n");
                Arman();
            }

            else if(!wizard(" --all\n", input))
            {
                int newlineFlag;
                for (int i = 1; ; ++i)
                {
                    int res = findAt(i, findingStr, text);
                    errorHandler(res, 0);
                    newlineFlag = res >= -1;
                    if(-1 < res)
                    {
                        if(i != 1)
                            strcat(printingResult, ", ");
                        char temp[1024];
                        sprintf(temp, "%d\n", res);
                        strcat(printingResult, temp);
                        res = findAt(i, findingStr, text);
                    }
                    else
                        break;
                }
                if(newlineFlag)
                    strcat(printingResult, "\n");
                printf("%s", printingResult);
            }

            else if(!wizard(" --all =D ", input))
            {
                int newlineFlag;
                for (int i = 1; ; ++i)
                {
                    int res = findAt(i, findingStr, text);
                    errorHandler(res, 0);
                    newlineFlag = res >= -1;
                    if(-1 < res)
                    {
                        if(i != 1)
                            strcat(printingResult, ", ");
                        char temp[1024];
                        sprintf(temp, "%d\n", res);
                        strcat(printingResult, temp);
                        res = findAt(i, findingStr, text);
                    }
                    else
                        break;
                }
                if(newlineFlag)
                    strcat(printingResult, "\n");
                Arman();
            }
            else
                goto invalid;
        }
        
        
        else if(!wizard("replace ", input))
        {

            if(wizard("--str1 ", input))
            {
                goto invalid;
            }

            if(extractString())
            {
                goto invalid;
            }

            char replacing[1024];
            strcpy(replacing, text);

            if(wizard(" --str2 ", input))
            {
                goto invalid;
            }

            if(extractString())
            {
                goto invalid;
            }

            char replacement[1024];
            strcpy(replacement, text);


            if(wizard(" --file ", input))
            {
                goto invalid;
            }

            if(extractString())
            {
                goto invalid;
            }
            if(!wizard("\n", input))
            {
                history(text);
                int a;
                if((a = replace(replacing, replacement, 1, text)) == 0)
                    submitHistory(text);
                errorHandler(a, 1);
                printf("%s", printingResult);
            }
            else if(!wizard(" =D ", input))
            {
                history(text);
                int a;
                if((a = replace(replacing, replacement, 1, text)) >= -1)
                    submitHistory(text);
                if(a == -1)
                    a = 0;
                errorHandler(a, 1);
                Arman();
            }
            else if(!wizard(" --at ", input))
            {
                int index = numExtract();
                if(!wizard("\n", input))
                {
                    history(text);
                    int a;
                    if((a = replace(replacing, replacement, index, text)) == 0)
                        submitHistory(text);
                    errorHandler(a, 1);
                    printf("%s", printingResult);
                }
                else if(!wizard(" =D ", input))
                {
                    history(text);
                    int a;
                    if((a = replace(replacing, replacement, index, text)) == 0)
                        submitHistory(text);
                    errorHandler(a, 1);
                    Arman();
                }
                else
                {
                    goto invalid;
                }
            }
            else if(!wizard(" --all", input))
            {
                
                if(!wizard("\n", input)) {
                    history(text);
                    int count = 0;
                    for (;; ++count) {
                        int a = findAt(count + 1, replacing, text);
                        if (a == -1) {
                            break;
                        }
                        if (a < -1) {
                            errorHandler(a, 1);
                            printf("%s", printingResult);
                            goto restart;
                        }
                    }
                    int range = findAt(count, replacing, text) + 1;
                    for (count; count > 0; count--) {
                        int a = findAt(count, replacing, text);
                        if (a >= range || a == -1)
                            continue;
                        replace(replacing, replacement, count, text);
                    }
                    submitHistory(text);
                    printf("Success!\n");
                }
                
                else if(!wizard(" =D ", input)) {
                    history(text);
                    int count = 0;
                    for (;; ++count) {
                        int a = findAt(count + 1, replacing, text);
                        if (a == -1) {
                            break;
                        }
                        if (a < -1) {
                            errorHandler(a, 1);
                            Arman();
                            goto restart;
                        }
                    }
                    int range = findAt(count, replacing, text) + 1;
                    for (count; count > 0; count--) {
                        int a = findAt(count, replacing, text);
                        if (a >= range || a == -1)
                            continue;
                        replace(replacing, replacement, count, text);
                    }
                    submitHistory(text);
                    errorHandler(0, 1);
                    Arman();
                }
            
                else
                    goto invalid;
            }
            else
                goto invalid;
        }


        else if(!wizard("tree ", input))
        {
            int depth = numExtract();
            if(!wizard("\n", input))
            {
                strcat(printingResult, "root\n");
                tree(1, 0, depth);
                printf("%s", printingResult);
            }
            else if(!wizard(" =D ", input))
            {
                strcat(printingResult, "root\n");
                tree(1, 0, depth);
                Arman();
            }
            else
                goto invalid;
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
                    if(!wizard("\n", input))
                    {
                        for (int i = 0; ; ++i) {
                            if(findAt(i + 1, searchingSTR, text) == -1)
                            {
                                char temp[1024];
                                sprintf(temp, "%d\n", i);
                                strcat(printingResult, temp);
                                break;
                            }
                        }
                        printf("%s", printingResult);
                    }
                    else if(!wizard(" =D ", input))
                    {
                        for (int i = 0; ; ++i) {
                            if(findAt(i + 1, searchingSTR, text) == -1)
                            {
                                char temp[1024];
                                sprintf(temp, "%d\n", i);
                                strcat(printingResult, temp);
                                break;
                            }
                        }
                        Arman();
                    }
                    else
                        goto invalid;
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
                            } else if (a <= -2)
                                goto end;
                        }
                        if(!wizard("\n", input))
                        {
                            char temp[1024];
                            sprintf(temp, "%d\n", result);
                            strcat(printingResult, temp);
                            printf("%s", printingResult);
                            break;
                        }
                        else if(!wizard(" =D ", input))
                        {
                            char temp[1024];
                            sprintf(temp, "%d\n", result);
                            strcat(printingResult, temp);
                            Arman();
                            break;
                        }
                        wizard(" ", input);
                    }
                    end:;
                }
                else
                {
                    goto invalid;
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
                    if(!wizard("\n", input))
                    {
                        for (int i = 0; ; ++i) {
                            int a;
                            if((a = findAt(i + 1, searchingSTR, text)) <= -1)
                            {
                                if(i != 0)
                                {
                                    char temp[1024];
                                    sprintf(temp, "%s\n", currentFile);
                                    strcat(printingResult, temp);
                                }
                                errorHandler(a, 0);
                                printf("%s", printingResult);
                                break;
                            }
                        }
                    }
                    else if(!wizard(" =D ", input))
                    {
                        for (int i = 0; ; ++i) {
                            int a;
                            if((a = findAt(i + 1, searchingSTR, text)) <= -1)
                            {
                                if(i != 0)
                                {
                                    char temp[1024];
                                    sprintf(temp, "%s\n", currentFile);
                                    strcat(printingResult, temp);
                                }
                                errorHandler(a, 0);
                                Arman();
                                break;
                            }
                        }
                    }
                    else
                        goto invalid;
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
                                if(i != 0) {
                                    char temp[1024];
                                    sprintf(temp, "%s\n", currentFile);
                                    strcat(printingResult, temp);
                                }
                                break;
                            } else if (a <= -2)
                                goto end1;
                        }
                        if(!wizard("\n", input)) {
                            printf("%s", printingResult);
                            break;
                        }
                        else if(!wizard(" =D ", input)) {
                            Arman();
                            break;
                        }
                        wizard(" ", input);
                    }
                    end1:;
                }
                else
                {
                    goto invalid;
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
                    if(!wizard("\n", input))
                    {
                        for (int i = 0; ; ++i) {
                            int a;
                            if((a = findAt(i + 1, searchingSTR, text)) >= 0)
                            {

                                positionFinder(a, text);
                                catLine(text, calculatedLine);
                            }
                            else
                                break;
                        }
                        printf("%s", printingResult);
                    }
                    else if(!wizard(" =D ", input))
                    {
                        for (int i = 0; ; ++i) {
                            int a;
                            if((a = findAt(i + 1, searchingSTR, text)) >= 0)
                            {

                                positionFinder(a, text);
                                catLine(text, calculatedLine);
                                break;
                            }
                        }
                        Arman();
                    }
                    else
                        goto invalid;
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
                            if ((a = findAt(i + 1, searchingSTR, text)) >= 0) {
                                char temp[1024];
                                sprintf(temp, "%s: ", currentFile);
                                strcat(printingResult, temp);
                                positionFinder(a, text);
                                catLine(text, calculatedLine);
                            } 
                            else
                                break;
                        }
                        if(!wizard("\n", input))
                        {
                            printf("%s", printingResult);
                            break;
                        }
                        else if(!wizard(" =D ", input))
                        {
                            Arman();
                            break;
                        }

                        wizard(" ", input);
                    }
                    end2:;
                }
                else
                {
                    goto invalid;
                }
            }
        }


        else if(!wizard("auto-indent ", input))
        {
            extractString();
            if(!wizard("\n", input))
            {
                history(text);
                int a;
                if((a = autoIndent(text)) == 0)
                    submitHistory(text);
                errorHandler(a, 1);
                printf("%s", printingResult);
            }
            else if(!wizard(" =D ", input))
            {
                history(text);
                int a;
                if((a = autoIndent(text)) == 0)
                    submitHistory(text);
                errorHandler(a, 1);
                Arman();
            }
            else
                goto invalid;
        }


        else if(!wizard("undo ", input))
        {
            extractString();
            if(!wizard("\n", input))
            {
                int a = undo(text);
                errorHandler(a, 1);
                printf("%s", printingResult);
            }
            else if(!wizard(" =D ", input))
            {
                int a = undo(text);
                errorHandler(a, 1);
                Arman();
            }
            else
                goto invalid;
        }


        else if(!wizard("compare ", input))
        {
            extractString();
            char file1[1024];
            strcpy(file1, text);
            if(wizard(" ", input))
            {
                goto invalid;
            }
            extractString();
            if(!wizard("\n", input))
            {
                int a = compare(file1, text);
                errorHandler(a, 0);
                printf("%s", printingResult);
            }
            if(!wizard(" =D ", input))
            {
                int a = compare(file1, text);
                errorHandler(a, 0);
                Arman();
            }
            else
                goto invalid;
        }


        else
        {
            invalid:
            strcat(printingResult, "Invalid input!\n");
            while (1)
            {
                if (strlen(input) == 0) {
                    printf("%s", printingResult);
                    break;
                }
                if (!wizard("=D ", input)) {
                    Arman();
                    break;
                } else
                    countAndDelete(input);
            }
        }
    }
}