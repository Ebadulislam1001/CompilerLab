// sample input for a CFG
// A CB#    // LHS RHS#
// B +CB#
// B #
// C ED#
// D *ED#
// D #
// E (A)#
// E a#     // '#' stands for Epsilon

#include <bits/stdc++.h>
using namespace std;

// characteristics of a CFG             // (global variables)
char startSymbol = '#';                 // startSymbol of the grammar
vector<char> V;                         // set of variables
vector<char> T;                         // set of terminals
vector<pair<char, string>> productions; // vector of pairs where: first is LHS, second is RHS
vector<vector<int>> parseTable;         // LL(1) Parse Table

int index(vector<char> V, char c)
{
    for (int i = 0; i < V.size(); i++)
    {
        if (V[i] == c)
        {
            return i;
        }
    }
    return -1;
}

int main()
{
    char del, var, sym;
    int num, n;
    FILE *filePointer;
    // reading the grammar from txt file
    filePointer = fopen("5_CFG.txt", "r");
    do
    {
        // reading a production
        char LHS;
        string RHS;
        // reading LHS
        fscanf(filePointer, "%c%*c", &LHS);
        // setting first variable that appears as the startSymbol
        if (startSymbol == '#')
            startSymbol = LHS;
        // reading RHS
        do
        {
            fscanf(filePointer, "%c", &del);
            RHS.push_back(del);
        } while (del != '#');
        // pushing LHS & RHS as a pair
        productions.push_back({LHS, RHS});
        // checking for EOF
        fscanf(filePointer, "%c", &del);
    } while (del == '\n');
    fclose(filePointer);

    // printing all the productions
    printf("\nCFG PRODUCTIONS\n\n");
    for (int i = 0; i < productions.size(); i++)
    {
        // first refers to LHS, second refers to RHS
        printf("%02d. %c -> %s\n", i + 1, productions[i].first, productions[i].second.c_str());
    }

    // reading the Parse Table from txt file
    filePointer = fopen("6_LL_ParseTable.txt", "r");
    fscanf(filePointer, "%d", &n);
    do
    {
        fscanf(filePointer, "%*c%c", &del);
        T.push_back(del);
    } while (del != '$');

    for (int i = 0; i < n; i++)
    {

        fscanf(filePointer, "%*c%c", &del);
        V.push_back(del);
        vector<int> row;
        for (int j = 0; j < 6; j++)
        {
            fscanf(filePointer, "%d", &num);
            row.push_back(num);
        }
        parseTable.push_back(row);
    }
    fclose(filePointer);

    // printing the parse table
    printf("\nLL(1) PARSE TABLE\n\n");
    for (int j = 0; j < T.size(); j++)
    {
        printf("\t%c", T[j]);
    }
    for (int i = 0; i < V.size(); i++)
    {
        printf("\n%c", V[i]);
        for (int j = 0; j < T.size(); j++)
        {
            printf("\t%d", parseTable[i][j]);
        }
    }

    while (1)
    {
        string input;
        string stack;
        bool accepted = true;

        // getting the input string
        printf("\n\nEnter Input String:\t");
        getline(cin, input);
        input.push_back('$');

        // checking the exit condition
        if (input[0] == '!')
        {
            printf("\nHave a nice day!");
            return 0;
        }
        // String traversal starts here
        stack.push_back(startSymbol);
        stack.push_back('$');

        printf("\n=====INPUT===========STACK===========REMARKS=====\n");
        while (input.size() > 0 && stack.size() > 0)
        {
            // printing the current state of the input and the stack
            printf("\n%10s\t%10s", input.c_str(), stack.c_str());
            // reversing the two strings for better functioning
            reverse(input.begin(), input.end());
            reverse(stack.begin(), stack.end());

            if (isupper(stack.back()))
            {
                int x = index(V, stack.back());
                int y = index(T, input.back());
                if (x >= 0 && y >= 0 && parseTable[x][y] > 0)
                {
                    int p = parseTable[x][y] - 1;
                    string RHS = productions[p].second;
                    // printing the remark
                    printf("\t %c->%s", productions[p].first, RHS.c_str());
                    RHS.pop_back(); // to remove the # from the end
                    stack.pop_back();
                    for (int s = RHS.size(); s > 0; s--)
                    {
                        stack.push_back(RHS[s - 1]);
                    }
                }
                else
                {
                    accepted = false;
                    break;
                }
            }
            else
            {
                if (input.back() == stack.back())
                {
                    string remarks;
                    remarks.push_back(input.back());
                    remarks += " = ";
                    remarks.push_back(stack.back());
                    printf("\t%12s", remarks.c_str());
                    input.pop_back();
                    stack.pop_back();
                }
                else
                {
                    accepted = false;
                    break;
                }
            }
            // reversing the two strings back to normal
            reverse(input.begin(), input.end());
            reverse(stack.begin(), stack.end());
        }
        if (accepted == false || input.size() > 0 || stack.size() > 0)
        {
            printf("\n\nThe input string is rejected\n");
        }
        else
        {
            printf("\n\nThe input string is accepted\n");
        }
    }

    return 0;
}