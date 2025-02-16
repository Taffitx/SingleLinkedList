#pragma once 
#include <cassert> 
#include <cstddef> 
#include <string> 
#include <utility> 
#include <iterator> 

template <typename Type> 
class SingleLinkedList { 
private: 
    struct Node { 
        Node() = default; 
        Node(const Type& val, Node* next) 
            : value(val) 
            , next_node(next) { 
        } 
        Type value; 
        Node* next_node = nullptr; 
    }; 

    template <typename ValueType> 
    class BasicIterator { 
        friend class SingleLinkedList; 

        explicit BasicIterator(Node* node) : node_(node) {}

    public: 
        using iterator_category = std::forward_iterator_tag; 
        using value_type = Type; 
        using difference_type = std::ptrdiff_t; 
        using pointer = ValueType*; 
        using reference = ValueType&; 

        BasicIterator() = default; 

        BasicIterator(const BasicIterator<Type>& other) noexcept : node_(other.node_) {}

        BasicIterator& operator=(const BasicIterator& rhs) = default; 

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept { 
            return node_ == rhs.node_; 
        } 

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept { 
            return !(*this == rhs); 
        } 

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept { 
            return node_ == rhs.node_; 
        } 

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept { 
            return !(*this == rhs); 
        } 

        BasicIterator& operator++() noexcept { 
            assert(node_); 
            node_ = node_->next_node; 
            return *this; 
        } 

        BasicIterator operator++(int) noexcept { 
            BasicIterator old = *this; 
            ++(*this); 
            return old; 
        } 

        [[nodiscard]] reference operator*() const noexcept { 
            assert(node_); 
            return node_->value; 
        } 

        [[nodiscard]] pointer operator->() const noexcept { 
            assert(node_); 
            return &node_->value; 
        } 

    private: 
        Node* node_ = nullptr; 
    }; 

public: 
    using value_type = Type; 
    using reference = value_type&; 
    using const_reference = const value_type&; 
    using Iterator = BasicIterator<Type>; 
    using ConstIterator = BasicIterator<const Type>; 

    [[nodiscard]] Iterator before_begin() noexcept { 
        return Iterator(&head_); 
    } 

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept { 
        return ConstIterator(const_cast<Node*>(&head_)); 
    } 

    [[nodiscard]] ConstIterator before_begin() const noexcept { 
        return ConstIterator(const_cast<Node*>(&head_)); 
    } 

    Iterator InsertAfter(ConstIterator pos, const Type& value) { 
        assert(pos.node_); 
        Node* pos_node = const_cast<Node*>(pos.node_); 
        pos_node->next_node = new Node(value, pos_node->next_node); 
        ++size_; 
        return Iterator(pos_node->next_node); 
    } 

    void PopFront() noexcept { 
        assert(!IsEmpty()); 
        Node* temp = head_.next_node; 
        head_.next_node = temp->next_node; 
        delete temp; 
        --size_; 
    } 

    Iterator EraseAfter(ConstIterator pos) noexcept { 
        assert(pos.node_ && pos.node_->next_node && size_ > 0); 
        Node* pos_node = const_cast<Node*>(pos.node_); 
        Node* temp = pos_node->next_node; 
        pos_node->next_node = temp->next_node; 
        delete temp; 
        --size_; 
        return Iterator(pos_node->next_node); 
    } 

    [[nodiscard]] Iterator begin() noexcept { 
        return Iterator(head_.next_node); 
    } 

    [[nodiscard]] Iterator end() noexcept { 
        return Iterator(nullptr); 
    } 

    [[nodiscard]] ConstIterator begin() const noexcept { 
        return cbegin(); 
    } 

    [[nodiscard]] ConstIterator end() const noexcept { 
        return cend(); 
    } 

    [[nodiscard]] ConstIterator cbegin() const noexcept { 
        return ConstIterator(head_.next_node); 
    } 

    [[nodiscard]] ConstIterator cend() const noexcept { 
        return ConstIterator(nullptr); 
    } 

    SingleLinkedList() : head_(), size_(0u) {} 

    SingleLinkedList(std::initializer_list<Type> values) : SingleLinkedList() { 
        InitFromRange(values.begin(), values.end()); 
    } 

    SingleLinkedList(const SingleLinkedList& other) : SingleLinkedList() { 
        InitFromRange(other.begin(), other.end()); 
    } 

    SingleLinkedList& operator=(const SingleLinkedList& rhs) { 
        if (this != &rhs) { 
            SingleLinkedList temp(rhs); 
            swap(temp); 
        } 
        return *this; 
    } 

    void swap(SingleLinkedList& other) noexcept { 
        std::swap(size_, other.size_); 
        std::swap(head_.next_node, other.head_.next_node); 
    } 

    void PushFront(const Type& value) { 
        head_.next_node = new Node(value, head_.next_node); 
        ++size_; 
    } 

    void Clear() noexcept { 
        while (head_.next_node != nullptr) { 
            auto* next = head_.next_node->next_node; 
            delete head_.next_node; 
            head_.next_node = next; 
        } 
        size_ = 0u; 
    } 

    [[nodiscard]] size_t GetSize() const noexcept { 
        return size_; 
    } 

    [[nodiscard]] bool IsEmpty() const noexcept { 
        return size_ == 0u; 
    } 

    ~SingleLinkedList() noexcept { 
        Clear(); 
    } 

private: 
    Node head_; 
    size_t size_; 

    template <typename InputIt> 
    void InitFromRange(InputIt first, InputIt last) { 
        SingleLinkedList temp_list; 
        Node* hold_node_ = &temp_list.head_; 

        while (first != last) { 
            hold_node_->next_node = new Node(*first++, nullptr); 
            hold_node_ = hold_node_->next_node; 
            ++temp_list.size_; 
        } 
        swap(temp_list); 
    } 
}; 

template <typename Type> 
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept { 
    lhs.swap(rhs); 
} 

template <typename Type> 
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) { 
    if (&lhs == &rhs) { 
        return true; 
    } 
    if (lhs.GetSize() != rhs.GetSize()) { 
        return false; 
    } 
    return std::equal(lhs.begin(), lhs.end(), rhs.begin()); 
} 

template <typename Type> 
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) { 
    return !(lhs == rhs); 
} 

template <typename Type> 
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) { 
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); 
} 

template <typename Type> 
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) { 
    return !(rhs < lhs); 
} 

template <typename Type> 
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) { 
    return rhs < lhs; 
} 

template <typename Type> 
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) { 
    return !(lhs < rhs); 
}