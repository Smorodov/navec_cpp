﻿
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