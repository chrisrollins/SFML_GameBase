//
//  Menu.h
//  group PROJECT
//
//  Created by Jakin Wang on 2/26/19.
//  Copyright © 2019 Jakin Wang. All rights reserved.
//

#include <SFML/Graphics.hpp>
#include <Screen.h>
#include <vector>
#ifndef Menu_h
#define Menu_h

using namespace sf;

class Menu{
    std::vector<Text> TextArray;
    Font f;
    Text t;
public:
    void setText(Text t);
    void setFont(Font f);
    void setFontOther(Color c = Color::Blue, unsigned int size = 12, float position_x = 0, float position_y = 0);
    void setFontFromFile(std::string filename);
    void draw(Engine::Screen s);
};

#endif /* Menu_h */
