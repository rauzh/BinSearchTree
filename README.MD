# BinSearchTree

C++20 Binary Search Tree container class with random access iterator

Реализован шаблонный контейнерный класс, так что можно создавать дерево объектов любого типа, у которого опеределены операции сравнения.
Все перечисленные ниже возможности дерева протестированы с помощью GTest.
При разработке использовались концепты, ексепшнс, абстрактные базовые классы, шаблоны.

## Конструкторы дерева

* Пустой
* По дереву любого сравнимого типа
* По контейнеру любого сравнимого типа
* По итератору (InputIterator)
* По массиву объектов
* По initializer_list  (Tree<float> bst = {2.0, 2.2, 5.2, 1.2})

* Продублировано для операции присваивания =

## Операции дерева

* **ВСТАВКА** (Insert для элемента, Add для элемента/дерева/контейнера/итератора/initializer_list/массива)
* Также продублировано операторами + и +=
* **УДАЛЕНИЕ** (Delete для элемента, Remove для элемента/дерева/контейнера/итератора/initializer_list/массива). Remove для дерева работает как вычитание множеств
* Также продублировано операторами - и -=
* **ПОИСК** (Find. Возвращает итератор на найденный объект)
* **ПЕРЕСЕЧЕНИЕ** (Intersection для дерева)
* Также продублировано операторами & и &=

## Сравнения

* ==, !=, IsEqual, IsNotEqual

## Индексация

* [] для дерева работают, возвращают ссылку на элемент дерева в порядке обхода pre-order

## Итератор

Итератор идет по дереву pre-order обходом. Соответственно операции "перескакивания" через несколько элементов реализуются в порядке как если бы до них дошли этим обходом.

* Категория - random_acess_tag
* Соответственно реализованы операторы ++, --, +, -, +=, -=, ==, !=, >, >=, <, <=, []
* Также реализованы операторы * и ->

### ----------------------------------------------------------------------

TODO: написать в ридми нормальную табличку/доку по возможностям дерева, чтобы пользователю было понятно как его использовать