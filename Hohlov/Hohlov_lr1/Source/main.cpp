#include <iostream>
#include <vector>
#define DBG
using namespace std;



//Структура вставленного квадрата в виде координаты-размер квадрата
struct InsertedRect{
    int x;
    int y;
    int size;
};

int n, m;
int minCount;               // минимальное число квадратов для покрытия
int colorings = 0;          // количество покрытий минимальным числом квадратов
vector<InsertedRect> minRect;     // минимальное разбиение
vector<vector<int>> resultRect;

//Функция инициализации прямоугольника нулями
void clearRect(vector<vector<int>> &_rect){
    for (int i = 0; i < n; i++) {
        _rect.resize(n);
        for (int j = 0; j < m; j++) {
            _rect[i].push_back(0);
        }
    }
}

void printRect(vector<vector<int>>& _rect){        //Вывод текущего состояния поля в стандартный поток вывода

    cout << "\t===>>\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout.width(3);
            cout << _rect[i][j];
        }
        cout << '\n';
    }
    cout << endl;
}

//Функция восстановления прямоугольника и вектора вставленных квадратов
void recovery(vector<vector<int>>& _rect, vector<InsertedRect> &_res){
    InsertedRect tmp = *(_res.rbegin());                      // сохраняем во временную переменную последний добавленный квадрат
    _res.pop_back();                                    // удаляем из вектора последний квадрат
    cout << "recovery\n";
    for (int i = tmp.x; i < tmp.x+tmp.size; i++)        // заполняем нулями область, в которой был размещен последний квадрат
        for (int j = tmp.y; j < tmp.y+tmp.size; j++)
            _rect[i][j] = 0;
}

//Функция проверки на то, можно ли вставить квадрат размера size в точку {x,y}
bool isPushable(vector<vector<int>> &_rect, int x, int y, int size){
    if (n-x < size || m-y < size)               // если квадрат выходит за пределы поля возвращаем false
        return false;
    for (int i = x; i < x+size; i++)            // если квадрат пересекат другой квадрат возвращаем false
        for (int j = y; j < y+size; j++){
            if (_rect[i][j])
                return false;
        }
    return true;                                // если можно вставить квадрат возвращаем true
}

//Функция вставки квадрата
void push(vector<vector<int>>& _rect, int x, int y, int size){
    for (int i = x; i < x+size; i++)
        for (int j = y; j < y+size; j++){
            _rect[i][j] = size;
        }
}

//Непосредственно рекурсивный бэктрекинг
void recBack(vector<vector<int>>& _rect, int curSquare, int unPainted, int countOfSquares, vector<InsertedRect> &_res){
    if ((countOfSquares==minCount-1 && unPainted > curSquare*curSquare))        // если на текущем шаге дойдем до числа лучшего на данный
        return;                                                                 // момент разбиения, а поля не будет закрашено полностью, возвращаемся

    bool pushed = false;
    for (int i = 0; i < n && !pushed; i++){
        for (int j = 0; j < m && !pushed; j++){
            if (_rect[i][j] == 0) {
                if (isPushable(_rect, i, j, curSquare)) {                   // если можно вставить в первую свободную ячейку - вставляем
                    pushed = true;
                    push(_rect, i, j, curSquare);
                    printRect(_rect);
                    _res.push_back({i, j, curSquare});
                }
                else{                                                         // иначе возвращаемся
                    return;
                }
            }
            else
                j+=_rect[i][j]-1;                                             // так как квадраты в матрице заполняются числом, равным своему размеру,
                                                                              // можем проскакивать на это число дальше по матрице
        }
    }

    if (countOfSquares+1 == minCount) {                             // если число квадратов на этом шаге равно лучшему на данный момент разбиению,
        if (unPainted==curSquare*curSquare)                         // то если все закрашено, увеличиваем счетчик вариантов заполнения минимальным числом квадратов
        {
            colorings++;
            cout << "it's a new minimum\n";
        }
        recovery(_rect, _res);                                 // отменяем внесенные изменения и возвращаемся
        return;
    }
    if (unPainted==curSquare*curSquare && countOfSquares+1 < minCount){ // если все поле заполнено и текущее число квадратов меньше чем лучшее на данный
        colorings = 1;                                                  // момент, заменяем лучшее на текущее
        minCount = countOfSquares+1;
        minRect.assign(_res.begin(), _res.end());
        cout << "it's a new minimum\n";
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                resultRect[i][j] = _rect[i][j];

        recovery(_rect, _res);                                    // отменяем внесенные изменения и возвращаемся
        return;
    }

    for (int i = min(min(n,m)-1, max(n,m) - curSquare); i > 0; i--)    // рекурсивно вызываем для следующих квадратов
        if (i*i <= unPainted)
            recBack(_rect, i, unPainted-curSquare*curSquare, countOfSquares+1, _res);

    recovery(_rect, _res);         // отменяем изменения перед выходом
}

int main() {
    cout << "Enter rectangle height&width: ";
    cin >> n >> m;                             // считываем размер прямоугольника
    minCount = n*m+1;                          // выставляем недостижимое значение числа квадратов
    int expansionCoeff = 1;                    // коэфф расширения для замены квадратов
    vector<vector<int>> rect;
    vector<InsertedRect> result;
    clearRect(resultRect);
    // если квадрат кратен 2, 3, 5 - заменяем его на квадрат размера 2, 3, 5 и записываем коэффициент расширения для вывода
    if (n==m){
        if (n%2 == 0){
            expansionCoeff = n/2;
            n = 2;
            m = 2;
        }
        else if (n%3 == 0){
            expansionCoeff = n/3;
            n = 3;
            m = 3;
        }
        else if (n%5 == 0){
            expansionCoeff = n/5;
            n = 5;
            m = 5;
        }
    }

    clearRect(rect);        // инициализируем матрицу нулями
    int unPainted = n*m;       // записываем число не закрашенных ячеек

    for (int i = min(n,m)-1; i > 0; i--)   // вызываем бэктрекинг с начальным квадратом размера от n-1 до 1
        recBack(rect, i, unPainted, result.size(), result);

    cout << "Minimal number of squares: " << minCount << endl;
    for (auto & i : minRect){   // выводим результат с учетом коэффициента расширения
        cout << expansionCoeff*i.x + 1 << ' ' << expansionCoeff*i.y + 1 << ' ' << expansionCoeff*i.size << '\n';
    }
    printRect(resultRect);
    cout << "\nCount of minimal permutations: " << colorings << endl;
    return 0;
}