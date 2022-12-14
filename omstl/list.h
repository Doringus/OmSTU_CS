#pragma once

#include <memory>
#include <list>
#include <iostream>

#include "iterator.h"

namespace  omstl {

    struct listNodeBase_t {
        listNodeBase_t* next = nullptr;
        listNodeBase_t* prev = nullptr;

        void insert_before(listNodeBase_t& node) noexcept {
            node.next = this;
            node.prev = this->prev;
            this->prev = &node;
        }
    };

    static void swap_nodes(listNodeBase_t& lhs, listNodeBase_t& rhs) noexcept {
        lhs.prev->next = &rhs;
        rhs.prev = lhs.prev;
        rhs.next->prev = &lhs;
        lhs.next = rhs.next;
        rhs.next = &lhs;
        lhs.prev = &rhs;
    }

    static void cut_segment(listNodeBase_t& lhs, listNodeBase_t& rhs) noexcept {
        lhs.prev->next = rhs.next;
        rhs.next->prev = lhs.prev;
        lhs.prev = nullptr;
        rhs.next = nullptr;
    }

    template <typename T>
    struct listNode_t : public listNodeBase_t {
        alignas(T) unsigned char data[sizeof(T)];
    };

    template <typename T, typename Pointer, typename Reference>
    class ListIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = Pointer;
        using reference         = Reference;
        using node_type         = listNode_t<value_type>;

        constexpr explicit ListIterator(const listNodeBase_t* node) noexcept;
        constexpr ListIterator(const ListIterator& other) noexcept;

        constexpr reference operator*() const noexcept;
        constexpr pointer operator->() const noexcept;

        constexpr ListIterator<T, Pointer, Reference>& operator++() noexcept;
        constexpr ListIterator<T, Pointer, Reference> operator++(int) & noexcept;

        constexpr ListIterator<T, Pointer, Reference>& operator--() noexcept;
        constexpr ListIterator<T, Pointer, Reference> operator--(int) & noexcept;

        friend auto operator<=>(const ListIterator& lhs, const ListIterator& rhs) = default;

    public:
        node_type* m_Node;
    };

    template <typename T, typename Alloc = std::allocator<listNode_t<T>>>
    class List {
    public:
        using iterator = ListIterator<T, T*, T&>;
        using const_iterator = ListIterator<T, T const *, T const &>;
        using reverse_iterator = ReverseIterator<iterator>;
        using const_reverse_iterator = ReverseIterator<const_iterator>;
    public:

        List() {
            m_End.prev = &m_End;
            m_End.next = &m_End;
        }

        constexpr iterator begin() noexcept;
        constexpr const_iterator begin() const noexcept;
        constexpr const_iterator cbegin() const noexcept;

        constexpr iterator end() noexcept;
        constexpr const_iterator end() const noexcept;
        constexpr const_iterator cend() const noexcept;

        constexpr decltype(auto) rbegin() noexcept {
        #ifdef OMSTL_ITERATORS
            return reverse_iterator(end());
        #else
            return std::make_reverse_iterator(end());
        #endif
        }
        constexpr decltype(auto) rbegin() const noexcept {
        #ifdef OMSTL_ITERATORS
            return reverse_iterator(end());
        #else
            return std::make_reverse_iterator(end());
        #endif
        }
        constexpr decltype(auto) crbegin() const noexcept {
        #ifdef OMSTL_ITERATORS
            return reverse_iterator(cend());
        #else
            return std::make_reverse_iterator(cend());
        #endif
        }

        constexpr decltype(auto) rend() noexcept {
        #ifdef OMSTL_ITERATORS
            return reverse_iterator(begin());
        #else
            return std::make_reverse_iterator(begin());
        #endif
        }
        constexpr decltype(auto) rend() const noexcept {
        #ifdef OMSTL_ITERATORS
            return reverse_iterator(begin());
        #else
            return std::make_reverse_iterator(begin());
        #endif
        }
        constexpr decltype(auto) crend() const noexcept {
        #ifdef OMSTL_ITERATORS
            return reverse_iterator(cbegin());
        #else
            return std::make_reverse_iterator(cbegin());
        #endif
        }

        void push_back(T&& value);

        template <typename... Args>
        void emplace_back(Args&&... args);

        void push_front(T&& value);

        template <typename... Args>
        void emplace_front(Args&&... args);

        void sort();

    private:

        void store_back(listNode_t<T>* node) {
            m_End.prev->next = node;
            node->next = &m_End;
            node->prev = m_End.prev;
            m_End.prev = node;
        }

        void store_front(listNode_t<T>* node) {
            m_End.next->prev = node;
            node->next = m_End.next;
            node->prev = &m_End;
            m_End.next = node;
        }

        iterator merge_sort(iterator begin, iterator end, size_t elementsCount);

    private:
        Alloc m_Allocator;
        listNodeBase_t m_End; // fake node
        size_t m_Size = 0;
    };

    //////////////////////////////////////////////////////////////////
    /// ListIterator ///
    /////////////////////////////////////////////////////////////////

    template<typename T, typename Pointer, typename Reference>
    constexpr inline ListIterator<T, Pointer, Reference>::ListIterator(const listNodeBase_t *node) noexcept
        : m_Node(static_cast<node_type*>(const_cast<listNodeBase_t*>(node))) { }

    template<typename T, typename Pointer, typename Reference>
    constexpr inline ListIterator<T, Pointer, Reference>::ListIterator(const ListIterator& other) noexcept
        : m_Node(other.m_Node) { }

    template<typename T, typename Pointer, typename Reference>
    constexpr inline typename ListIterator<T, Pointer, Reference>::reference
    ListIterator<T, Pointer, Reference>::operator*() const noexcept {
        return *(reinterpret_cast<typename ListIterator<T, Pointer, Reference>::pointer>(m_Node->data));
    }

    template<typename T, typename Pointer, typename Reference>
    constexpr inline typename ListIterator<T, Pointer, Reference>::pointer
    ListIterator<T, Pointer, Reference>::operator->() const noexcept {
        return reinterpret_cast<typename ListIterator<T, Pointer, Reference>::pointer>(m_Node->data);
    }

    template<typename T, typename Pointer, typename Reference>
    inline constexpr ListIterator<T, Pointer, Reference>& ListIterator<T, Pointer, Reference>::operator++() noexcept {
        m_Node = static_cast<node_type*>(m_Node->next);
        return *this;
    }

    template<typename T, typename Pointer, typename Reference>
    inline constexpr ListIterator<T, Pointer, Reference> ListIterator<T, Pointer, Reference>::operator++(int) & noexcept{
        auto tmp = *this;
        operator++();
        return tmp;
    }

    template<typename T, typename Pointer, typename Reference>
    inline constexpr ListIterator<T, Pointer, Reference>& ListIterator<T, Pointer, Reference>::operator--() noexcept {
        m_Node = static_cast<node_type*>(m_Node->prev);
        return *this;
    }

    template<typename T, typename Pointer, typename Reference>
    inline constexpr ListIterator<T, Pointer, Reference> ListIterator<T, Pointer, Reference>::operator--(int) & noexcept {
        auto tmp = *this;
        operator--();
        return tmp;
    }

    //////////////////////////////////////////////////////////////////
    /// List ///
    /////////////////////////////////////////////////////////////////

    template<typename T, typename Alloc>
    constexpr inline typename List<T, Alloc>::iterator List<T, Alloc>::begin() noexcept {
        return iterator(m_End.next);
    }

    template<typename T, typename Alloc>
    constexpr inline typename List<T, Alloc>::const_iterator List<T, Alloc>::begin() const noexcept {
        return const_iterator(m_End.next);
    }

    template<typename T, typename Alloc>
    constexpr inline typename List<T, Alloc>::const_iterator List<T, Alloc>::cbegin() const noexcept {
        return const_iterator(m_End.next);
    }

    template<typename T, typename Alloc>
    constexpr inline typename List<T, Alloc>::iterator List<T, Alloc>::end() noexcept {
        return iterator(&m_End);
    }

    template<typename T, typename Alloc>
    constexpr inline typename List<T, Alloc>::const_iterator List<T, Alloc>::end() const noexcept {
        return const_iterator(&m_End);
    }

    template<typename T, typename Alloc>
    constexpr inline typename List<T, Alloc>::const_iterator List<T, Alloc>::cend() const noexcept {
        return const_iterator(&m_End);
    }

    template<typename T, typename Alloc>
    inline void List<T, Alloc>::push_back(T&& value) {
        auto node = m_Allocator.allocate(1);
        new (node->data) T(std::forward<T>(value));
        store_back(node);
        m_Size++;
    }

    template<typename T, typename Alloc>
    template<typename... Args>
    inline void List<T, Alloc>::emplace_back(Args&&... args) {
        auto node = m_Allocator.allocate(1);
        new (node->data) T(std::forward<Args>(args)...);
        store_back(node);
    }

    template<typename T, typename Alloc>
    inline void List<T, Alloc>::push_front(T&& value) {
        auto node = m_Allocator.allocate(1);
        new (node->data) T(std::forward<T>(value));
        store_front(node);
        m_Size++;
    }

    template<typename T, typename Alloc>
    template<typename... Args>
    inline void List<T, Alloc>::emplace_front(Args&&... args) {
        auto node = m_Allocator.allocate(1);
        new (node->data) T(std::forward<Args>(args)...);
        store_front(node);
    }

    template<typename T, typename Alloc>
    void List<T, Alloc>::sort() {
        merge_sort(begin(), end(), m_Size);
    }

    template<typename T, typename Alloc>
    typename List<T, Alloc>::iterator
    List<T, Alloc>::merge_sort(List::iterator lhs, List::iterator rhs, size_t elementsCount) {
        if(elementsCount == 1) {
            return lhs;
        } else if(elementsCount == 2) {
            if(*lhs > *rhs) {
                swap_nodes(*lhs.m_Node, *rhs.m_Node);
                return rhs;
            }
            return lhs;
        }

        size_t mid = elementsCount / 2;
        auto midIt = std::next(lhs, mid);
        lhs = merge_sort(lhs, midIt, mid);
        auto rightPart = merge_sort(midIt, rhs, elementsCount - mid);

        /// Merge segments
        midIt = rightPart;
        iterator result(rhs);
        for(; (lhs != midIt) && (rightPart != rhs); lhs++) {
            if(*lhs > *rightPart) {
                auto subsegmentEnd = std::next(rightPart);
                while((subsegmentEnd != rhs) && (*subsegmentEnd < *lhs)) {
                    subsegmentEnd++;
                }
                auto* nodeSubsegmentBegin = rightPart.m_Node;
                auto* nodeSubsegmentEnd = subsegmentEnd.m_Node;
                rightPart = subsegmentEnd;
                cut_segment(*nodeSubsegmentBegin, *nodeSubsegmentEnd);
                lhs.m_Node->insert_before(*nodeSubsegmentEnd);
            }
        }
        return result;
    }

}