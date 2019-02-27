//
//  Menu.cpp
//  group PROJECT
//
//  Created by Jakin Wang on 2/26/19.
//  Copyright © 2019 Jakin Wang. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "Menu.h"

void Menu::draw(Engine::Screen s){
//    for (Text t : this->TextArray)
//        s.add(t);
}

void Menu::setFont(Font f){
    t.setFont(f);
}

void Menu::setFontOther(Color c , unsigned int size , float position_x, float position_y){
    t.setFillColor(c);
    t.setCharacterSize(size);
    t.setPosition(position_x, position_y);
}

void Menu::setFontFromFile(std::string filename){
    if(!f.loadFromFile(filename)){
        std::cerr << "Error Loading Font!!" << std::endl;
        exit(1);
    }
}

void Menu::setText(Text t){
    this->t = t;
}



