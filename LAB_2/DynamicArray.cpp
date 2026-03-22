#include "DynamicArray.h"

DynamicArray::DynamicArray() : size(0), capacity(2) {
    data = new Date*[capacity];
}
DynamicArray::~DynamicArray() {
    for (int i = 0; i < size; i++)
        delete data[i];      // удаляем каждый объект 
        delete[] data;           // удаляем сам массив указателей
}




void DynamicArray::resize() {
    capacity *= 2;
    Date** temp = new Date*[capacity];
    for (int i = 0; i < size; i++)
        temp[i] = data[i];   // копируем указатели, не объекты
         delete[] data;
         data = temp;
}
void DynamicArray::push(Date* d) {
      if (size == capacity)
        resize();
         data[size++] = d;
}





Date* DynamicArray::get(int index) const {
    return data[index];
}
int DynamicArray::getSize() const {
    return size;
}