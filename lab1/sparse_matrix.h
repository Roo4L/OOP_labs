//
// Created by CopiedWonder on 16.09.2020.
//

#ifndef LAB1_2_0_SPARSE_MATRIX_H
#define LAB1_2_0_SPARSE_MATRIX_H
namespace sparse_matrix
{
    struct elem {
        int j;
        float num;
        elem* next;
        ~elem(){};
    };
    class Line
    {
    private:
        int len;
        int max_len;
        elem* head;
    public:
        Line(): len(0), max_len(0), head(nullptr) {};
        explicit Line(const int mlen);
        void add(const int j, const float num);
        float operator[](const int j) const;
        Line& operator=(const Line& x);
        int length() const;
        int max_length() const;
        ~Line();
    };
    class Matrix
    {
    private:
        int n;
        int m;
        Line* rows;
    public:
        Matrix(const int n, const int m);
        int height() const;
        int width() const;
        void add(const int i, const int j, const float num);
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
