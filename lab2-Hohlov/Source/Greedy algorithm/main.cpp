#include <iostream>
#include <vector>
#include <map>
#include <fstream>

std::ifstream in;

class Graph{                                 //Класс для работы с графом
private:
    std::map<char, std::vector<std::pair<char, double>>> graph;                       //Хранение пар вершин с расстоянием между ними
    std::map<char, std::vector<std::pair<char, double>>>::iterator graph_iterator;    //Итератор вершин графа

public:

    void fillingGraph() {                       //Заполнение графа
        while (true) {
            char vertex1;                   //Пара вершин
            char vertex2;                   //и расстояние
            double weight;                  //между ними
            if (in >> vertex1 && in >> vertex2 >> weight){
                graph[vertex1].push_back(std::pair<char, double>(vertex2, weight));
                continue;
            }
            break;
        }
    }

    void runSearch(char start, char finish){                    //Функция жадного алгоритма
        std::map<char,bool> visitedVertexs;                     //Посещенные вершины
        std::map<char,char> way;                                  //Контейнер путей
        std::vector<std::pair<char, double>> stackPath;              //Стэк для хранения вершин

        std::pair<char, double> current = std::pair<char, double>(start, 0);

        for(graph_iterator = graph.begin(); graph_iterator!=graph.end(); graph_iterator++)
            visitedVertexs[graph_iterator->first]=false;

        while(true){

            if(current.first == finish){                                 //Пока текущая вершина не будет конечной выполняется цикл
                std::cout << "--------------------------------"<<std::endl;
                std::cout << "The algorithm has finished work";
                break;
            }


            std::cout << "-----------------------------------" << std::endl;
            std::cout << "Current vertex: " << current.first << std::endl<<std::endl;
            std::vector<std::pair<char, double>> neighbours;                //Вектор соседей текущей вершины
            visitedVertexs[current.first]=true;

            for(std::pair<char,double> neighbor : graph[current.first]) {
                if (!visitedVertexs[neighbor.first])
                    neighbours.push_back(neighbor);                         //Заполнение вектора соседних вершин
            }

            std::cout << "Next neighbors of vertex " << current.first << " and path to their" << ":" << std::endl;
            if (neighbours.size() == 0)
                std::cout << "Empty"<<std::endl;

            for (int i=0; i<neighbours.size(); i++){
                std::cout <<"\t"<< neighbours[i].first << " --> " << neighbours[i].second << std::endl;
            }
            std::cout << std::endl;

            if (neighbours.size() == 0){                                    //Если зашли в тупик, то возвращаемся в предыдущую вершину
                current = stackPath[stackPath.size()-1];
                std::cout << "Back to the: " << current.first << std::endl;
                stackPath.pop_back();
            }

            else {
                stackPath.push_back(current);                               //Иначе переходим в новую вершину, старую кладем на стэк
                current = neighbours[minimal_element(neighbours)];
                std::cout << "Go to the " << current.first << std::endl;
                way[current.first] = stackPath[stackPath.size()-1].first;
            }

        }


        std::vector<char> path;       //Вектор для хранения пути вершин

        char vertex = finish;               //Идем с конца наперед, восстанавливая путь, пока не придем в начало
        while (vertex != start ){
            path.push_back(vertex);
            vertex = way.find(vertex)->second;

        }
        path.push_back(vertex);


        std::cout<< std::endl <<"Result of algorithm: ";
        for (int i=path.size()-1; i>=0; i--)
            std::cout << path[i] << " ";                //Печать пути
        std::cout << std::endl;
    }

private:

    int minimal_element(std::vector<std::pair<char, double>> neighbours){  //Функция нахождения минимального элемента в векторе соседей
        double min_value = neighbours[0].second;
        int min = 0;
        for (int i = 1; i<neighbours.size(); i++){
            if (neighbours[i].second < min_value) {         //Если находится элемент меньше минимально, то он становится минимальным
                min_value = neighbours[i].second;
                min = i;
            }
        }
        return min;                                  //Возвращается индекс минимального элемента
    }

};

int main() {
    in.open("test.txt");
    char start, finish;
    in >> start;
    in >> finish;
    Graph graph;                                       //Создание графа
    graph.fillingGraph();                              //Заполнение графа
    graph.runSearch(start,finish);                     //Поиск кратчайшего пути
    return 0;
}
