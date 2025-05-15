/*
#include <iostream>
#include <fstream>
#include <conio.h>
#include <vector>

using namespace std;

class node_ch
{
public:
    char data;
    node_ch *after;
    node_ch *before;
    node_ch *above;
    node_ch *below;

    node_ch(char c) : data(c), after(nullptr), before(nullptr), above(nullptr), below(nullptr) {}
};

class node_line
{
public:
    node_ch *first;
    node_line *last;
    node_line *beforeLine;

    node_line() : first(nullptr), last(nullptr), beforeLine(nullptr) {}
};

class cursor
{
public:
    node_line *line;
    node_ch *charr;

    cursor() : line(nullptr), charr(nullptr) {}
};

class notepad
{
private:
    node_line *first;
    cursor cursor;

public:
    notepad() : first(nullptr)
    {
        first = new node_line();
        cursor.line = first;
        cursor.charr = nullptr;
    }

    void add_ch(char c)
    {
        node_ch *newNode = new node_ch(c);

        if (cursor.charr)
        {
            newNode->after = cursor.charr->after;
            cursor.charr->after = newNode;
            newNode->before = cursor.charr;
            if (newNode->after)
            {
                newNode->after->before = newNode;
            }
        }
        else
        {
            cursor.line->first = newNode;
        }

        cursor.charr = newNode;
    }

    void add_line()
    {
        node_line *newLine = new node_line();
        if (cursor.line)
        {
            newLine->last = cursor.line->last;
            cursor.line->last = newLine;
            newLine->beforeLine = cursor.line;
            if (newLine->last)
            {
                newLine->last->beforeLine = newLine;
            }
        }

        cursor.charr = nullptr;
        cursor.line = newLine;
    }

    void back_del()
    {
        if (cursor.charr)
        {
            node_ch *before = cursor.charr->before;
            node_ch *after = cursor.charr->after;

            if (before)
            {
                before->after = after;
            }

            if (after)
            {
                after->before = before;
            }

            if (cursor.charr == cursor.line->first)
            {
                cursor.line->first = after;
            }

            delete cursor.charr;
            cursor.charr = before;
        }
    }

    void right()
    {
        if (cursor.charr)
        {
            cursor.charr = cursor.charr->after;
        }
        else
        {
            cursor.charr = cursor.line->first;
        }
    }

    void left()
    {
        if (cursor.charr)
        {
            cursor.charr = cursor.charr->before;
        }
        else if (cursor.line->beforeLine)
        {
            cursor.line = cursor.line->beforeLine;
            cursor.charr = cursor.line->first;
        }
    }

    void up()
    {
        if (cursor.line->beforeLine)
        {
            cursor.line = cursor.line->beforeLine;
            if (cursor.charr)
            {
                node_ch *node_ch = cursor.line->first;
                while (node_ch->after && node_ch->after != cursor.charr)
                {
                    node_ch = node_ch->after;
                }
                cursor.charr = node_ch;
            }
            else
            {
                cursor.charr = cursor.line->first;
            }
        }
    }

    void down()
    {
        if (cursor.line->last)
        {
            cursor.line = cursor.line->last;
            if (cursor.charr)
            {
                node_ch *node_ch = cursor.line->first;
                while (node_ch->after && node_ch->after != cursor.charr)
                {
                    node_ch = node_ch->after;
                }
                cursor.charr = node_ch;
            }
            else
            {
                cursor.charr = cursor.line->first;
            }
        }
    }

    void display()
    {
        node_line *line = first;
        while (line)
        {
            node_ch *node_ch = line->first;
            while (node_ch)
            {
                if (cursor.line == line && cursor.charr == node_ch)
                {
                    cout << '|'; // cursor
                }
                cout << node_ch->data;
                node_ch = node_ch->after;
            }
            cout << endl;
            line = line->last;
        }
    }

    void toFile(const string &files)
    {
        ofstream file;
        file.open(files);

        if (file.is_open())
        {
            node_line *line = first;
            while (line)
            {
                node_ch *node_ch = line->first;
                while (node_ch)
                {
                    file << node_ch->data;
                    node_ch = node_ch->after;
                }
                file << endl;
                line = line->last;
            }
            file.close();
        }
        else
        {
            cout << "Failed to open the file." << endl;
        }
    }

    void loadFile(const string &files)
    {
        ifstream file;
        file.open(files);

        if (file.is_open())
        {
            first = new node_line();
            cursor.line = first;
            cursor.charr = nullptr;

            string text;
            while (getline(file, text))
            {
                for (char c : text)
                {
                    add_ch(c);
                }
                if (!file.eof())
                {
                    add_line();
                }
            }
            file.close();
        }
        else
        {
            cout << "Failed to open the file." << endl;
        }
    }
};

int main()
{
    notepad n1;
    char ch;

    vector<string> dictionary;

    // Import dictionary words
    ifstream dict("dictionary.txt");
    if (dict.is_open())
    {
        string word;
        while (getline(dict, word))
        {
            dictionary.push_back(word);
        }
        dict.close();
    }
    else
    {
        cout << "Failed to open file" << endl;
    }

    cout << "Welcome to NotePad\n(Enter 'Ctrl + S' to save, 'Ctrl + L' to load,and 'Esc' to exit)" << endl;

    while (true)
    {
    back:
        ch = _getch(); // no need to press enter

        if (ch == 13)
        {
            n1.add_ch('\n'); // Add newline
        }
        else if (ch == 8)
        {
            n1.back_del();
        }
        else if (ch == 19) // 'Ctrl + S'
        {
            string files = "notepad.txt";
            n1.toFile(files);
            cout << "File saved.\n";
            goto back;
        }
        else if (ch == 12) // 'Ctrl + L'
        {
            string filename = "notepad.txt";
            n1.loadFile(filename);
            system("cls");
            cout << "File loaded.\n";
            goto back;
        }
        else if (ch == 27) // 'Esc'
        {
            cout << "Program ended\n";
            break;
        }
        else if (ch == 72) // Up arrow key
        {
            n1.up();
        }
        else if (ch == 80) // Down arrow key
        {
            n1.down();
        }
        else if (ch == 75) // Left arrow key
        {
            n1.left();
        }
        else if (ch == 77) // Right arrow key
        {
            n1.right();
        }
        else
        {
            if (ch != 0) // Ignore special keys
            {
                n1.add_ch(ch);
            }
        }

        system("cls");
        n1.display();
    }

    return 0;
}
*/