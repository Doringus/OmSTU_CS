#pragma once

#include <memory>
#include <list>
#include <iostream>

namespace  omstl {

    struct listNodeBase_t {
        listNodeBase_t* next = nullptr;
        listNodeBase_t* prev = nullptr;
    };

    template <typename T>
    struct listNode_t : public listNodeBase_t {
        alignas(T) unsigned char data[sizeof(T)];
    };

    template <typename T, typename Alloc = std::allocator<listNode_t<T>>>
    class List {
    public:

        List() {
            m_End.prev = &m_End;
            m_End.next = &m_End;
        }

        void push_back(T&& value) {
            auto node = m_Allocator.allocate(1);
            new (node->data) T(std::forward<T>(value));
            m_Size++;
        }

        void test() {
            auto node = static_cast<listNode_t<T>*>(m_End.next);
            while (node != &m_End) {
                std::cout << *reinterpret_cast<T*>(node->data);
                node = static_cast<listNode_t<T>*>(node->next);
            }
        }

        template <typename... Args>
        void emplace_back(Args&&... args) {
            auto node = m_Allocator.allocate(1);
            new (node->data) T(std::forward<Args>(args)...);
            store_back(node);
        }

        void push_front(T&& value) {
            auto node = m_Allocator.allocate(1);
            new (node->data) T(std::forward<T>(value));
            store_front(node);
            m_Size++;
        }

        template <typename... Args>
        void emplace_front(Args&&... args) {
            auto node = m_Allocator.allocate(1);
            new (node->data) T(std::forward<Args>(args)...);
            store_front(node);
        }

        void sort() {
            
        }

    private:

        void store_back(listNode_t<T>& node) {
            m_End.prev->next = node;
            node->next = &m_End;
            node->prev = m_End.prev;
            m_End.prev = node;
        }

        void store_front(listNode_t<T>& node) {
            m_End.next->prev = node;
            node->next = m_End.next;
            node->prev = &m_End;
            m_End.next = node;
        }

    private:
        Alloc m_Allocator;
        listNodeBase_t m_End; // fake node
        size_t m_Size = 0;
    };
}