#include<iostream>
#include<string>
using namespace std;

void skipChars(FILE *&readPointer, int charCount)
{
    for (int i = 0; i < charCount; i += 1)
        fgetc(readPointer);
}
void skipLines(FILE *&readPointer, int lineCount)
{
    for (int i = 0; i < lineCount; i += 1)
    {
        char line[1000];
        fscanf(readPointer, "%[^\n]", line);
        fgetc(readPointer);
    }
}
int readInt(FILE *&readPointer)
{
    int ans = 0;
    fscanf(readPointer, "%d", &ans);
    return ans;
}
string readString(FILE *&readPointer, char delim)
{
    string ans = "";
    char ch;
    while (true)
    {
        fscanf(readPointer, "%c", &ch);
        if (ch == delim)
            break;
        ans.push_back(ch);
    }
    return ans;
}