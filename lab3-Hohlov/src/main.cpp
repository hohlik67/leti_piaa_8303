#include <iostream>
#include <algorithm>
#include <map>
#include <deque>
#include <queue>
#include <climits>


using Graph = std::map<char, std::map<char, int>>;          //Граф для алгоритма

bool BFS(Graph &graph, char start, char end, std::map<char, char>& path) {          //Функция поиска в ширину
    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "Start wide search" << std::endl;
    std::map<char,bool> findVertex;                   //Посещенные вершины
    std::queue<char> queueVertex;                  //Очередь вершин
    queueVertex.push(start);
    bool hasNeighbor;                       //Флаг на то, имеет ли вершина непосещенных соседей
    findVertex[start] = true;
    while (!queueVertex.empty())                   //Пока очередь не пустая
    {
        char vertex = queueVertex.front();
        queueVertex.pop();
        std::cout << "Current vertex " << vertex << " and his neighbor: " << std::endl;
        hasNeighbor = false;
        for (auto const neighbor : graph[vertex]){                      //Просматриваются соседи и кладутся в очередь
            if (neighbor.second > 0 && !(findVertex[neighbor.first])) {
                queueVertex.push(neighbor.first);
                findVertex[neighbor.first] = true;
                path[neighbor.first] = vertex;
                std::cout << "\t\t" << neighbor.first << "  with possible flow =  " << neighbor.second << std::endl;
                hasNeighbor = true;
                if (neighbor.first == end) {
                    queueVertex = *(new std::queue<char>());
                    break;
                }

            }

        }

        if (!hasNeighbor){
            std::cout << "\t\thasn't not visited neighbor";
        }

        std::cout << std::endl;
    }

    return findVertex[end];                            //Была ли достигнута финишная вершина графа

}

void printCurrentFlows(Graph& flowGraph, int pathFlow, int maxCurrentFlow, std::string& pathStr){  //Функция печати текущего состояния графа, найденного нового пути и найденного нового потока
    std::cout << "\nFind new path with flow = " << pathFlow << ": " + pathStr << std::endl;
    std::cout << "Flow graph: " << std::endl;

    for (auto const& vertex: flowGraph) {
        for (auto const neighbor: flowGraph[vertex.first]) {
            std::cout << "\t" << vertex.first << " " << neighbor.first << " " << neighbor.second << std::endl;
        }
    }

    std::cout << "Current flow of graph = " << maxCurrentFlow << std::endl;
}

void printResult(Graph& graph, Graph& flowGraph, int maxFlow){           //Функция печати результата
    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "\nResult of algorithm: " <<std::endl;
    int flow;
    std::cout <<"Max flow = "<< maxFlow << std::endl;                              //Печать максимального потока
    for (auto const& vertex: graph)
        for (auto const neighbor: graph[vertex.first]) {
            if (neighbor.second - flowGraph[vertex.first][neighbor.first] < 0)
                flow = 0;
            else
                flow = neighbor.second - flowGraph[vertex.first][neighbor.first];

            std::cout << vertex.first << " " << neighbor.first << " " << flow << std::endl;   // Печать потока через ребро
        }
}


void FFA(Graph &graph, char start, char finish) {           //Функция, реализующая алгоритм Форда-Фалкерсона
    Graph flowGraph = graph;                                //Граф с потоками
    char fromVertex, toVertex;
    std::map<char,char> path;                               //Пары, составляющие путь
    std::string pathStr;

    int maxFlow = 0;
    while (BFS(flowGraph, start, finish, path))        //Пока возможен поиск в ширину
    {
        int pathFlow = INT_MAX;
        pathStr = finish;
        for (toVertex = finish; toVertex != start; toVertex = path[toVertex])           //Восстанавливается путь от финиша к началу
        {
            fromVertex = path[toVertex];
            pathFlow = std::min(pathFlow, flowGraph[fromVertex][toVertex]);           //Находится поток пути
        }
        for (toVertex = finish; toVertex != start; toVertex = path[toVertex])           //Восстанавливается путь от финиша к началу
        {
            fromVertex = path[toVertex];
            flowGraph[fromVertex][toVertex] -= pathFlow;                               //Изменяется граф с потоком
            flowGraph[toVertex][fromVertex] += pathFlow;
            pathStr = std::string(1, fromVertex) + " --> " + pathStr;
        }

        maxFlow += pathFlow;                                                   //Изменяется число максимального потока
        printCurrentFlows(flowGraph, pathFlow, maxFlow, pathStr);
    }

    printResult(graph, flowGraph, maxFlow);
}


int main() {
    Graph graph;
    char start, finish, vertexFrom, vertexTo;
    int pathLength, countVertex;

    std::cin >> countVertex;
    std::cin >> start;
    std::cin >> finish;

    for (int i=0; i<countVertex; i++) {                                         //Считывание вершин графа
        std::cin >> vertexFrom;
        std::cin >> vertexTo;
        std::cin >> pathLength;
        graph[vertexFrom][vertexTo] = pathLength;
    }

    FFA(graph, start, finish);                                      //Запуск алгоритма Форда-Фалкерсона

    return 0;
}
