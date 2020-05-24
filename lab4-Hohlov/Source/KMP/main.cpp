#include <iostream>
#include <vector>
#include <string>
#include <fstream>


void prefixFunction (std::vector<int>& pi, const std::string& string) {        //Заполнение массива значениями префикс-функций
    std::cout << "-----------------------------------------"<<std::endl;
    std::cout << "Calculating prefix function"<<std::endl<<std::endl;
    pi[0] = 0;
    int j = 0;            //Вспомогательный указатель на индекс
    int i = 1;            //Текущий указатель на индекс в строке

    while(i < string.size()){       //Пока не пройдены все символы строки-поиска

        for (char k : string)           //Вывод промежуточных результатов
            std::cout << k << " ";
        std::cout << std::endl;
        for (int k : pi)
            std::cout << k <<" ";
        std::cout << std::endl;
        for (int k=0; k<j; k++)
            std::cout << "  ";
        std::cout << "j ";
        for (int k=0; k<i-j-1; k++)
            std::cout << "  ";
        std::cout << "i " << std::endl;

        if (string[i] == string[j]){     //Если символы i и j индекса совпадают, то оба смещаются вправо, изменяется массив префикс-функций
            pi[i] = j+1;
            i++;
            j++;
            std::cout << "s[i] == s[j],  i++,  j++,  pi[i] = " << j << std::endl << std::endl;
        }
        else{
            if (j==0){                  //Иначе, если j указывает на начальный символ строки, то на i символ значение префикс-функции 0
                pi[i] = 0;
                i++;                    //i-тый указатель смещается вправо
                std::cout << "s[i] != s[j],  i++,  pi[i] = 0" << std::endl << std::endl;
            }
            else{
                j = pi[j-1];            //Если указатель j не указывает на начальный символ, то сравниваются префиксы и суффиксы меньшего размера
                std::cout << "s[i] != s[j],  j = " << pi[j] << std::endl << std::endl;
            }
        }

    }


    std::cout << "\nPrefix function for string:\n";             //Вывод строки со значением префикс-функций каждого символа
    for (char k : string)
        std::cout << k << " ";
    std::cout << std::endl;
    for (int i=0; i<string.size(); i++)
        std::cout << pi[i] << " ";
    std::cout <<"\n\n";
    std::cout << "-----------------------------------------"<<std::endl;
}


std::vector<int> KMP(std::ifstream& input, const std::string& pattern, const std::string& text) {   //Функция, реализующая алгоритм Кнута-Морриса-Пратта
    std::vector<int> result;                                //Вектор индексов вхождения подстроки
    std::vector<int> pi(pattern.size());                    //Вектор значений префикс-функций

    prefixFunction(pi, pattern);                         //Заполнение вектора префикс-функций
    std::cout << "Finding substring in the string"<<std::endl<<std::endl;

    int k = 0;                          //Указатель на текущий элемент в строке-шаблоне
    int l = 1;                          //Счетчик символов в строке поиска
    int i = 0;

    while (i < text.size()) {
        char ch = text[i++];                   //Считывается очередной символ
        std::cout << "Read symbol: " << ch << std::endl;   //Вывод промежуточных значений
        for (char i : pattern)
            std::cout << i << " ";
        std::cout << std::endl;
        for (int i : pi)
            std::cout << i <<" ";
        std::cout << std::endl;
        for (int i=0; i<k; i++)
            std::cout << "  ";
        std::cout << "k" << std::endl;


        while ((k > 0) && (pattern[k] != ch)) {             //Если счтанный символ не удовлетворяет условию, то изменяется указатель k
            k = pi[k-1];

            std::cout << "\nread symbol != pattern[k], k = pi[k-1] = " << k << std::endl;
            for (char i : pattern)
                std::cout << i << " ";
            std::cout << std::endl;
            for (int i : pi)
                std::cout << i <<" ";
            std::cout << std::endl;
            for (int i=0; i<k; i++)
                std::cout << "  ";
            for (int i=0; i<k-1; i++)
                std::cout << "  ";
            std::cout << "k" << std::endl;
        }

        if (pattern[k] == ch) {         //Если считанный символ совпадает с текущим в строке-шаблоне, то указатель k сдвигается вправо
            k++;
            std::cout << "read symbol == pattern[k], k++" << std::endl << std::endl;
        }
        else{
            std::cout << "read symbol != pattern[k]"<< std::endl <<std::endl;
        }

        if (k == pattern.size()) {          //Если найдена подстрока, то индекс заносится в массив результата
            result.push_back(l - pattern.size());
            std::cout << "Found string entry in index: " << l - pattern.size() << std::endl<< std::endl;
        }
        l++;
        std::cout << "---------------------------------------" << std::endl;
    }

    if (result.empty()) {                   //Если не было найдено подстроки
        result.push_back(-1);
    }

    return result;
}


int main() {
    std::ifstream input;
    input.open("test.txt");
    std::string pattern;
    std::string text;
    input >> pattern >> text;
    auto result = KMP(input, pattern, text);          //Запуск алгоритма Кнута-Морриса-Пратта

    std::cout << "\n\nResult of algorithm work:" << std::endl;     //Вывод результата
    if (result[0] == -1)
        std::cout << "Substring not found" << std::endl;
    else {
        for (int i = 0; i < result.size(); i++) {
            std::cout << result[i];
            if (i != result.size() - 1) {
                std::cout << ',';
            }
        }
    }

    input.close();

    return 0;
}