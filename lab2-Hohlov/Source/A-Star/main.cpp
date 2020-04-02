#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>

std::ifstream in;

class Graph{                                                        //Класс для работы с графом
private:
    std::map<int, std::vector<std::pair<int, double>>> graph;                               //Хранение пар вершин с расстоянием между ними
    std::map<int, std::vector<std::pair<int,double>>>::iterator graph_iterator;             //Итератор вершин графа

public:

    void fillingGraph() {                           //Заполнение графа
        while (true) {
            int vertex1;                            //Пара вершин
            int vertex2;                            //и расстояние
            double weight;                          //между ними
            if (in >> vertex1 && in >> vertex2 >> weight){
                graph[vertex1].push_back(std::pair<int, double>(vertex2, weight));
                continue;
            }
            break;
        }
    }



    void runSearch(int start, int finish){                      //Функция поиска кратчайшего пути
        std::vector<std::pair<int, double>> distance;           //Контейнер дистанций между вершинами
        std::map<int,bool> visitedVertexs;                      //Посещенные вершины
        std::map<int,int> way;                                  //Контейнер путей

        for(graph_iterator = graph.begin(); graph_iterator!=graph.end(); graph_iterator++)
            visitedVertexs[graph_iterator->first]=false;

        distance.emplace_back(std::pair<int, double>(start, 0));

        for(int i=0; i<=graph.size(); i++){

            std::pair<int, double> current = minimal_element(distance, finish);      //Нахождение ближайшей вершины

            if(current.first == finish){                                               //Если текущая вершина финишная, то выходим из цикла
                std::cout << "--------------------------------"<<std::endl;
                std::cout << "The algorithm has finished work";
                break;
            }


            std::cout << "-----------------------------------" << std::endl;
            std::cout << "Current vertex: " << current.first << std::endl<<std::endl;

            visitedVertexs[current.first]=true;

            for(std::pair<int,double> neighbor : graph[current.first]) {            //Цикл для всех соседних вершин, относительно текущей
                if (!visitedVertexs[neighbor.first]) {
                    std::pair<int, double> *distanceToNeighbor = &distance[findEl(distance, neighbor.first)];  //Находим вершину в векторе дистанций
                    if (distanceToNeighbor->first == distance[distance.size() - 1].first) {                         //Если еще не посещалась вершина
                        distance.push_back(std::pair<int, double>(neighbor.first, current.second + neighbor.second));
                        way[neighbor.first] = current.first;                                                        //Добавляем вершину в путь

                    }
                    else if (current.second + neighbor.second < distanceToNeighbor->second) {                   //Если путь до уже найденной вершины короче из текущей вершины
                        way[neighbor.first] = current.first;
                        distanceToNeighbor->second = current.second + neighbor.second;                            //Перестраиваем путь
                    }

                }
                std::cout << "Find new vertex  " << neighbor.first << "  with path length " << distance[distance.size()-1].second << std::endl;
                std::cout << "Value of heuristic function: " << heuristic(neighbor.first, finish) << std::endl << std::endl;


            }
            removeEl(distance, current);

            std::cout << "Optimal length path to vertex:" << std::endl;
            for (int i = 0; i<distance.size(); i++){
                std::cout << "\t" << distance[i].first << " --> " << distance[i].second << std::endl;
            }

        }

        std::vector<int> path;                  //Вектор пути

        int vertex = finish;                    //Обратным путем из финиша до конца восстанавливаем путь
        while (vertex != start ){
            path.push_back(vertex);
            vertex = way.find(vertex)->second;
        }
        path.push_back(vertex);


        std::cout<< std::endl <<"Result of algorithm: ";
        for (int i=path.size()-1; i>=0; i--)
            std::cout << path[i] << " ";                    //Выводим найденный путь
    }

private:
    void removeEl(std::vector<std::pair<int, double>>& distance, std::pair<int, double> el){    //Функция удаления элемента из вектора дистанций
        for (int i=0; i<distance.size(); i++){
            if (distance[i].first == el.first){
                for (i; i<distance.size() - 1; i++)
                    distance[i] = distance[i+1];
                distance.pop_back();
                return;
            }
        }
    }

    int findEl(std::vector<std::pair<int, double>>& distance, int vertex){      //Функция нахождения элемента в векторе дистанций
        for (int i=0; i<distance.size(); i++){
            if (distance[i].first == vertex)
                return i;                                                       //Возвращается индекс элемента в векторе
        }
        return distance.size()-1;
    }

    std::pair<int, double> minimal_element(std::vector<std::pair<int, double>> distance, int finish){  //Функция нахождения минимального элемента в векторе
        double min_value = distance[0].second + heuristic(distance[0].first, finish);
        std::pair<int, double> min = distance[0];
        for (int i = 1; i<distance.size(); i++){
            if (distance[i].second + heuristic(distance[i].first, finish) < min_value) {
                min_value = distance[i].second + heuristic(distance[i].first, finish);
                min = distance[i];
            }
        }
        return min;                                                     //Возвращается индекс минимального элемента
    }


    int heuristic(int first, int second){               //Эвристическая функция
        return abs(first - second);
    }


};

int main() {
    in.open("test.txt");
    int start, finish;
    in >> start;
    in >> finish;
    Graph graph;                                    //Создание графа
    graph.fillingGraph();                           //Заполнение графа
    graph.runSearch(start,finish);                  //Поиск кратчайшего пути
    return 0;
}
