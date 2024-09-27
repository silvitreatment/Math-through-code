#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Структура точки в k-мерном пространстве
struct Point {
    vector<int> coords; // координаты точки

    Point(int dimensions) : coords(dimensions) {}

    // Для удобного доступа к координатам
    int& operator[](int index) {
        return coords[index];
    }

    const int& operator[](int index) const {
        return coords[index];
    }
};

// Узел k-d дерева
struct KDNode {
    Point point;       // Точка, хранящаяся в узле
    KDNode* left;      // Левое поддерево
    KDNode* right;     // Правое поддерево

    KDNode(Point p) : point(p), left(nullptr), right(nullptr) {}
};

// K-d дерево
class KDTree {
private:
    int k; // Количество измерений (размерности)

    // Рекурсивная функция для построения k-d дерева
    KDNode* buildTree(vector<Point>& points, int depth) {
        if (points.empty()) return nullptr;

        // Определяем ось разбиения
        int axis = depth % k;

        // Сортируем точки по оси
        sort(points.begin(), points.end(), [axis](const Point& a, const Point& b) {
            return a[axis] < b[axis];
            });

        // Выбираем медиану
        int medianIndex = points.size() / 2;
        KDNode* node = new KDNode(points[medianIndex]);

        // Разделяем точки на левую и правую части
        vector<Point> leftPoints(points.begin(), points.begin() + medianIndex);
        vector<Point> rightPoints(points.begin() + medianIndex + 1, points.end());

        // Рекурсивно строим левое и правое поддеревья
        node->left = buildTree(leftPoints, depth + 1);
        node->right = buildTree(rightPoints, depth + 1);

        return node;
    }

public:
    KDNode* root; // Корень дерева

    KDTree(const vector<Point>& points, int dimensions) : k(dimensions), root(nullptr) {
        vector<Point> pts = points; // Копируем точки для сортировки
        root = buildTree(pts, 0);   // Строим дерево
    }

    // Функция для поиска ближайшей точки в дереве
    Point nearestNeighbor(const Point& target) {
        return nearest(root, target, 0).first;
    }

private:
    // Рекурсивная функция для поиска ближайшего соседа
    pair<Point, double> nearest(KDNode* node, const Point& target, int depth) {
        if (!node) return { Point(k), DBL_MAX }; // Базовый случай: если узел пуст

        // Ось разбиения
        int axis = depth % k;

        // Выбираем следующее поддерево для поиска
        KDNode* nextBranch = nullptr;
        KDNode* otherBranch = nullptr;

        if (target[axis] < node->point[axis]) {
            nextBranch = node->left;
            otherBranch = node->right;
        }
        else {
            nextBranch = node->right;
            otherBranch = node->left;
        }

        // Рекурсивно ищем ближайшую точку в выбранном поддереве
        auto best = nearest(nextBranch, target, depth + 1);
        double bestDist = distance(target, best.first);

        // Проверяем текущий узел
        double currentDist = distance(target, node->point);
        if (currentDist < bestDist) {
            best = { node->point, currentDist };
            bestDist = currentDist;
        }

        // Проверяем другую ветвь, если есть вероятность найти там точку ближе
        double axisDist = abs(target[axis] - node->point[axis]);
        if (axisDist < bestDist) {
            auto possibleBest = nearest(otherBranch, target, depth + 1);
            double possibleBestDist = distance(target, possibleBest.first);

            if (possibleBestDist < bestDist) {
                best = possibleBest;
            }
        }

        return best;
    }

    // Вычисляем евклидово расстояние между двумя точками
    double distance(const Point& a, const Point& b) {
        double dist = 0;
        for (int i = 0; i < k; ++i) {
            dist += (a[i] - b[i]) * (a[i] - b[i]);
        }
        return sqrt(dist);
    }
};

int main() {
    // Пример использования k-d дерева
    int k = 2; // Размерность (например, 2D пространство)

    // Вводим набор точек
    vector<Point> points = {
        Point(k), Point(k), Point(k),
        Point(k), Point(k)
    };

    points[0][0] = 2; points[0][1] = 3;
    points[1][0] = 5; points[1][1] = 4;
    points[2][0] = 9; points[2][1] = 6;
    points[3][0] = 4; points[3][1] = 7;
    points[4][0] = 8; points[4][1] = 1;

    // Строим дерево
    KDTree tree(points, k);

    // Ищем ближайшую точку к целевой
    Point target(k);
    target[0] = 9;
    target[1] = 2;

    Point nearest = tree.nearestNeighbor(target);

    // Выводим ближайшую точку
    cout << "Nearest point to (" << target[0] << ", " << target[1] << ") is ("
        << nearest[0] << ", " << nearest[1] << ")\n";

    return 0;
}
