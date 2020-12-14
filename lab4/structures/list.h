//
// Created by copiedwonder on 13.12.2020.
//

#ifndef LAB4_LIST_H
#define LAB4_LIST_H
#include <stdexcept>
#include <iterator>

namespace mystl {
    template<typename T>
    class list {
    private:
        struct elem {
            T item;
            elem* next;
            elem* prev;
        };
        elem* pastend = new elem{T(), nullptr, nullptr};
        elem* head = pastend;
        elem* tail = pastend;
        size_t size_;
    public:
        // Initialisation
        list(): list(0) {};
        explicit list(size_t size);
        explicit list(size_t size, const T& value);
        list(const list& other);
        list(list&& other) noexcept;
        ~list();
        // Assignment
        list<T>& operator=(const list& other);
        list<T>& operator=(list&& other) noexcept;
        void assign(size_t count, const T& value);
        template<class InputIt>
                void assign(InputIt first, InputIt last);
        // Element Access
        T& front();
        const T& front() const;
        T& back();
        const T& back() const;
        // Iterators
        class iterator: public std::iterator<std::bidirectional_iterator_tag, T> {
        protected:
            const list<T>& rel_list_;
            elem* el_;
        public:
            iterator(const list<T>& rel_list, elem* el): rel_list_(rel_list), el_(el) {};
            T& operator*() const { return el_->item;}
            T* operator->() const { return &(el_->item);}
            virtual list<T>::iterator& operator++();
            virtual list<T>::iterator& operator--();
            virtual list<T>::iterator operator+(size_t count) const;
            virtual list<T>::iterator operator-(size_t count) const;
            friend bool operator==(const list<T>::iterator& lhs, const list<T>::iterator& rhs);
            friend bool operator!=(const list<T>::iterator& lhs, const list<T>::iterator& rhs);
            friend mystl::list<T>;
        };
        class reverse_iterator: public list<T>::iterator {
        public:
            reverse_iterator(const list& rel_list, elem* el): list<T>::iterator(rel_list, el) {};
            list<T>::reverse_iterator& operator++() override;
            list<T>::reverse_iterator& operator--() override;
            list<T>::reverse_iterator operator+(size_t count) const override;
            list<T>::reverse_iterator operator-(size_t count) const override;
        };

        class const_iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
        protected:
            const list& rel_list_;
            elem* el_;
        public:
            const_iterator(const list& rel_list, elem* el): rel_list_(rel_list), el_(el) {};
            const T& operator*() const { return el_->item;}
            const T* operator->() const { return &(el_->item);}
            virtual list<T>::const_iterator& operator++();
            virtual list<T>::const_iterator& operator--();
            virtual list<T>::const_iterator operator+(size_t count) const;
            virtual list<T>::const_iterator operator-(size_t count) const;
            friend bool operator==(const list<T>::const_iterator& lhs, const list<T>::const_iterator& rhs);
            friend bool operator!=(const list<T>::const_iterator& lhs, const list<T>::const_iterator& rhs);
            friend mystl::list<T>;
        };

        class const_reverse_iterator: public const_iterator {
        private:
            const list& rel_list_;
            elem* el_;
        public:
            const_reverse_iterator(const list& rel_list, elem* el): const_iterator(rel_list, el) {};
            const_reverse_iterator& operator++() override;
            const_reverse_iterator& operator--() override;
            const_reverse_iterator operator+(size_t count) const override;
            const_reverse_iterator operator-(size_t count) const override;
        };

        iterator begin() const;
        const_iterator cbegin() const;
        iterator end() const;
        const_iterator cend() const;
        reverse_iterator rbegin() const;
        const_reverse_iterator crbegin() const;
        reverse_iterator rend() const;
        const_reverse_iterator crend() const;
        // Capacity
        bool empty() const { return !(cbegin() == cend()); };
        size_t size() const { return size_; };
        // Modifiers
        void clear();

        iterator insert(iterator pos, const T& value);
        iterator insert(const_iterator pos, const T& value);
        iterator insert(const_iterator pos, T&& value);
        void insert(iterator pos, size_t count, const T& value);
        template<class InputIt>
        void insert(iterator pos, InputIt first, InputIt last);
        template<class InputIt>
        void insert(const_iterator pos, InputIt first, InputIt last);

        iterator erase(iterator pos);
        iterator erase(const_iterator pos);
        iterator erase(iterator first, iterator last);
        iterator erase(const_iterator first, const_iterator last);

        void push_back(const T& value);
        void push_back(T&& value);
        void pop_back();

        void push_front(const T& value);
        void push_front(T&& value);
        void pop_front();

        void resize(size_t count);
        void resize(size_t count, const T& value);

        void swap(list<T>& other);
        // Operations
        void merge(list<T>& other);
        void merge(list<T>&& other);
        size_t remove(const T& value);
        void reverse();
        size_t unique();
        void sort();
    };

    template<typename T>
    typename list<T>::iterator &list<T>::iterator::operator++() {
        if (*this != rel_list_.end()) {
            this->el_ = this->el_->next;
        }
        else {
            throw std::out_of_range("Iterator out of range.");
        }
        return *this;
    }

    template<typename T>
    typename list<T>::iterator &list<T>::iterator::operator--() {
        if (*this != rel_list_.begin()) {
            this->el_ = this->el_->prev;
        }
        else {
            throw std::out_of_range("Iterator out of range.");
        }
        return *this;
    }

    template<typename T>
    typename list<T>::iterator list<T>::iterator::operator+(size_t count) const {
        list<T>::iterator it = *this;
        for (int i = 0; i < count; i++) {
            if (it == rel_list_.end()) {
                throw std::out_of_range("Iterator out of range.");
            }
            else {
                it.el_ = it.el_->next;
            }
        }
        return it;
    }

    template<typename T>
    typename list<T>::iterator list<T>::iterator::operator-(size_t count) const {
        list<T>::iterator it = *this;
        for (int i = 0; i < count; i++) {
            if (it == rel_list_.begin()) {
                throw std::out_of_range("Iterator out of range.");
            }
            else {
                it.el_ = it.el_->prev;
            }
        }
        return it;
    }

    template<typename T>
    bool operator==(const typename list<T>::iterator &lhs, const typename list<T>::iterator &rhs) {
        return (lhs.el_ == rhs.el_);
    }

    template<typename T>
    bool operator!=(const typename list<T>::iterator &lhs, const typename list<T>::iterator &rhs) {
        return !(lhs == rhs);
    }

    template<typename T>
    typename list<T>::reverse_iterator &list<T>::reverse_iterator::operator++() {
        if (*this != this->rel_list_.rend()) {
            this->el_ = this->el_->prev;
        }
        return *this;
    }

    template<typename T>
    typename list<T>::reverse_iterator &list<T>::reverse_iterator::operator--() {
        if (*this != this->rel_list_.rbegin()) {
            this->el_ = this->el_->next;
        }
        else {
            throw std::out_of_range("Reverse_Iterator out of range.");
        }
        return *this;
    }

    template<typename T>
    typename list<T>::reverse_iterator list<T>::reverse_iterator::operator+(size_t count) const {
        list<T>::reverse_iterator it = *this;
        for (int i = 0; i < count; i++) {
            if (it == this->rel_list_.rend()) {
                throw std::out_of_range("Reverse_Iterator out of range.");
            }
            else {
                it.el_ = it.el_->prev;
            }
        }
        return it;
    }

    template<typename T>
    typename list<T>::reverse_iterator list<T>::reverse_iterator::operator-(size_t count) const {
        list<T>::reverse_iterator it = *this;
        for (int i = 0; i < count; i++) {
            if (it == this->rel_list_.rbegin()) {
                throw std::out_of_range("Reverse_Iterator out of range.");
            }
            else {
                it.el_ = it.el_->next;
            }
        }
        return it;
    }

    template<typename T>
    typename list<T>::const_iterator &list<T>::const_iterator::operator++() {
        if (*this != rel_list_.cend()) {
            this->el_ = this->el_->next;
        }
        else {
            throw std::out_of_range("Const_Iterator out of range.");
        }
        return *this;
    }

    template<typename T>
    typename list<T>::const_iterator &list<T>::const_iterator::operator--() {
        if (*this != rel_list_.cbegin()) {
            this->el_ = this->el_->prev;
        }
        else {
            throw std::out_of_range("Const_Iterator out of range.");
        }
        return *this;
    }

    template<typename T>
    typename list<T>::const_iterator list<T>::const_iterator::operator+(size_t count) const {
        list<T>::const_iterator it = *this;
        for (int i = 0; i < count; i++) {
            if (it == rel_list_.cend()) {
                throw std::out_of_range("Const_Iterator out of range.");
            }
            else {
                it.el_ = it.el_->next;
            }
        }
        return it;
    }

    template<typename T>
    typename list<T>::const_iterator list<T>::const_iterator::operator-(size_t count) const {
        list<T>::const_iterator it = *this;
        for (int i = 0; i < count; i++) {
            if (it == rel_list_.cbegin()) {
                throw std::out_of_range("Const_Iterator out of range.");
            }
            else {
                it.el_ = it.el_->prev;
            }
        }
        return it;
    }

    template<typename T>
    bool operator==(const typename list<T>::const_iterator &lhs, const typename list<T>::const_iterator &rhs) {
        return (lhs.el_ == rhs.el_);
    }

    template<typename T>
    bool operator!=(const typename list<T>::const_iterator &lhs, const typename list<T>::const_iterator &rhs) {
        return !(lhs == rhs);
    }

    template<typename T>
    typename list<T>::const_reverse_iterator &list<T>::const_reverse_iterator::operator++() {
        if (*this != rel_list_.crend()) {
            this->el_ = this->el_->prev;
        }
        else {
            throw std::out_of_range("Const_Reverse_Iterator out of range.");
        }
        return *this;
    }

    template<typename T>
    typename list<T>::const_reverse_iterator &list<T>::const_reverse_iterator::operator--() {
        if (*this != rel_list_.crbegin()) {
            this->el_ = this->el_->next;
        }
        else {
            throw std::out_of_range("Const_Reverse_Iterator out of range.");
        }
        return *this;
    }

    template<typename T>
    typename list<T>::const_reverse_iterator list<T>::const_reverse_iterator::operator+(size_t count) const {
        list<T>::const_reverse_iterator it = *this;
        for (int i = 0; i < count; i++) {
            if (it == rel_list_.crend()) {
                throw std::out_of_range("Const_Reverse_Iterator out of range.");
            }
            else {
                it.el_ = it.el_->prev;
            }
        }
        return it;
    }

    template<typename T>
    typename list<T>::const_reverse_iterator list<T>::const_reverse_iterator::operator-(size_t count) const {
        list<T>::const_reverse_iterator it = *this;
        for (int i = 0; i < count; i++) {
            if (it == rel_list_.crbegin()) {
                throw std::out_of_range("Const_Reverse_Iterator out of range.");
            }
            else {
                it.el_ = it.el_->next;
            }
        }
        return it;
    }

    template<typename T>
    list<T>::list(size_t size) {
        size_ = size;
        if (size < 0) {
            throw std::invalid_argument("Can't construct list with negative size");
        }
        if (size > 0) {
            for (int i = 0; i < size; i++) {
                if (tail != pastend) {
                    tail->next = new elem;
                    tail->next->prev = tail;
                    tail->next->next = pastend;
                    tail = tail->next;
                }
                else { // generate head
                    head = new elem;
                    head->prev = pastend;
                    head->next = pastend;
                    tail = head;
                }
            }
        }
        pastend->next = head;
        pastend->prev = tail;
    }

    template<typename T>
    list<T>::list(size_t size, const T& value) {
        size_ = size;
        if (size < 0) {
            throw std::invalid_argument("Can't construct list with negative size");
        }
        if (size > 0) {
            for (int i = 0; i < size; i++) {
                if (tail != pastend) {
                    tail->next = new elem(value);
                    tail->next->prev = tail;
                    tail->next->next = pastend;
                    tail = tail->next;
                }
                else { // generate head
                    head = new elem(value);
                    head->prev = pastend;
                    head->next = pastend;
                    tail = head;
                }
            }
        }
        pastend->next = head;
        pastend->prev = tail;
    }

    template<typename T>
    list<T>::list(const list &other) {
        size_ = other.size_;
        for (auto& cur : other) {
            if (tail != pastend) {
                tail->next = new elem(*cur);
                tail->next->prev = tail;
                tail->next->next = pastend;
                tail = tail->next;
            }
            else { // generate head
                head = new elem(*cur);
                head->prev = pastend;
                head->next = pastend;
                tail = head;
            }
        }
        pastend->next = head;
        pastend->prev = tail;
    }

    template<typename T>
    list<T>::list(list &&other) noexcept {
        size_ = other.size_;
        head = other.head;
        tail = other.tail;
        pastend = other.pastend;
        other.size_ = 0;
        other.head = nullptr;
        other.tail = nullptr;
    }

    template<typename T>
    list<T>::~list() {
        elem* next = head;
        while (head != pastend) {
            next = head->next;
            delete head;
            head = next;
        }
        size_ = 0;
        tail = nullptr;
        pastend->next = nullptr;
        pastend->prev = nullptr;
    }

    template<typename T>
    list<T>& list<T>::operator=(const list &other) {
        resize(other.size_);
        elem* ptr = head;
        for (auto& cur : other) {
            ptr->item = *cur;
            ptr = ptr->next;
        }
        return *this;
    }

    template<typename T>
    list<T> &list<T>::operator=(list &&other) noexcept {
        resize(other.size_);
        elem* ptr = head;
        for (auto& cur : other) {
            ptr->item = *cur;
            ptr = ptr->next;
        }
        other.clear();
        return *this;
    }

    template<typename T>
    void list<T>::assign(size_t count, const T &value) {
        resize(count);
        for (auto& it : *this) {
            *it = value;
        }
    }

    template<typename T>
    template<class InputIt>
    void list<T>::assign(InputIt first, InputIt last) {
        clear();
        insert(begin(), first, last);
    }

    template<typename T>
    T& list<T>::front() {
        if (empty())
            throw std::out_of_range("List is empty");
        else
            return head->item;
    }

    template<typename T>
    const T &list<T>::front() const {
        if (empty())
            throw std::out_of_range("List is empty");
        else
            return head->item;
    }

    template<typename T>
    T &list<T>::back() {
        if (empty())
            throw std::out_of_range("List is empty");
        else
            return tail->item;
    }

    template<typename T>
    const T &list<T>::back() const {
        if (empty())
            throw std::out_of_range("List is empty");
        else
            return tail->item;
    }

    template<typename T>
    typename list<T>::iterator list<T>::begin() const {
        return mystl::list<T>::iterator(*this, head);
    }

    template<typename T>
    typename list<T>::const_iterator list<T>::cbegin() const {
        return list<T>::const_iterator(*this, head);
    }

    template<typename T>
    typename list<T>::reverse_iterator list<T>::rbegin() const {
        return list<T>::reverse_iterator(*this, tail);
    }

    template<typename T>
    typename list<T>::const_reverse_iterator list<T>::crbegin() const {
        return list<T>::const_reverse_iterator(*this, tail);
    }

    template<typename T>
    typename list<T>::iterator list<T>::end() const {
        return list<T>::iterator(*this, pastend);
    }

    template<typename T>
    typename list<T>::const_iterator list<T>::cend() const {
        return list<T>::const_iterator(*this, pastend);
    }

    template<typename T>
    typename list<T>::reverse_iterator list<T>::rend()  const {
        return list<T>::reverse_iterator(*this, pastend);
    }

    template<typename T>
    typename list<T>::const_reverse_iterator list<T>::crend() const {
        return list<T>::const_reverse_iterator(*this, pastend);
    }

    template<typename T>
    void list<T>::clear() {
        elem* next = head;
        while (head != pastend) {
            next = head->next;
            delete head;
            head = next;
        }
        size_ = 0;
        tail = pastend;
    }

    template<typename T>
    typename list<T>::iterator list<T>::insert(list<T>::iterator pos, const T &value) {
        auto prev = pos - 1;
        pos.el_->prev = new elem{value, pos.el_, prev.el_};
        prev.el_->next = pos.el_->prev;
        if (prev.el_ == pastend)
            head = prev.el_->next;
        if (pos.el_ == pastend)
            tail = pos.el_->prev;
        return list<T>::iterator(*this, pos.el_->prev);
    }

    template<typename T>
    typename list<T>::iterator list<T>::insert(list<T>::const_iterator pos, const T &value) {
        auto prev = pos - 1;
        pos.el_->prev = new elem{value, pos.el_, prev.el_};
        prev->next = pos.el_->prev;
        if (prev.el_ == pastend)
            head = prev.el_->next;
        if (pos.el_ == pastend)
            tail = pos.el_->prev;
        return list<T>::iterator(*this, pos.el_->prev);
    }

    template<typename T>
    typename list<T>::iterator list<T>::insert(list<T>::const_iterator pos, T &&value) {
        auto prev = pos - 1;
        pos.el_->prev = new elem{move(value), pos.el_, prev.el_};
        prev->next = pos.el_->prev;
        if (prev.el_ == pastend)
            head = prev.el_->next;
        if (pos.el_ == pastend)
            tail = pos.el_->prev;
        return list<T>::iterator(*this, pos.el_->prev);
    }

    template<typename T>
    void list<T>::insert(list::iterator pos, size_t count, const T &value) {
        auto prev = pos - 1;
        for (size_t i = 0; i < count; i++){
            pos.el_->prev = new elem{value, pos.el_, prev.el_};
            prev->next = pos.el_->prev;
            prev++;
        }
        if (prev.el_ == pastend)
            head = prev.el_->next;
        if (pos.el_ == pastend)
            tail = pos.el_->prev;
    }

    template<typename T>
    template<class InputIt>
    void list<T>::insert(list::iterator pos, InputIt first, InputIt last) {
        auto prev = pos - 1;
        for (InputIt it = first; it != last; ++it) {
            pos.el_->prev = new elem{*it, pos.el_, prev.el_};
            prev->next = pos.el_->prev;
            prev++;
        }
        if (prev.el_ == pastend)
            head = prev.el_->next;
        if (pos.el_ == pastend)
            tail = pos.el_->prev;
    }

    template<typename T>
    template<class InputIt>
    void list<T>::insert(list::const_iterator pos, InputIt first, InputIt last) {
        auto prev = pos - 1;
        for (InputIt it = first; it != last; ++it) {
            pos.el_->prev = new elem{*it, pos.el_, prev.el_};
            prev->next = pos.el_->prev;
            prev++;
        }
        if (prev.el_ == pastend)
            head = prev.el_->next;
        if (pos.el_ == pastend)
            tail = pos.el_->prev;
    }

    template<typename T>
    typename list<T>::iterator list<T>::erase(list<T>::iterator pos) {
        if (pos.el_ != pastend) {
            auto prev = pos - 1;
            auto post = pos + 1;
            prev.el_->next = post.el_;
            post.el_->prev = prev.el_;
            delete pos.el_;
            if (prev.el_ == pastend)
                head = prev.el_->next;
            if (post.el_ == pastend)
                tail = post.el_->prev;
            return post;
        }
        else {
            throw std::invalid_argument("Can't erase past-the-end element.");
        }
    }

    template<typename T>
    typename list<T>::iterator list<T>::erase(list<T>::const_iterator pos) {
        if (pos.el_ != pastend) {
            auto prev = pos - 1;
            auto post = pos + 1;
            prev.el_->next = post.el_;
            post.el_->prev = prev.el_;
            delete pos.el_;
            if (prev.el_ == pastend)
                head = prev.el_->next;
            if (post.el_ == pastend)
                tail = post.el_->prev;
            return iterator(this, post.el_);
        }
        else {
            throw std::invalid_argument("Can't erase past-the-end element.");
        }
    }

    template<typename T>
    typename list<T>::iterator list<T>::erase(list::iterator first, list::iterator last) {
        auto prev = first - 1;
        auto it = first;
        auto next = it;
        while (it != last) {
            next = it + 1;
            delete it.el_;
            it = next;
        }
        prev.el_->next = last.el_;
        last.el_->prev = prev.el_;
        if (prev.el_ == pastend)
            head = prev.el_->next;
        if (last.el_ == pastend)
            tail = last.el_->prev;
        return last;
    }

    template<typename T>
    typename list<T>::iterator list<T>::erase(list::const_iterator first, list::const_iterator last) {
        auto prev = first - 1;
        auto it = first;
        auto next = it;
        while (it != last) {
            next = it + 1;
            delete it.el_;
            it = next;
        }
        prev.el_->next = last.el_;
        last.el_->prev = prev.el_;
        if (prev.el_ == pastend)
            head = prev.el_->next;
        if (last.el_ == pastend)
            tail = last.el_->prev;
        return iterator(this, last.el_);
    }

    template<typename T>
    void list<T>::push_back(const T &value) {
        this->insert(this->end(), value);
    }

    template<typename T>
    void list<T>::push_back(T &&value) {
        this->insert(this->end(), move(value));
    }

    template<typename T>
    void list<T>::pop_back() {
        this->erase(this->end());
    }

    template<typename T>
    void list<T>::push_front(const T &value) {
        this->insert(this->begin(), value);
    }

    template<typename T>
    void list<T>::push_front(T &&value) {
        this->insert(this->begin(), move(value));
    }

    template<typename T>
    void list<T>::pop_front() {
        this->erase(this->begin());
    }

    template<typename T>
    void list<T>::resize(size_t count) {
        if (count > size()) {
            this->insert(this->end(), size() - count, T());
        }
        else if (count < size()) {
            this->erase(this->end() - int(size() - count), this->end());
        }
    }

    template<typename T>
    void list<T>::resize(size_t count, const T &value) {
        if (count > size()) {
            this->insert(this->end(), size() - count, value);
        }
        else if (count < size()) {
            this->erase(this->end() - int(size() - count), this->end());
        }
    }

    template<typename T>
    void list<T>::swap(list<T> &other) {
        size_t tmp_size = size_;
        elem* tmp_head = head;
        elem* tmp_tail = tail;
        elem* tmp_pastend = pastend;

        size_ = other.size_;
        head = other.head;
        tail = other.tail;
        pastend = other.pastend;

        other.size_ = tmp_size;
        other.head = tmp_head;
        other.tail = tmp_tail;
        other.pastend = tmp_pastend;
    }

    template<typename T>
    void list<T>::merge(list<T> &other) {
        tail->next = other.head;
        tail = other.tail;
        pastend->pred = tail;
        size_ += other.size_;

        other.size_ = 0;
        other.head = other.pastend;
        other.tail = other.pastend;
    }

    template<typename T>
    void list<T>::merge(list<T> &&other) {
        tail->next = other.head;
        tail = other.tail;
        pastend->pred = tail;
        size_ += other.size_;

        other.size_ = 0;
        other.head = other.pastend;
        other.tail = other.pastend;
    }

    template<typename T>
    size_t list<T>::remove(const T &value) {
        size_t counter = 0;
        iterator cur = begin();
        iterator next = cur;
        while (cur != end()) {
            next = cur + 1;
            if (*cur == value) {
                erase(cur);
                counter++;
            }
            cur = next;
        }
        return counter;
    }

    template<typename T>
    void list<T>::reverse() {
        elem* curptr = head;
        elem* tmpptr;
        while (curptr->prev != pastend) {
            tmpptr = curptr->next;
            curptr->next = curptr->prev;
            curptr->prev = tmpptr;
            curptr = curptr->prev;
        }
        // swap pastend
        tmpptr = curptr->next;
        curptr->next = curptr->prev;
        curptr->prev = tmpptr;
    }

    template<typename T>
    size_t list<T>::unique() {
        if (size_ == 0)
            return 0;
        size_t counter = 0;
        list<T>::iterator cur = begin(), next = begin() + 1;
        while (next != end()) {
            if (*cur == *next) {
                this->erase(cur);
                counter++;
            }
            cur = next;
            next++;
        }
        return counter;
    }

    template<typename T>
    void list<T>::sort() {
        list<T>::iterator cur = begin();
        list<T>::iterator next;
        while (cur != end()) {
            next = cur + 1;
            while (next != end()) {
                if (*next < *cur) {
                    T tmp = *next;
                    *next = *cur;
                    *cur = tmp;
                }
                next++;
            }
            cur++;
        }
    }
}

#endif //LAB4_LIST_H
