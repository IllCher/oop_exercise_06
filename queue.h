#ifndef QUEUE_H
#define QUEUE_H
#include <memory>
#include <cstdint>
#include <iterator>
namespace containers {

    template<class T, class Allocator = std::allocator<T>>
    class queue {
    private:
        struct lst_node;
        size_t size_= 0;
    public:
        queue() = default;

        class iterator {
        public:
            using value_type = T;
            using reference = T&;
            using pointer = T*;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::forward_iterator_tag;
            explicit iterator(lst_node* ptr);
            T& operator*();
            iterator& operator++();
            iterator operator++(int);
            bool operator== (const iterator& other) const;
            bool operator!= (const iterator& other) const;
        private:
            lst_node* item_;
            friend queue;
        };

        iterator begin();
        iterator end();
        void push(const T& value);
        T& top();
        void pop();
        size_t size();
        void it_rmv(iterator d_it);
        void it_insert(iterator ins_it, T& value);
    private:
        using allocator_type = typename Allocator::template rebind<lst_node>::other;

        struct deleter {
            deleter(allocator_type* allocator): allocator_(allocator) {}

            void operator() (lst_node* ptr) {
                if (ptr != nullptr) {
                    std::allocator_traits<allocator_type>::destroy(*allocator_, ptr);
                    allocator_->deallocate(ptr, 1);
                }
            }

        private:
            allocator_type* allocator_;
        };

        using unique_ptr = std::unique_ptr<lst_node, deleter>;

        struct lst_node {
            T value;
            unique_ptr next{nullptr, deleter{nullptr}};
            lst_node(const T& value_): value(value_) {}
            iterator next_();
        };

        allocator_type allocator_{};
        unique_ptr head_{nullptr, deleter{nullptr}};
        lst_node* tail_ = nullptr;
    };

    template<class T, class Allocator>
    typename queue<T, Allocator>::iterator queue<T, Allocator>::begin() {
        return iterator(head_.get());
    }

    template<class T, class Allocator>
    typename queue<T, Allocator>::iterator queue<T, Allocator>::end() {
        return iterator(nullptr);
    }

    template<class T, class Allocator>
    size_t queue<T, Allocator>::size() {
        return size_;
    }

    template<class T, class Allocator>
    void queue<T, Allocator>::push(const T &value) {
        lst_node* result = this->allocator_.allocate(1);
        std::allocator_traits<allocator_type>::construct(this->allocator_, result, value);
        if (!size_) {
            head_ = unique_ptr(result, deleter{&this->allocator_});
            tail_ = head_.get();
            size_++;
            return;
        }
        tail_->next = unique_ptr(result, deleter{&this->allocator_});
        tail_ = tail_->next.get();
        size_++;
    }

    template<class T, class Allocator>
    void queue<T, Allocator>::pop() {
        if (size_== 0) {
            throw std::logic_error ("can`t pop from empty queue");
        }
        head_ = std::move(head_->next);
        size_--;
    }
    template<class T, class Allocator>
    T& queue<T, Allocator>::top() {
        if (size_== 0) {
            throw std::logic_error ("empty");
        }
        return head_->value;
    }

    template<class T, class Allocator>
    void queue<T, Allocator>::it_rmv(containers::queue<T, Allocator>::iterator d_it) {
        iterator i = this->begin(), end = this->end();
        if (d_it == end) throw std::logic_error ("out of borders");
        if (d_it == this->begin()) {
            this->pop();
            return;
        }
        while((i.item_ != nullptr) && (i.item_->next_() != d_it)) {
            ++i;
        }
        if (i.item_ == nullptr) throw std::logic_error ("out of borders");
        i.item_->next = std::move(d_it.item_->next);
        size_--;
    }

    template<class T, class Allocator>
    void queue<T, Allocator>::it_insert(containers::queue<T, Allocator>::iterator ins_it, T& value) {
        /*auto tmp = std::unique_ptr<lst_node>(new lst_node{value});
        iterator i = this->begin();
        if (ins_it == this->begin()) {
            tmp->next = std::move(head_.get());
            head_.get() = std::move(tmp);
            size_++;
            return;
        }
        while((i.item_ != nullptr) && (i.item_->next_() != ins_it)) {
            ++i;
        }
        if (i.item_ == nullptr) throw std::logic_error ("out of borders");
        tmp->next = std::move(i.item_->next);
        i.item_->next = std::move(tmp);
        size_++;*/
    }


    template<class T, class Allocator>
    typename queue<T, Allocator>::iterator queue<T, Allocator>::lst_node::next_() {
        return iterator(this->next.get());
    }

    template<class T, class Allocator>
    queue<T, Allocator>::iterator::iterator(containers::queue<T, Allocator>::lst_node *ptr) {
        item_ = ptr;
    }

    template<class T, class Allocator>
    T& queue<T, Allocator>::iterator::operator*() {
        return this->item_->value;
    }

    template<class T, class Allocator>
    typename queue<T, Allocator>::iterator& queue<T, Allocator>::iterator::operator++() {
        if (item_ == nullptr) throw std::logic_error ("out of queue borders");
        *this = item_->next_();
        return *this;
    }

    template<class T, class Allocator>
    typename queue<T, Allocator>::iterator queue<T, Allocator>::iterator::operator++(int) {
        iterator old = *this;
        ++*this;
        return old;
    }

    template<class T, class Allocator>
    bool queue<T, Allocator>::iterator::operator==(const iterator& other) const {
        return item_ == other.item_;
    }

    template<class T, class Allocator>
    bool queue<T, Allocator>::iterator::operator!=(const iterator& other) const {
        return item_ != other.item_;
    }

}


#endif
