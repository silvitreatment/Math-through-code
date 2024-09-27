#include <iostream>
#include <cmath>
#include <vector>
#include <limits>

using namespace std;

// Структура узла в куче Фибоначчи
struct FibonacciNode {
    int key;                  // Ключ узла
    int degree;               // Количество детей
    FibonacciNode* parent;    // Указатель на родителя
    FibonacciNode* child;     // Указатель на любого ребенка
    FibonacciNode* left;      // Левый сосед в списке
    FibonacciNode* right;     // Правый сосед в списке
    bool mark;                // Флаг, помечающий, потерял ли узел ребенка

    // Конструктор узла
    FibonacciNode(int k) : key(k), degree(0), parent(nullptr), child(nullptr),
        left(this), right(this), mark(false) {}
};

// Класс кучи Фибоначчи
class FibonacciHeap {
public:
    FibonacciHeap() : minNode(nullptr), nodeCount(0) {}

    // Вставка нового узла в кучу
    void insert(int key) {
        FibonacciNode* newNode = new FibonacciNode(key);
        if (minNode == nullptr) {
            minNode = newNode;
        }
        else {
            insertIntoRootList(newNode);
            if (key < minNode->key) {
                minNode = newNode;
            }
        }
        ++nodeCount;
    }

    // Извлечение минимального узла (удаление его из кучи)
    FibonacciNode* extractMin() {
        FibonacciNode* oldMin = minNode;
        if (minNode != nullptr) {
            // Если у минимального узла есть дети, перемещаем их в корневой список
            if (minNode->child != nullptr) {
                FibonacciNode* child = minNode->child;
                do {
                    FibonacciNode* nextChild = child->right;
                    insertIntoRootList(child);
                    child->parent = nullptr;
                    child = nextChild;
                } while (child != minNode->child);
            }

            // Удаляем минимальный узел из корневого списка
            removeFromRootList(minNode);
            if (minNode == minNode->right) {
                minNode = nullptr; // Если был один элемент
            }
            else {
                minNode = minNode->right;
                consolidate(); // Консолидация деревьев
            }
            --nodeCount;
        }
        return oldMin;
    }

    // Уменьшение ключа узла
    void decreaseKey(FibonacciNode* node, int newKey) {
        if (newKey > node->key) {
            throw invalid_argument("Новый ключ должен быть меньше текущего.");
        }

        node->key = newKey;
        FibonacciNode* parent = node->parent;

        if (parent != nullptr && node->key < parent->key) {
            cut(node, parent);
            cascadingCut(parent);
        }

        if (node->key < minNode->key) {
            minNode = node;
        }
    }

    // Удаление узла из кучи
    void deleteNode(FibonacciNode* node) {
        decreaseKey(node, numeric_limits<int>::min());
        extractMin();
    }

private:
    FibonacciNode* minNode;   // Указатель на минимальный узел
    int nodeCount;            // Количество узлов в куче

    // Вставка узла в корневой список
    void insertIntoRootList(FibonacciNode* node) {
        if (minNode == nullptr) {
            minNode = node;
        }
        else {
            node->left = minNode;
            node->right = minNode->right;
            minNode->right->left = node;
            minNode->right = node;
        }
    }

    // Удаление узла из корневого списка
    void removeFromRootList(FibonacciNode* node) {
        if (node->right == node) {
            minNode = nullptr;
        }
        else {
            node->left->right = node->right;
            node->right->left = node->left;
        }
    }

    // Консолидация деревьев в куче
    void consolidate() {
        int maxDegree = static_cast<int>(log2(nodeCount)) + 1;
        vector<FibonacciNode*> degreeTable(maxDegree, nullptr);

        vector<FibonacciNode*> rootNodes;
        FibonacciNode* curr = minNode;
        do {
            rootNodes.push_back(curr);
            curr = curr->right;
        } while (curr != minNode);

        for (FibonacciNode* node : rootNodes) {
            int d = node->degree;
            while (degreeTable[d] != nullptr) {
                FibonacciNode* other = degreeTable[d];
                if (node->key > other->key) {
                    swap(node, other);
                }
                link(other, node);
                degreeTable[d] = nullptr;
                ++d;
            }
            degreeTable[d] = node;
        }

        minNode = nullptr;
        for (FibonacciNode* node : degreeTable) {
            if (node != nullptr) {
                if (minNode == nullptr) {
                    minNode = node;
                }
                else {
                    insertIntoRootList(node);
                    if (node->key < minNode->key) {
                        minNode = node;
                    }
                }
            }
        }
    }

    // Слияние двух деревьев (один становится ребёнком другого)
    void link(FibonacciNode* child, FibonacciNode* parent) {
        removeFromRootList(child);
        child->left = child->right = child;
        child->parent = parent;
        if (parent->child == nullptr) {
            parent->child = child;
        }
        else {
            child->left = parent->child;
            child->right = parent->child->right;
            parent->child->right->left = child;
            parent->child->right = child;
        }
        ++parent->degree;
        child->mark = false;
    }

    // Обрезка узла и его перемещение в корневой список
    void cut(FibonacciNode* node, FibonacciNode* parent) {
        if (node->right != node) {
            node->left->right = node->right;
            node->right->left = node->left;
        }
        if (parent->child == node) {
            if (node->right == node) {
                parent->child = nullptr;
            }
            else {
                parent->child = node->right;
            }
        }
        --parent->degree;
        insertIntoRootList(node);
        node->parent = nullptr;
        node->mark = false;
    }

    // Каскадная обрезка: если родитель потерял второго ребёнка, его тоже обрезаем
    void cascadingCut(FibonacciNode* node) {
        FibonacciNode* parent = node->parent;
        if (parent != nullptr) {
            if (!node->mark) {
                node->mark = true;
            }
            else {
                cut(node, parent);
                cascadingCut(parent);
            }
        }
    }
};

int main() {
    FibonacciHeap heap;

    // Вставка элементов
    heap.insert(10);
    heap.insert(3);
    heap.insert(15);
    heap.insert(6);

    // Извлечение минимального элемента
    cout << "Минимальный элемент: " << heap.extractMin()->key << endl;

    return 0;
}
