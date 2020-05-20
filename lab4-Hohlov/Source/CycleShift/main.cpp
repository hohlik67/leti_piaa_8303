#include <iostream>
#include <fstream>


int* prefixFunction(char* string, int size) {                        //Заполнение массива значениями префикс-функций
    std::cout << "-----------------------------------------"<<std::endl;
    std::cout << "Calculating prefix function"<<std::endl<<std::endl;
    int* pi = new int[size];                    //Массив со значениями префикс функций
    for (int i=0; i<size; i++)
        pi[i] = 0;
    int j = 0;                               //Вспомогательный указатель на индекс
    int i = 1;                               //Текущий указатель на индекс в строке

    while(i < size) {                           //Пока не пройдены все символы строки-поиска

        for (int k = 0; k<size; k++)               //Вывод промежуточных результатов
            std::cout << string[k] << " ";
        std::cout << std::endl;
        for (int k = 0; k<size; k++)
            std::cout << pi[k] <<" ";
        std::cout << std::endl;
        for (int k=0; k<j; k++)
            std::cout << "  ";
        std::cout << "j ";
        for (int k=0; k<i-j-1; k++)
            std::cout << "  ";
        std::cout << "i " << std::endl;

        if (string[i] == string[j]){                    //Если символы i и j индекса совпадают, то оба смещаются вправо, изменяется массив префикс-функций
            pi[i] = j+1;
            i++;
            j++;
            std::cout << "s[i] == s[j],  i++,  j++,  pi[i] = " << j << std::endl << std::endl;
        }
        else{
            if (j==0){                                 //Иначе, если j указывает на начальный символ строки, то на i символ значение префикс-функции 0
                pi[i] = 0;
                i++;                                    //i-тый указатель смещается вправо
                std::cout << "s[i] != s[j],  i++,  pi[i] = 0" << std::endl << std::endl;
            }
            else{
                j = pi[j-1];                            //Если указатель j не указывает на начальный символ, то сравниваются префиксы и суффиксы меньшего размера
                std::cout << "s[i] != s[j],  j = " << pi[j] << std::endl << std::endl;
            }
        }
    }

    std::cout << "\nPrefix function for string:\n";         //Вывод строки со значением префикс-функций каждого символа
    for (int i=0; i<size; i++)
        std::cout << string[i] << " ";
    std::cout << std::endl;
    for (int i=0; i<size; i++)
        std::cout << pi[i] << " ";
    std::cout <<"\n\n";
    std::cout << "-----------------------------------------"<<std::endl;

    return pi;
}


void cyclicShift(const std::string& firstString, const std::string& secondString) {    //Функция поиска циклического сдвига
    if (firstString.size() != secondString.size()) {                //Проверка на равенство длины двух строк
        std::cout << -1;                                            //Если не равны, то одна не может быть циклическим сдвигом другой
        return;
    }

    int size = firstString.size();

    char* buff = new char[3 * size];                        //Создание вспомогательной строки для поиска результата

    int i = 0;
    for (; i < size; ++i) {
        buff[i] = secondString[i];                          //Заносится первая введенная строка
    }

    for (int k = 0; k < 2; ++k) {                           //Далее дважды заносится вторая введенная строка
        for (int j = 0; j < size; ++j) {
            buff[i++] = firstString[j];
        }
    }

    int* pi = prefixFunction(buff, size * 3);           //Получается массив их префикс функций

    std::cout << "Find cyclic shift" << std::endl<<std::endl;
    for (int i = 2 * size - 1; i < 3 * size; ++i) {         //Обходится вспомогательная строка

        for (int k = 0; k<size*3; k++)               //Вывод промежуточных результатов
            std::cout << buff[k] << " ";
        std::cout << std::endl;
        for (int k = 0; k<size*3; k++)
            std::cout << pi[k] <<" ";
        std::cout << std::endl;
        for (int k=0; k<i; k++)
            std::cout << "  ";
        std::cout << "i ";

        if (pi[i] == size) {                                //Если значение префикс функции на символе равно размеру введенной строки, то найден циклический сдвиг
            std::cout << "\nFound cyclic shift!!! Size = " << size << ", pi[i] = " <<pi[i] <<std::endl;
            std::cout <<"Cyclic shift index: "<< i + 1 - 2 * size;    //Вывод результата
            delete [] buff;
            delete [] pi;
            return;
        }

        else{
            std::cout << "\nCyclic shift still not found, size = " << size << ", pi[i] = " <<pi[i] << ", size != pi[i]"<<std::endl;
            std::cout << "Go next, i++" << std::endl << std::endl;
        }
    }

    std::cout << -1;                            //Если циклический сдвиг не найден
    delete [] buff;
    delete [] pi;
}


int main() {
    std::ifstream input;
    input.open("test.txt");
    std::string firstString;
    std::string secondString;

    input >> firstString >> secondString;       //Считывание двух строк, для поиска циклического сдвига

    cyclicShift(firstString, secondString);  //Запуск алгоритма поиска циклического сдвига

    return 0;
}