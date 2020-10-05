//
// Created by CopiedWonder on 16.09.2020.
//
#include <iostream>
#include <stdexcept>
#include "sparse_matrix.h"

/*
 * Class line definitions
 */
sparse_matrix::Line::Line(const int mlen) {
    if (mlen < 1) { throw std::invalid_argument("Bad line max length."); };
    len = 0;
    head = nullptr;
}

void sparse_matrix::Line::add(const int j, const float num) {
    len++;
    if (head == nullptr) {
        head = new elem{j, num, nullptr};
    }
    else {
        elem* ptr = head;
        elem* prev = nullptr;
        while (ptr != nullptr && j > ptr->j) {
            prev = ptr;
            ptr = ptr->next;
        }
        elem* new_elem;
        new_elem = new elem{j, num, ptr};
        if (prev != nullptr) {
            prev->next = new_elem;
        }
        else {
            head = new_elem;
        }
    }
}

float sparse_matrix::Line::operator[](const int j) const {
    elem* ptr = head;
    while (ptr != nullptr && ptr->j < j) {
        ptr = ptr->next;
    }
    return (ptr == nullptr || ptr->j != j) ? 0 : ptr->num;
}

sparse_matrix::Line& sparse_matrix::Line::copy(sparse_matrix::Line& x, int max_len) {
    delete head;
    len = x.length();
    head = nullptr;
    elem* ptr;
    for (int j = 0; j < max_len; j++) {
        if (x[j] != 0) {
            if (head == nullptr) {
                head = new elem{j, x[j], nullptr};
                ptr = head;
            }
            else {
                ptr->next = new elem{j, x[j], nullptr};
                ptr = ptr->next;
            }
        }
    }
    return *this;
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

/*
 * Class Matrix definitions
 */
sparse_matrix::Matrix::Matrix(const int n, const int m) {
    if (n < 1 || m < 1) { throw std::invalid_argument("Bad matrix params.");};
    this->n = n;
    this->m = m;
    rows = new Line[n];
    Line* tmp;
    for (int i = 0; i < n; i++) {
        tmp = new Line(m);
        rows[i] = *tmp;
        delete tmp;
    }
}

void sparse_matrix::Matrix::add(const int i, const int j, const float num) {
    if (i < 0 || i >= n || j < 0 || j >= m) { throw std::out_of_range("Matrix index out of range.");};
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
    if (i < 0 || i >= n) { throw std::invalid_argument("Bad matrix index.");};
    return rows[i];
}

void sparse_matrix::Matrix::swap(const int i, const int j) {
    Line tmp(this->m);
    tmp.copy((*this)[i], this->m);
    (*this)[i].copy((*this)[j], this->m);
    (*this)[j].copy(tmp, this->m);
}

int sparse_matrix::Matrix::max_signed(bool (*cmp)(const float)) {
    int max = 0;
    int max_index = -1;
    int cur;
    for (int i = 0; i < n; i++) {
        cur = 0;
        for (int j = 0; j < m; j++) {
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
    delete[] rows; // TODO check if memory is really cleaned
}

/*
 * Useful functions definitions
 */
bool sparse_matrix::is_positive(const float x) {
    return (x > 0);
}

bool sparse_matrix::is_negative(const float x) {
    return (x < 0);
}