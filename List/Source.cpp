
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>


using namespace std;

// ���� "���������"
class Triangle {
public:
    double a, b, c;  // ������� ����������

    // ����������� �� �������������
    Triangle() : a(0), b(0), c(0) {}

    // ����������� � �����������
    Triangle(double a, double b, double c) : a(a), b(b), c(c) {}
    double side1;
    double side2;
    double side3;

    double getArea() const {
        double s = (side1 + side2 + side3) / 2.0;
        return sqrt(s * (s - side1) * (s - side2) * (s - side3));
    }

    double getPerimeter() const {
        return side1 + side2 + side3;
    }

    // ����������� �� �������������
    Triangle() : side1(0), side2(0), side3(0) {}

    // ����������� � �����������
    Triangle(double s1, double s2, double s3) : side1(s1), side2(s2), side3(s3) {}

    // ���������� ����� ���������� �� �������� ������
    double area() const {
        double p = (a + b + c) / 2;
        return sqrt(p * (p - a) * (p - b) * (p - c));
    }

    // ��������, �� � ��������� �����������
    bool isRight() const {
        return a * a + b * b == c * c || a * a + c * c == b * b || b * b + c * c == a * a;
    }

    // ��������, �� � ��������� ������������
    bool isIsosceles() const {
        return a == b || a == c || b == c;
    }

    // �������������� ��������� "<" ��� ��������� ���������� �� ������
    bool operator<(const Triangle& other) const {
        return area() < other.area();
    }

    // �������������� ��������� ">>" ��� ���������� ���������� � ������
    friend istream& operator>>(istream& in, Triangle& triangle) {
        in >> triangle.a >> triangle.b >> triangle.c;
        return in;
    }

    // �������������� ��������� "<<" ��� ��������� ���������� � ����
    friend ostream& operator<<(ostream& os, const Triangle& triangle) {
        os << "Triangle: side1=" << triangle.side1 << ", side2=" << triangle.side2 << ", side3=" << triangle.side3;
        return os;
    }
};



// ���� "�����" �����'������ ������
template<typename T>
class Node {
public:
    T data;             // ���, ������� � ����
    Node<T>* prev;      // �������� �� ��������� �����
    Node<T>* next;      // �������� �� ��������� �����

    // ����������� �� �������������
    Node() : prev(nullptr), next(nullptr) {}

    // ����������� � �����������
    Node(const T& data, Node<T>* prev = nullptr, Node<T>* next = nullptr) : data(data), prev(prev), next(next) {}
};

// ���� "�����'����� ������"
template<typename T>
class DoublyLinkedList {
public:
    Node<T>* head; // �������� �� ������ ������
    Node<T>* tail; // �������� �� ���� ������
    DoublyLinkedList() : head(nullptr), tail(nullptr) {}

    // ��������� ����� � ����� ������
    void push_back(const T& data) {
        Node<T>* newNode = new Node<T>(data, tail, nullptr);
        if (tail != nullptr) {
            tail->next = newNode;
        }
        tail = newNode;
        if (head == nullptr) {
            head = newNode;
        }
    }

    // �������� ������
    void clear() {
        Node<T>* currentNode = head;
        while (currentNode != nullptr) {
            Node<T>* nextNode = currentNode->next;
            delete currentNode;
            currentNode = nextNode;
        }
        head = nullptr;
        tail = nullptr;
    }

    // ��������, �� � ������ �������
    bool isEmpty() const {
        return head == nullptr;
    }

    int getSize() const { return count; }

    // ��������, �� ������ ������ ������� �������
    bool contains(const T& data) const {
        Node<T>* currentNode = head;
        while (currentNode != nullptr) {
            if (currentNode->data == data) {
                return true;
            }
            currentNode = currentNode->next;
        }
        return false;
    }

    // �������������� ��������� "<<" ��� ��������� ������ � ����

};

// ��������� ���� ��� �������� ������������ �����'������ ������ ����������
using TriangleList = DoublyLinkedList<Triangle>;

// ������� ��� ���������� ������ ���������� � �����
TriangleList readTrianglesFromFile(const string& filename) {
    TriangleList triangles;
    ifstream inputFile(filename);
    if (inputFile.is_open()) {
        Triangle triangle;
        while (inputFile >> triangle) {
            triangles.push_back(triangle);
        }
        inputFile.close();
    }
    return triangles;
}

// ������� ��� ���������� ������ ���������� �� ������
void sortTrianglesByArea(DoublyLinkedList<Triangle>& triangles) {
    vector<Triangle> temp(triangles.getSize());
    int i = 0;
    for (Node<Triangle>* currentNode = triangles.head; currentNode != nullptr; currentNode = currentNode->next) {
        temp[i++] = currentNode->data;
    }
    sort(temp.begin(), temp.end());
    i = 0;
    for (Node<Triangle>* currentNode = triangles.head; currentNode != nullptr; currentNode = currentNode->next) {
        currentNode->data = temp[i++];
    }
}

TriangleList moveTrianglesByPerimeter(TriangleList& triangles, double minPerimeter, double maxPerimeter) {
    TriangleList result;
    for (Node<Triangle>* currentNode = triangles.head; currentNode != nullptr; currentNode = currentNode->next) {
        double perimeter = currentNode->data.getPerimeter();
        if (perimeter >= minPerimeter && perimeter <= maxPerimeter) {
            result.push_back(currentNode->data);
        }
    }
    return result;
}
// ������� ��� ������ ������ ���������� � ����
void writeTrianglesToFile(const TriangleList& triangles, const string& filename) {
    ofstream outputFile(filename);
    if (outputFile.is_open()) {
        for (Node<Triangle>* currentNode = triangles.head; currentNode != nullptr; currentNode = currentNode->next) {
            outputFile << currentNode->data << endl;
        }
        outputFile.close();
    }
}

// Unit �����
void runUnitTests() {
    // ���� ��� ������� ���������� ������ ���������� � �����
    cout << "Running test for reading triangles from file..." << endl;
    TriangleList triangles = readTrianglesFromFile("triangles.txt");
    assert(!triangles.isEmpty());
    cout << "Test passed!" << endl << endl;
    // ���� ��� ������� ���������� ������ ���������� �� ������
    cout << "Running test for sorting triangles by area..." << endl;
    sortTrianglesByArea(triangles);
    Node<Triangle>* currentNode = triangles.head;
    double previousArea = currentNode->data.getArea();
    currentNode = currentNode->next;
    while (currentNode != nullptr) {
        assert(currentNode->data.getArea() >= previousArea);
        previousArea = currentNode->data.getArea();
        currentNode = currentNode->next;
    }
    cout << "Test passed!" << endl << endl;

    // ���� ��� ������� ���������� ���������� � ������� ���������� � ����� ������
    cout << "Running test for moving triangles by perimeter..." << endl;
    TriangleList selectedTriangles = moveTrianglesByPerimeter(triangles, 10, 20);
    currentNode = selectedTriangles.head;
    while (currentNode != nullptr) {
        assert(currentNode->data.getPerimeter() >= 10 && currentNode->data.getPerimeter() <= 20);
        currentNode = currentNode->next;
    }
    cout << "Test passed!" << endl << endl;

    // ���� ��� ������� ������ ������ ���������� � ����
    cout << "Running test for writing triangles to file..." << endl;
    writeTrianglesToFile(selectedTriangles, "selected_triangles.txt");
    ifstream inputFile("selected_triangles.txt");
    assert(inputFile.good());
    inputFile.close();
    cout << "Test passed!" << endl << endl;

    cout << "All tests passed!" << endl;
}
int main() {
    // ��������� unit �����
    runUnitTests();

    // ���������� ������ ���������� � �����
    TriangleList triangles = readTrianglesFromFile("triangles.txt");

    // ���������� ������ ���������� �� ������
    sortTrianglesByArea(triangles);
    cout << "Sorted triangles by area:" << endl << triangles << endl;
    // ���������� ���������� � ������� ���������� � ����� ������
    TriangleList selectedTriangles = moveTrianglesByPerimeter(triangles, 10, 20);
    cout << "Selected triangles by perimeter (10-20):" << endl << selectedTriangles << endl;
    // ����� ������ �������� ���������� � ����
    writeTrianglesToFile(selectedTriangles, "selected_triangles.txt");

    return 0;
}
// ���� �� ����� ����������
ostream& operator<<(ostream& os, const Triangle& triangle) {
    os << "Triangle (" << triangle.side1 << ", " << triangle.side2 << ", " << triangle.side3 << ")";
    return os;
}

// ���� �� ����� ������ ����������
ostream& operator<<(ostream& os, const TriangleList& triangles) {
    Node<Triangle>* currentNode = triangles.head;
    while (currentNode != nullptr) {
        os << currentNode->data << endl;
        currentNode = currentNode->next;
    }
    return os;
}

// ���������� ���������� � ������ �����
istream& operator>>(istream& is, Triangle& triangle) {
    is >> triangle.side1 >> triangle.side2 >> triangle.side3;
    return is;
}

// ����� ���������� � ���� ������
ofstream& operator<<(ofstream& ofs, const Triangle& triangle) {
    ofs << triangle.side1 << " " << triangle.side2 << " " << triangle.side3;
    return ofs;
}

// ���������� ���������� � �����
ifstream& operator>>(ifstream& ifs, Triangle& triangle) {
    ifs >> triangle.side1 >> triangle.side2 >> triangle.side3;
    return ifs;
}
