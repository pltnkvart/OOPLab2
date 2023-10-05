#ifndef LAB2_B_TRAIN_H
#define LAB2_B_TRAIN_H

#include "../container/container.h"

class Train {
private:
    Container *containers;
    int countContainers;
    double maxWeight;
    double maxVolume;
public:
    //  конструктор по умолчанию (явный или неявный);
    Train();

    //  создание экземпляров класса с инициализацией заданным количеством контейнеров из массива контейнеров;
    Train(int, Container (&containers)[], double, double);

    //  создание экземпляров класса с инициализацией одним контейнером;
    Train(Container &, double, double);

    //  копирующий конструктор
    Train(const Train &);

    //  перемещающий конструктор
    Train(Train &&) noexcept;

    //  деструктор
    ~Train() { delete[] containers; };

    //  setters
    void setMaxWeight(double weight) { maxWeight = weight; };

    void setMaxVolume(double volume) { maxVolume = volume; };

    //  getters
    int getCount() { return countContainers; };

    Container *getContainers() { return containers; };

    //  методы ввода и вывода состояния класса в входной/выходной поток;
    friend std::istream &operator>>(std::istream &is, Train &train);

    //  вывод
    void printState();

    //  (+=) добавление нового контейнера;
    void operator+=(Container &);

    //  ([]) получение контейнера по его номеру (возврат по ссылке);
    Container &operator[](int);

    //  удаление контейнера по его номеру;
    void deleteContainer(int);

    //  подсчёт суммарной массы всех контейнеров;
    double countMass();

    //  вычисление центра массы поезда, измеряемой в контейнерах (считаем, что все контейнеры имеют одинаковую длину);
    double countCenterMass();

    //  минимизация количества контейнеров за счёт перегрузки груза между ними и удаления пустых контейнеров;
    void minContainers();

    //  обеспечение безопасности транспортировки опасных грузов — между двумя контейнерами с опасным грузом должно находиться не менее двух контейнеров с безопасным грузом (обеспечить при помощи перемещения контейнеров, при необходимости добавить пустые контейнеры).
    void ensuringSecurity();
};

std::ostream &operator<<(std::ostream &s, Train &train);

#endif //LAB2_B_TRAIN_H
