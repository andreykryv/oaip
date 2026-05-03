#pragma once
#include <stdexcept>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>

// ═══════════════════════════════════════════════════════════════
//  Задание 5 — Deque с блочной структурой
//  Итераторы не инвалидируются при resize.
//  push_back, push_front, pop_back, pop_front — O(1) амортизированно
//  random access — O(1)
// ═══════════════════════════════════════════════════════════════

template<typename T>
class BlockDeque {
public:
    static constexpr int BLOCK_SIZE = 8; // размер блока K

    // Блок из K элементов
    struct Block {
        T data[BLOCK_SIZE];
        Block() {}
    };

private:
    // Кольцевой массив указателей на блоки
    std::vector<Block*> map_;  // указатели на блоки
    int mapBegin_ = 0;  // индекс первого используемого блока в map_
    int mapEnd_   = 0;  // индекс блока ПОСЛЕ последнего

    // Внутри блоков: begin_ и end_ — абсолютные позиции в логических единицах
    int begin_  = 0;   // абсолютная позиция первого элемента
    int end_    = 0;   // абсолютная позиция ПОСЛЕ последнего

    int totalBlocks_ = 0;  // выделено блоков

    // Получить элемент по абсолютной позиции
    T& at(int absPos) {
        int blockIdx = absPos / BLOCK_SIZE;
        int offset   = absPos % BLOCK_SIZE;
        int mapIdx   = (mapBegin_ + blockIdx) % (int)map_.size();
        return map_[mapIdx]->data[offset];
    }
    const T& at(int absPos) const {
        int blockIdx = absPos / BLOCK_SIZE;
        int offset   = absPos % BLOCK_SIZE;
        int mapIdx   = (mapBegin_ + blockIdx) % (int)map_.size();
        return map_[mapIdx]->data[offset];
    }

    void growBack() {
        // Нужен новый блок справа
        ensureCapacity(mapEnd_ - mapBegin_ + 2);
        map_[(mapBegin_ + (mapEnd_ - mapBegin_)) % map_.size()] = new Block();
        ++mapEnd_;
    }

    void growFront() {
        ensureCapacity(mapEnd_ - mapBegin_ + 2);
        // Двигаем mapBegin_ назад (кольцо)
        mapBegin_ = (mapBegin_ - 1 + (int)map_.size()) % (int)map_.size();
        map_[mapBegin_] = new Block();
    }

    void ensureCapacity(int neededBlocks) {
        if ((int)map_.size() >= neededBlocks + 2) return;
        int newCap = std::max((int)map_.size() * 2, neededBlocks + 4);
        std::vector<Block*> newMap(newCap, nullptr);
        // Копируем блоки по порядку
        int n = mapEnd_ - mapBegin_;
        for (int i = 0; i < n; ++i) {
            newMap[i] = map_[(mapBegin_ + i) % (int)map_.size()];
        }
        map_ = std::move(newMap);
        mapBegin_ = 0;
        mapEnd_   = n;
    }

public:
    BlockDeque() {
        map_.resize(4, nullptr);
        // начальный блок
        map_[0] = new Block();
        mapBegin_ = 0; mapEnd_ = 1;
        // Начинаем с середины блока для равновесия
        begin_ = BLOCK_SIZE / 2;
        end_   = BLOCK_SIZE / 2;
    }

    ~BlockDeque() {
        for (auto* b : map_) if (b) delete b;
    }

    void push_back(const T& val) {
        // Проверяем, есть ли место в последнем блоке
        int lastBlockEnd = end_ % BLOCK_SIZE;
        if (end_ >= (mapEnd_ - mapBegin_) * BLOCK_SIZE) {
            growBack();
        }
        at(end_) = val;
        ++end_;
    }

    void push_front(const T& val) {
        if (begin_ == 0) {
            // Нет места слева — вставляем новый блок
            int offset = begin_ % BLOCK_SIZE;
            if (offset == 0) growFront();
            // Сдвигаем все позиции
            int addedCells = BLOCK_SIZE;
            begin_ += addedCells;
            end_   += addedCells;
        }
        --begin_;
        at(begin_) = val;
    }

    void pop_back() {
        if (empty()) throw std::underflow_error("Deque is empty");
        --end_;
    }

    void pop_front() {
        if (empty()) throw std::underflow_error("Deque is empty");
        ++begin_;
    }

    T& operator[](int i) { return at(begin_ + i); }
    const T& operator[](int i) const { return at(begin_ + i); }

    T& front() { return operator[](0); }
    T& back()  { return operator[](size()-1); }

    int  size()  const { return end_ - begin_; }
    bool empty() const { return end_ == begin_; }

    void clear() {
        begin_ = end_ = BLOCK_SIZE / 2;
        // Reset to single block
        for (int i = 1; i < (int)map_.size(); ++i) {
            if (map_[i]) { delete map_[i]; map_[i] = nullptr; }
        }
        mapBegin_ = 0; mapEnd_ = 1;
        if (!map_[0]) map_[0] = new Block();
    }

    std::vector<T> toVector() const {
        std::vector<T> v;
        v.reserve(size());
        for (int i = 0; i < size(); ++i) v.push_back(operator[](i));
        return v;
    }

    // Итератор с random access, не инвалидируется при resize
    struct Iterator {
        BlockDeque* dq;
        int pos; // абсолютная позиция

        T& operator*() { return dq->at(pos); }
        Iterator& operator++() { ++pos; return *this; }
        Iterator& operator--() { --pos; return *this; }
        Iterator operator+(int n) const { return {dq, pos + n}; }
        bool operator==(const Iterator& o) const { return pos == o.pos; }
        bool operator!=(const Iterator& o) const { return pos != o.pos; }
    };

    Iterator begin_iter() { return {this, begin_}; }
    Iterator end_iter()   { return {this, end_}; }

    // Статистика по блокам
    struct Stats {
        int totalBlocks;
        int usedElements;
        int capacity;
        int blockSize;
    };
    Stats stats() const {
        return { mapEnd_ - mapBegin_,
                 size(),
                 (mapEnd_ - mapBegin_) * BLOCK_SIZE,
                 BLOCK_SIZE };
    }
};