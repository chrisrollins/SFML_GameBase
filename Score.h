//
//  Score.hpp
//  Project_XCODE
//
//  Created by Jakin Wang on 2/27/19.
//  Copyright © 2019 Jakin Wang. All rights reserved.
//

#ifndef Score_hpp
#define Score_hpp

#include <stdio.h>
#include "Screen.h"
#include "SampleUIText.h"

class Score : public SampleUIText{
   
public:
    Score(std::string text) : SampleUIText(text), number(0){}
    Score(int s) : number(s), SampleUIText(std::to_string(number)){}
    Score(std::string text, const sf::Color &C, unsigned int size,
          float position_x, float position_y) : SampleUIText(text, C, size, position_x, position_y) {}
    Score(int s, const sf::Color &C, unsigned int size,
          float position_x, float position_y) : number(s), SampleUIText(std::to_string(number), C, size, position_x, position_y) {}
    void EveryFrame(uint64_t f){
        if(f % 60  == 0){
            number++;
            this->setText(std::to_string(number));
        }
    }
private:
    int number;
};

#endif /* Score_hpp */
