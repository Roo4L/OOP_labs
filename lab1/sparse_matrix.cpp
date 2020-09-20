//
// Created by CopiedWonder on 16.09.2020.
//
#include "sparse_matrix.h"
#include <iostream>
#include <stdexcept>

sparse_matrix::elem::~elem() {
    delete next;
}

sparse_matrix::Line::Line(const int mlen) {
    if (mlen < 1) {
        throw std::invalid_argument("Bad line max length.");
    }
    len = 0;
    head = nullptr;
    max_len = mlen;
}

void sparse_matrix::Line::add(const int j, const float num) {
    len++;
    if (head == nullptr) {
        head = new elem{j, num, nullptr}; //TODO check if correct
    }
    else {
        elem* ptr = head;
        elem* prev = nullptr;
        while (ptr != nullptr && num > ptr->num) {
            prev = ptr;
            ptr = ptr->next;
        }
        elem* new_elem;
        new_elem = new elem{j, num, ptr}; //TODO check if correct
        if (prev != nullptr) {
            prev->next = new_elem;
        }
    }
}

float sparse_matrix::Line::operator[](const int j) const {
    elem* ptr = head;
    while (ptr != nullptr && ptr->j != j) {
        ptr = ptr->next;
    }
    if (ptr == nullptr) {
        return 0;
    }
    else {
        return ptr->num;
    }

}

int sparse_matrix::Line::length() const{
    return len;
}

sparse_matrix::Line::~Line() {
    elem* ptr;
    elem* next = head;
    while (next != nullptr) {
        ptr = next;
        next = ptr->next;
        delete ptr;
    }
}

sparse_matrix::Line& sparse_matrix::Line::operator=(const sparse_matrix::Line& x) {
    delete head;
    len = x.length();
    max_len = x.max_length();
    head = nullptr;
    elem* ptr;
    for (int i = 0; i < max_len; i++) {
        if (x[i] != 0) {
            if (head == nullptr) {
                head = new elem{i, x[i], nullptr};
                ptr = head;
            }
            else {
                ptr->next = new elem{i, x[i], nullptr};
                ptr = ptr->next;
            }
        }
    }
    return *this;
}

int sparse_matrix::Line::max_length() const {
    return max_len;
}

sparse_matrix::Matrix::Matrix(const int n, const int m) {
    if (n < 1 || m < 1) {
        throw std::invalid_argument("Bad matrix params.");
    }
    this->n = n;
    this->m = m;
    rows = new Line[n];
    for (int i = 0; i < n; i++) {
        rows[i] = *(new Line(m));
    }
}

int sparse_matrix::Matrix::hight() {
    return n;
}

int sparse_matrix::Matrix::width() {
    return m;
}

void sparse_matrix::Matrix::add(const int i, const int j, const float num) {
    if (i < 0 || i >= n || j < 0 || j >= m) {
        throw std::out_of_range("Matrix index out of range.");
    }
    rows[i].add(j, num);
}

void sparse_matrix::Matrix::output() const {
    int ptr;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) { //TODO rewrite to avoid excess condition check
            std::cout << (*this)[i][j] << '\t';
        }
        std::cout << std::endl;
    }
}

sparse_matrix::Line& sparse_matrix::Matrix::operator[](const int i) const {
    if (i < 0 || i >= n) {
        throw std::invalid_argument("Bad matrix index.");
    }
    return rows[i];
}

void sparse_matrix::Matrix::swap(const int i, const int j) {
    Line tmp = rows[i];
    rows[i] = rows[j];
    rows[j] = tmp;
}

int sparse_matrix::Matrix::max_signed(bool (*cmp)(const float)) {
    int max = 0;
    int max_index = 0;
    int cur;
    for (int i = 0; i < n; i++) {
        cur = 0;
        for (int j = 0; j < rows[i].length(); j++) {
            if (cmp(rows[i][j])) {
                cur++;
            }
        }
        if (cur > max) {
            max = cur;
            max_index = i;
        }
    }
    return max_index;
}

sparse_matrix::Matrix::~Matrix() {
    delete[] rows;
}

bool sparse_matrix::is_positive(const float x) {
    return (x > 0);
}

bool sparse_matrix::is_negative(const float x) {
    return (x < 0);
}