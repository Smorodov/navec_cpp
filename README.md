##С++ порт программы Navec##

[Исходный код на Питоне](https://github.com/natasha/navec "Navec").


#Пример#

```cpp

#include <locale>
#include <chrono>
#include "navec.h"
//----------------------------------------
//
//----------------------------------------
class Timer
{
public:
    Timer() : beg_(clock_::now()) {}
    void reset() { beg_ = clock_::now(); }
    double elapsed() const
    {
        return std::chrono::duration_cast<second_>
            (clock_::now() - beg_).count();
    }
    void out(std::string message = "")
    {
        double t = elapsed();
        std::cout << message << u8"  Затрачено времени:" << t << u8" [с]" << std::endl;
        reset();
    }
private:
    typedef std::chrono::high_resolution_clock clock_;
    typedef std::chrono::duration<double, std::ratio<1> > second_;
    std::chrono::time_point<clock_> beg_;
};

//----------------------------------------
//
//----------------------------------------
void main(void)
{
    setlocale(LC_ALL, "ru_RU.UTF8");
    std::locale::global(std::locale("ru_RU.UTF8"));
    navec nv;
    nv.loadModel("data/navec_hudlit_v1_12B_500K_300d_100q.npz", "data/words.txt");
    
    std::cout << u8"------------------------" << std::endl;
    std::cout << u8" модель загружена       " << std::endl;
    std::cout << u8"------------------------" << std::endl;
    
    Timer timer;
    std::string word = u8"матрица";
    std::cout << u8"------------------------" << std::endl;
    std::cout << u8"Слова, близкие к слову :" << word << std::endl;
    std::cout << u8"------------------------" << std::endl;
    float threshold = 0.4;

    for (auto item : nv.words_map)
    {
        float sim = nv.similarity(word, item.first);
        if (sim > threshold)
        {
            std::cout << item.first << std::endl;
        }
    }
    
    timer.out();
   
}
```

#Вывод программы#

```
---------------------------
Loding NAVEC model file : data/navec_hudlit_v1_12B_500K_300d_100q.npz
---------------------------
indexes
indexes shape=[500002,100]
-----------------
codes
codes shape=[100,256,3]
vectors=500002
qdim=100
dim=300
centroids=256
------------------------
 модель загружена
------------------------
------------------------
Слова, близкие к слову :матрица
------------------------
matrix
астральная
базовая
виртуальная
генетическая
защитная
институциональная
информационная
компьютерная
личностная
матриц
матрица
матрице
матрицей
матрицу
матрицы
ментальная
первичная
перезагрузка
проекция
структура
сценарная
трехмерная
универсальная
шарры
энергетическая
энергоинформационная
эфирная
ячейка
  Затрачено времени:0.473107 [с]
```