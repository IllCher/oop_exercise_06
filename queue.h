#ifndef QUEUE_H
#define QUEUE_H
#include <memory>
#include <cstdint>
typedef unsigned long long ull;
template <class T>
class queue {
private:
    class iterator;
    struct lst_node;
public:
    using value_type = T;
    using size_type = ull;
    using reference = value_type&;
    queue() = default;
    queue(const queue& q) = delete;
    queue& operator = (const queue&) = delete;
    void push(const T& value) {
        head_ = push_impl(head_, value);
        size_++;
    }
    std::unique_ptr<lst_node> push_impl(std::unique_ptr<lst_node> tmp, const T& value) {
        if (tmp != nullptr) {
            tmp->next = push_impl(tmp->next, value);
            return tmp;
        }
        return new_node(value);
    }

    void pop() {
        if (empty())
            throw std::logic_error("empty");
        head_ = head_->next;
        size_--;
    }

    reference top() {
        if (empty()) {
            throw std::logic_error("empty");
        }
        return head_->value;
    }

    size_type size() {
        return size_;
    }

    bool empty() {
        return size_ == 0;
    }

    iterator begin() {
        return iterator(head_, this);
    }

    iterator end() {
        return iterator(nullptr, this);
    }

    void it_insert(iterator it, const value_type& value) {
        if (it == end()) {
            push(value);
            return;
        }
        std::unique_ptr<lst_node> new_elem = new_node(value);
        if (it == begin()) {
            new_elem->next = head_;
            head_ = new_elem;
            size_++;
            return ;
        }
        iterator i = begin();
        while(i.item_ != nullptr && i.item_->next != it) {
            ++i;
        }
        new_elem->next = i.item_->next;
        i.item_->next = new_elem;
        size_++;
    }
    void it_rmv(iterator it) {
        std::unique_ptr<lst_node> tmp = it.item_;
        if (it == end()) {
            throw std::logic_error("can't remove end iterator");
        }
        if (it == begin()) {
            pop();
            return ;
        }
        iterator i = begin();
        while(i.item_ != nullptr && i.item_->next != it) {
            ++i;
        }
        i.item_->next = it.item_->next;
        size_--;
    }

private:
    struct lst_node {
        lst_node() = default;
        std::unique_ptr<lst_node> next;
        value_type value;

        lst_node(const value_type& val):
                value(val), next(nullptr)
        {}
    };

    class iterator {
    public:
        using difference_type = ull;
        using value_type = queue::value_type;
        using reference = queue::value_type&;
        using pointer = queue::value_type*;
        using iterator_category = std::forward_iterator_tag;

        iterator(lst_node* item, queue const * lst): item_(item), lst_(lst)
        {}


        iterator(const iterator& it) {
            item_ = it.item_;
            lst_ = it.lst_;
        }

        ~iterator() = default;

        iterator& operator= (const iterator& it) {
            item_ = it.item_;
            return *this;
        }

        iterator& operator++ () {
            std::unique_ptr<lst_node> tmp = item_;
            if (tmp) {
                if (tmp->next == nullptr) {
                    throw std::logic_error("out of bounds");
                }
                tmp = tmp->next;
                item_ = tmp;
                return  *this;
            }
            throw std::logic_error("smt strange");
        }
        iterator operator++ (int) {
            iterator res(*this);
            ++(*this);
            return res;
        }
        reference operator*() {
            return item_->value;
        }

        pointer operator->() {
            return &item_->value;
        }

        bool operator!= (const iterator& example) {
            return !(*this == example);
        }

        bool operator== (const iterator& example) {
            return item_ == example.item_;
        }
    private:
        lst_node* item_;
        queue const *lst_;
        friend class queue;
    };
    std::unique_ptr<lst_node> head_;
    ull size_ = 0;
    std::unique_ptr<lst_node> new_node(const value_type& value) {
        return std::make_unique<lst_node>(value);
    }
};

#endif
