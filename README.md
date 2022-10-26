### МИНИСТЕРСТВО НАУКИ И ВЫСШЕГО ОБРАЗОВАНИЯ РОССИЙСКОЙ ФЕДЕРАЦИИ
### Федеральное государственное автономное образовательное учреждение высшего образования
## Дальневосточный федеральный университет
***
### Институт математики и компьютерных технологий
### Департамент информационных и компьютерных систем
### Отчёт о практическом задании по предмету АИСД
# Алгоритм Дейкстры с использованием Фибоначчиевой кучи
***
### Коканов Александр Владиславович, гр. Б9121-09.03.03пикд
### г. Владивосток, 2022

# Содержание
***
[*Содержание*](#содержание)
[*Введение*](#введение)
[*Глоссарий*](#глоссарий)
[*Алгоритм Дейкстры*](#алгоритм-дейкстры)
[*Фибоначчиева куча*](#фибоначчиева-куча)
[*Список литературы*](#список-литературы)

# Введение

Во многих ситуациях нам необходимо найти самый оптимальный с точки зрения расстояния или, другим словами, кратчайший путь между какими-либо двумя точками. Однако помимо этого, при больших входных данных, нам нужно уметь делать это как можно быстрее. 

## Применение

- Картографические сервисы.
Алгоритмы нахождения кратчайшего пути на графе применяются для нахождения путей между физическими объектами на таких картографических сервисах, как карты Google или OpenStreetMap.
- маршрутизация движения данных в компьютерной сети.
- расчет движения тока по электрическим цепям.
- поиск системой бронирования наиболее быстрых или дешевых билетов, в том числе с возможными пересадками;
- разработка поведения неигровых персонажей, создание игрового ИИ в разработке игр. 
- Недетерминированная машина
Если представить недетерминированную абстрактную машину как граф, где вершины описывают состояния, а ребра определяют возможные переходы, тогда алгоритмы поиска кратчайшего пути могут быть применены для поиска оптимальной последовательности решений для достижения главной цели. Например, если вершинами являются состояния Кубика Рубика, а ребром представляет собой одно действие над кубиком, тогда алгоритм может быть применён для поиска решения с минимальным количеством ходов.


# Глоссарий
Граф 
:  структура данных, отображающая связи между разными объектами. Состоит из множества вершин и множества соединяющих их рёбер.

Вершина
: точка в графе, отдельный объект. Может иметь некоторые характеристики, в частности, число, поставленное в соответствие данной вершине и называемое весом, стоимостью или пропускной способностью вершины.

Ребро
: неупорядоченная пара двух вершин, которые связаны друг с другом. Эти вершины называются концевыми точками или концами ребра. Ребро также может иметь вес.

Путь
: последовательность вершин, в которой каждая вершина соединена со следующей ребром. Обычно путь задаётся перечислением вершин, по которым он пролегает.

Ориентированное ребро 
: ребро, в котором одну вершину считают началом ребра, а другую — концом.

Связный граф
: граф, в котором существует путь между любыми двумия вершинами.

Взвешенный граф
: граф, в котором у каждого ребра и/или каждой вершины есть вес

Дерево
: стандартная структура данных, являющаяся связным графом, в котором нет ни одного пути, где начальная и конечная вершина совпадают (цикл). 

Потомок
: вершина, имеющий родительскую вершину.

Родитель
: вершина, имеющая ребро, соединяющее её с вершиной-потомком.

Корень дерева
: вершина, не имеющая родителя

Ранг дерева 
:  количество рёбер, исходящих из корня дерева. 


Двусвязный циклический линейный список
: стандартная структура данных, состоящая из элементов, содержащих значение и ссылки на следующий и предыдущий элемент списка. При этом начальный элемент содержит ссылку на конечный элемент, и наоборот.

# Алгоритм Дейкстры

___Алгоритм Дейкстры___
: алгоритм на графах, изобретённый нидерландским учёным Эдсгером Дейкстрой в 1959 году. Находит кратчайшие пути от одной из вершин графа до всех остальных или до одной конкретной. Алгоритм работает только для графов без рёбер отрицательного веса.

#### Описание алгоритма
Каждой вершине из V сопоставим метку — минимальное известное расстояние от этой вершины до a.

Алгоритм работает пошагово — на каждом шаге он «посещает» одну вершину и пытается уменьшать метки.

Работа алгоритма завершается, когда все вершины посещены.

Инициализация:
- Метка самой вершины a инициализируется 0.
- Метки остальных вершин инициализируются бесконечносями. Это отражает то, что расстояния от a до других вершин пока неизвестны
- Все вершины графа помечаются как не посещённые.

Шаг алгоритма:
- Если все вершины посещены, алгоритм завершается.
- В противном случае, из ещё не посещённых вершин выбирается вершина u, имеющая минимальную метку.

- Мы рассматриваем всевозможные маршруты, в которых u является предпоследним пунктом. Вершины, в которые ведут рёбра из u, назовём соседями этой вершины. Для каждого соседа вершины u, кроме отмеченных как посещённые, рассмотрим новую длину пути, равную сумме значений текущей метки u и длины ребра, соединяющего u с этим соседом.
- Если полученное значение длины меньше значения метки соседа, заменим значение метки полученным значением длины. Рассмотрев всех соседей, пометим вершину u как посещённую и повторим шаг алгоритма.


Результатом алгоритма является список кратчаших маршрутов от исходной точки до всех остальных



# Фибоначчиева куча
Фибоначчиева куча
: стандартная структура данных, представляющая их себя набор деревьев, каждое из которых имеет не менее F(n + 2) элементов, где F(i) – i - ое число фибоначчи, а n –  ранг дерева. При этом в каждом дереве можно удалить не более одного элемента. Корни деревьев объединены в циклический двусвязный список. Основными операциями фибонччиевой кучи являются: вставка нового элемента, получение значения минимального элемента, удаление минимального элемента, уменьшение значения элемента, соединение двух куч.

Фибоначчиева куча хранит в себе указатель на минимальный узел и, в некоторых случаях, размер. 

``` c++
class FibonacciHeap {
public:
    Node *min;
    int size;
}
```
Сам узел хранит в себе следующие данные:
1. Ключ – значение, с помощью которого можно идентифицировать узел. 
2.	Значение элемента
3.	Указатель на родительский узел
4.	указатель на дочерний узел с минимальным значением
5.	указатель на левый узел
6.	указатель на правый узел
7.	Ранг дерева, корнем которого является данный узел
8.	Метка о том, был ли удален в процессе изменения ключа ребенок этой вершины

``` c++
class Node {
public:
    int key;
    int value;
    Node *parent;
    Node *left;
    Node *right;
    Node *child;
    int degree;
    bool mark;   
};
```

#### Операции, поддерживаемые Фибоначчиевой кучей
 


- ___Вставка элемента___
Вставка элемента, как и большинство операций, происходит довольно просто. Необходимо создать новый элемент и связать его с корневым списком кучи. Получается так, что при создании нового элемента мы просто создаём фибонначиево дерево с одной вершиной или же с нулевым рангом. Также после этого необходимо сравнить значения нового элемента и минимума, и если новый элемент меньше, то заменить минимум.

``` c++
Node *Insert(int value, int key) {
        Node *new_elem = MakeNode(value, key); // создание нового эдемента
        min = Merge(min, new_elem); // слияние нового элемента с корневым списком 
        return new_elem;
    }
```
- ___Получение минимума___
Так как мы храним указатель на узел с минимальным значением, нам необходимо просто вернуть сответствующий узел.

``` c++
Node* GetMin() {
        return min;
    }
```

- ___Слияние с другой кучей___
Слияние двух куч представляет собой, по сути, добавление корня первой кучи в список детей второй кучи. Таким образом, одна из куч становится подкучей второй. Важно отметить, что главной всегда остаётся куча с наименьшим минимумом. Так что если вдруг у нашей кучи минимум больше, чем у новой, нужно либо перед слиянием поменять кучи местами, либо после слияния поменять указатель на минимум.

``` c++
 Node* Merge(Node *first, Node *second) {
        if (first == nullptr) 
            return second;
        if (second == nullptr) 
            return first;
        if (first->key > second->key) {
            Node* loc_elem = first;
            first = second;
            second = loc_elem;
        }
        Node *A = first->left;
        Node *B = second->left;
        A->right = second;
        second->left = A;
        first->left = B;
        B->right = first;
        return first;
    }
```

- ___Уменьшение элемента___
При уменьшении элемента нам необходимо проверить,стал ли наш элемент меньше своего родителя. Если такое всё-таки произошло, мы отрезаем элемент от кучи и связываем его с корневым списком. Однано, может произойти так, что после наших действий у родителя нашего элемента было удалено 2 ребёнка, что недопустимо. В таком случае нам необходимо выполнить так называемое "каскадное образание". Начиная с родителя нашего элемента, мы проверяем, был ли он помечен. Если да, то вырезаем его, снимаем метку и повторяем все действия с его родителем (с родителем родителя исходного элемента). Продолжаем до тех пор, пока не найдём элемент без "пометки" или пока не дойдём до корня дерева. 

``` c++
Node* DecreaseKey(Node *heap, Node *elem, int key) {
        if (elem->key < key) // проверяем, меньше ли новое значение
            return heap;
        elem->key = key;
        if (elem->parent != nullptr) {
            if (elem->key < elem->parent->key) {
                heap = Cut(heap, elem); // вырезаем элемент
                Node* parent = elem->parent;
                elem->parent = nullptr;
                while (parent != nullptr && parent->mark) { // выполняем каскадное обрезание
                    heap = Cut(heap,parent);
                    elem = parent;
                    parent = elem->parent;
                    elem->parent = nullptr;
                }
                if (parent != nullptr && parent->parent != nullptr)
                    parent->mark = true;
            }
        }
        else if (elem->key < heap->key) 
                heap = elem;
        return heap;
    }
```
- ___Удаление минимума___
Удаление минимума является самой долгой операцией, так как именно при её выполнении мы приводим нашу кучу в порядок, а именно «прореживаем» деревья. Сначала мы отрезаем наш минимум от корневого списка, а всех его детей отправляем в корневой список. После этого мы проходимся по всем корням и попарно соединяем деревьев с равным рангом. То есть, если у нас есть два дерева ранга k, то мы их сливаем и получаем одно дерево ранга k + 1. Параллельно с этим мы ищем минимум среди корней. После выполнения данной операции у нас не будет два дерева с одинаковым рангом, а общее количество деревьев будет равно log(n), где n – количество вершин. 




# Список литературы
1. https://wiki5.ru/wiki/Fibonacci_heap#Implementation_of_operations 
2. https://neerc.ifmo.ru/wiki/index.php?title=%D0%A4%D0%B8%D0%B1%D0%BE%D0%BD%D0%B0%D1%87%D1%87%D0%B8%D0%B5%D0%B2%D0%B0_%D0%BA%D1%83%D1%87%D0%B0
3. https://ru.wikipedia.org/wiki/%D0%A4%D0%B8%D0%B1%D0%BE%D0%BD%D0%B0%D1%87%D1%87%D0%B8%D0%B5%D0%B2%D0%B0_%D0%BA%D1%83%D1%87%D0%B0
4. http://cppalgo.blogspot.com/2011/11/fibonacci-heap.html
5. https://ru.wikipedia.org/wiki/%D0%90%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC_%D0%94%D0%B5%D0%B9%D0%BA%D1%81%D1%82%D1%80%D1%8B
6. https://habr.com/ru/post/111361/
7. http://acm.math.spbu.ru/~sk1/mm/au-download/conspect/conspect.pdf
8. https://prog-cpp.ru/deikstra/
9. https://www.cs.princeton.edu/~wayne/teaching/fibonacci-heap.pdf 
10. https://vies.wiki/wiki/ru/Fibonacci_heap
11.	https://youtu.be/CeAjTL-Fshs
12.	http://acm.math.spbu.ru/~sk1/mm/au-download/14f-conspect/2014-10-21-Fib.pdf
13. https://wiki5.ru/wiki/Potential_method
14.	https://kbaile03.github.io/projects/fibo_dijk/fibo_dijk.html	
15.	http://www-m3.ma.tum.de/foswiki/pub/MN0506/WebHome/dijkstra.pdf
16.	http://staff.ustc.edu.cn/~csli/graduate/algorithms/book6/chap21.htm
17.	https://infopedia.su/17xb2c6.html
18.	https://e-maxx.ru/algo/dijkstra
19.	https://www.programiz.com/dsa/fibonacci-heap
20.	https://www.geeksforgeeks.org/fibonacci-heap-insertion-and-union/
21.	https://ask-dev.ru/info/101216/what-is-the-intuition-behind-the-fibonacci-heap-data-structure
22.	https://maryrosecook.com/blog/post/the-fibonacci-heap-ruins-my-life
23.	https://cse.sc.edu/~mgv/csce580f08/gradPres/boccanfusoMcKenzie080910.ppt
24.	https://www.cs.auckland.ac.nz/software/AlgAnim/dijkstra.html#dijkstra_anim
25.	https://web.stanford.edu/class/archive/cs/cs166/cs166.1146/lectures/07/Small07.pdf
26.	https://blog.skillfactory.ru/glossary/algoritm-dejkstry/ 
27.	https://e-maxx.ru/algo/dijkstra_sparse 



<!-- 
Алгоритм Дейкстры в сочетании с Фибоначчиевой кучей, в теории, является практически самым быстрым алгоритмом для поиска кратчайшего пути в графе. -->