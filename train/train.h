#ifndef LAB2_B_TRAIN_H
#define LAB2_B_TRAIN_H

#include "../container/container.h"

class Train {
private:
    Container *containers = nullptr;
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

    void setCountContainers(int count) { countContainers = count; };

    //  getters
    int getCount() const { return countContainers; };

    int getMaxWeight() const { return maxWeight; };

    int getMaxVolume() const { return maxVolume; };

    const Container *getContainers() const { return containers; };

//    const Train t;
//    t.getContainers()[0] = Container();

    //  метод вывода
//    friend std::ostream &operator<<(std::ostream &s, Train &train);

    //  метод ввода
    friend std::istream &operator>>(std::istream &is, Train &train);

    //  (+=) добавление нового контейнера;
    Train &operator+=(Container);

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

    // перегруженный оператор присваивания
    Train &operator=(const Train &);

    // перемещающий оператор присваивания
    Train &operator=(Train &&) noexcept;

//    std::ostream &operator<<(std::ostream &s);
};

std::ostream &operator<<(std::ostream &s, const Train &train);

#endif //LAB2_B_TRAIN_H
