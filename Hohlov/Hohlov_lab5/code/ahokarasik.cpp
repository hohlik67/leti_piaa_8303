#include <iostream>
#include <unordered_map>
#include <map>
#include <set>
#include <queue>
#include <string>
#define DBG
using namespace std;

//структура вершины бора. Хранит символ, ссылки на родителя, на суффикс и на детей,
//а также индекс шаблона. Если строка не терминальная, хранится -1. Также для удобства в терминальных вершинах
//хранится длина шаблона.
struct Vertex {
    char ch;
    Vertex* parent = nullptr;
    Vertex* suffix = nullptr;
    unordered_map<char, Vertex*> child;
    int terminateId = -1;
    int stringSize;
};

//класс бора
class Trie {
    Vertex root;
    map<int, set<int>> foundedStrings;
public:
    //метод для поиска строк в тексте, принимает текст.
    void findStrings(string text) {
        //начинаем поиск от корня бора
        Vertex* tmp = &root;
        for (int i = 0; i < text.length(); i++) {
            //если можем перейти из текущей вершины, то делаем это, иначе пытаемся перейти из суффиксов
            if (tmp->child.find(text[i]) != tmp->child.end()) {
                tmp = tmp->child[text[i]];
                //если вершина терминальная, записываем индекс ее начала в тексте и возвращаемся
                //к корню чтобы избежать пересечения вхождений шаблонов
                if (tmp->terminateId != -1) {
                    foundedStrings[i + 2 - tmp->stringSize].insert(tmp->terminateId);
                    tmp = &root;
                }
            } else {
                //если мы не можем перейти из текущей вершины, проверяем, можем ли перейти из ее суффиксов
                while (tmp->child.find(text[i]) == tmp->child.end() && tmp != &root) {
                    tmp = tmp->suffix;
                }
                if (tmp->child.find(text[i]) != tmp->child.end())
                    i--;
            }
        }
    }
    //метод построения суффиксных ссылок в боре
    void initSuffixRef() {
        Vertex* vert;
        queue<Vertex*> vertexToVisit;
        vertexToVisit.push(&root);
        //запускается поиск в ширину, на каждом шаге для родителя текущей вершины суффиксная ссылка уже построена,
        //а значит остается только проверить можем ли из суффикса родителя перейти по символу текущей вершины,
        //если не можем то уменьшаем суффикс родителя и проверяем так пока не дойдем до корня

        cout << "initialization of suffix links:\n";
        while (!vertexToVisit.empty()) {
            vert = vertexToVisit.front();
            vertexToVisit.pop();
            if (vert == &root) {
                cout << "root:\n";
            } else {
                Vertex* t = vert;
                vector<char> str;
                while (t!=&root) {
                    str.push_back(t->ch);
                    t = t->parent;
                }
                for (auto k = str.rbegin(); k!=str.rend(); k++){
                    cout << *k;
                }
            }
            cout << "\tchilds: ";
            if (vert->child.empty()) {
                cout << "none";
            }
            for (auto &i : vert->child) {
                vertexToVisit.push(i.second);
                cout << i.first;
            }
            Vertex* tmp = vert->parent;
            cout << endl;
            if (vert != &root)
                cout << "\tsuffixLink: ";
            while (tmp) {
                tmp = tmp->suffix;
                if (tmp && tmp->child.find(vert->ch) != tmp->suffix->child.end()) {
                    vert->suffix = tmp->child[vert->ch];
                    if (vert->suffix != &root) {
                        vector<char> suffix;
                        suffix.push_back(vert->ch);
                        while (tmp!=&root) {
                            suffix.push_back(tmp->ch);
                            tmp = tmp->parent;
                        }
                        for (auto k = suffix.rbegin(); k!=suffix.rend(); k++){
                            cout << *k;
                        }
                        cout << endl;
                    }
                    break;
                }
            }
            if (vert->suffix == &root)
                cout << "root\n";
        }
    }
    //метод добавления новой строки в бор
    void insert(const string& newString, int id) {
        cout << "\nadd new string \"" << newString << "\" to trie\n";
        cout << "path in trie : root";
        Vertex* tmp = &root;
        //спускаемся по бору для каждого символа, если вершина не существует, создаем ее
        for (auto i : newString) {
            cout << "->";
            if (tmp->child.find(i) == tmp->child.end()) {
                tmp->child[i] = new Vertex;
                tmp->child[i]->parent = tmp;
                tmp->child[i]->ch = i;
                tmp->child[i]->suffix = &root;
                cout << "(New)";
            }
            tmp = tmp->child[i];

            cout << tmp->ch;
        }

        cout << endl;
        //выставляем для терминальной вершины индекс шаблона и записываем длину шаблона
        tmp->terminateId = id;
        tmp->stringSize = newString.length();
    }
    //метод считывания строк-шаблонов из потока ввода
    void readStrings(){
        int numOfWords;
        vector<string> strings;
        cin >> numOfWords;
        for (int i = 0; i < numOfWords; i++) {
            string newString;
            cin >> newString;
            strings.push_back(newString);
        }
        for (int i = 0; i < numOfWords; i++) {
            insert(strings[i], i + 1);
        }
    }
    //метод для вывода найденных строк в тексте
    void printFoundedStrings() {
        cout << "\nAnswer:\n";
        for (auto i : foundedStrings) {
            for (auto j : i.second) {
                cout << i.first << ' ' << j << '\n';
            }
        }
    }

};

int main() {
    string text;
    cin >> text;
    Trie trie;
    trie.readStrings();
    trie.initSuffixRef();
    trie.findStrings(text);
    trie.printFoundedStrings();
    return 0;
}