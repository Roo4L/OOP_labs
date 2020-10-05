//
// Created by CopiedWonder on 16.09.2020.
//

#ifndef LAB1_2_0_SPARSE_MATRIX_H
#define LAB1_2_0_SPARSE_MATRIX_H


namespace sparse_matrix
{
    class Matrix;

    struct elem {
        int j;
        float num;
        elem* next;
        ~elem(){ delete next;};
    };
    class Line
    {
    private:
        int len;
        elem* head;
    public:
        Line(): len(0), head(nullptr) {};
        explicit Line(int mlen);
        void add(int j, float num);
        float operator[](int j) const;
        Line& copy(Line& x, int i);
        int length() const { return len; };
        ~Line();
    };

    class Matrix
    {
    private:
        int n;
        int m;
        Line* rows;
    public:
        Matrix(int n, int m);
        int height() const { return n; };
        int width() const { return m; };
        void add(int i, int j, float num);
        void output() const;
        Line& operator[](const int i) const;
        void swap(const int i, const int j);
        int max_signed(bool (*cmp)(const float));
        ~Matrix();
    };

    bool is_positive(const float x);
    bool is_negative(const float x);
}
#endif //LAB1_2_0_SPARSE_MATRIX_H
