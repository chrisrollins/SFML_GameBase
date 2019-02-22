//
//  MainCharacter.hpp
//  smflMaster
//
//  Created by radu mischiu on 2/21/19.
//  Copyright © 2019 Maria Gorbunova. All rights reserved.
//



#ifndef MainCharacter_hpp
#define MainCharacter_hpp

#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include "Screen.h"
#include "GameObject.h"

#include <stdio.h>

using namespace Engine;



class MainCharacter : public GraphicalGameObject
{
    bool W_KeyHeld = false;
    bool A_KeyHeld = false;
    bool S_KeyHeld = false;
    bool D_KeyHeld = false;
public:
    // Character(sf::RectangleShape r) : GraphicalGameObject(r) { }
    MainCharacter (sf::Sprite s):GraphicalGameObject(s, 450, 250 ){
    }
    
    void KeyPressed(sf::Event e)
    {
        switch (e.key.code)
        {
            case sf::Keyboard::W:
                this->W_KeyHeld = true;
                break;
            case sf::Keyboard::A:
                this->A_KeyHeld = true;
                break;
            case sf::Keyboard::S:
                this->S_KeyHeld = true;
                break;
            case sf::Keyboard::D:
                this->D_KeyHeld = true;
                break;
            default:
                break;
                
        }
    }
    void KeyReleased(sf::Event e)
    {
        switch (e.key.code)
        {
            case sf::Keyboard::W:
                this->W_KeyHeld = false;
                break;
            case sf::Keyboard::A:
                this->A_KeyHeld = false;
                break;
            case sf::Keyboard::S:
                this->S_KeyHeld = false;
                break;
            case sf::Keyboard::D:
                this->D_KeyHeld = false;
                break;
            default:
                break;
                
        }
    }
    void EveryFrame(uint64_t f)
    {
        sf::Sprite* r = this->placeholder();
        if (this->W_KeyHeld) { r->move(0, -10); }
        if (this->A_KeyHeld) { r->move(-10, 0); }
        if (this->S_KeyHeld) { r->move(0, 10); }
        if (this->D_KeyHeld) { r->move(10, 0); }
    }
    sf::Sprite* placeholder()
    {
        return dynamic_cast<sf::Sprite*>(this->graphic);
    }
};


#endif /* MainCharacter_hpp */
