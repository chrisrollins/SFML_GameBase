#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "ResourcePath.hpp"
#include "GameObject.h"
#include "Screen.h"
#include "Character.h"
#include "TileMap.h"
using namespace std;
using namespace sf;
using namespace Engine;

const int X = 6;
const int Y = 4;
void DISPLAY_MENU();
void DISPLAY_END(int Score_int);

Screen startingScreen;
class MCharacter : public GraphicalGameObject
{
    enum Direction{Down, Right, Up, Left};
    Vector2i source;
    bool W_KeyHeld = false;
    bool A_KeyHeld = false;
    bool S_KeyHeld = false;
    bool D_KeyHeld = false;
    std::vector<GraphicalGameObject*> objs;
public:
    MCharacter(Sprite s) : GraphicalGameObject(s) {
        source.x = 0;
        source.y = Down;
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
    void MouseButtonReleased(sf::Event e)
    {
        sf::Vector2f position = this->placeholder()->getPosition();
        std::cout << "Mouse clicked at (" << e.mouseButton.x << ", " << e.mouseButton.y << ")" << std::endl;
        std::cout << "character is at (" << position.x << ", " << position.y << ")" << std::endl;
        int adjustedX = position.x + e.mouseButton.x - (startingScreen.windowWidth / 2);
        int adjustedY = position.y + e.mouseButton.y - (startingScreen.windowHeight / 2);
//        SampleSquare* s = new SampleSquare(); //use a heap allocated object which has to be cleaned up later.
//        objs.push_back(s);
//        s->square()->setPosition(adjustedX, adjustedY);
//        startingScreen.add(s);
    }
    void EveryFrame(uint64_t f)
    {
        Sprite* r = this->placeholder();
        if (this->W_KeyHeld) { source.y = Up; r->move(0, -5); }
        if (this->A_KeyHeld) { source.y = Left; r->move(-5, 0); }
        if (this->S_KeyHeld) { source.y = Down; r->move(0, 5); }
        if (this->D_KeyHeld) { source.y = Right; r->move(5, 0); }
        if(f % 30 == 0){
            r->setTextureRect(IntRect(source.x * 100, source.y * 100, 100, 100));
            source.x++;
            if(source.x >= X) source.x = 0;
        }
    }
    Sprite* placeholder()
    {
        return dynamic_cast<Sprite*>(this->graphic);
    }
};

class Projectile : public GraphicalGameObject {
private:
    float x_speed;
    float y_speed;
    int x = 0;
public:
    Projectile(Sprite s) : GraphicalGameObject(s) {}
    Sprite* toSprite(){
        return dynamic_cast<Sprite*>(this->graphic);
    }
    void setX_Speed(float x_speed){this->x_speed = x_speed;}
    void setY_Speed(float y_speed){this->y_speed = y_speed;}
    bool intersection(Sprite s){
        return (this->toSprite()->getGlobalBounds().intersects(s.getGlobalBounds()));
    }
    void EveryFrame(uint64_t f) {
        
        Sprite* r = this->toSprite();
        r->move(5, 0);
        if(f % 30 == 0){
            r->setTextureRect(IntRect(x * 63, 12 * 60, 63, 60));
            x++;
            if(x >= 8) x = 0;
        }
    }
    
};

int main(int, char const**)
{
//    DISPLAY_MENU();
    int Score_int = 0;

    Font f;
    if(!f.loadFromFile("sansation.ttf")){
        cerr << "Cannot Load Font!!!" << endl;
        exit(1);
    }
    Text Score;
    Score.setFont(f);
    Score.setString(to_string(Score_int));
    Score.setFillColor(Color::Yellow);
    Score.setCharacterSize(50);
    Score.setPosition(920, 0);
    
    // declare a tilemap object
    TileMap map;
    // build map using different tiles
    const int level[] =
    {
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
        0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
        0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
        0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
        2, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
        0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
        0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
        0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
        0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
        2, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
        0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
    };
    // 32,32 means size of each tile
    // 16 means how many tiles per line
    // 8 means how many tiles per column
    // 16 & 8 are related to the level
    assert(map.load("/Users/jakin/Desktop/map.png", sf::Vector2u(32, 32), level, 30, 8));
    
    
    
    Clock clock;
    Time time;
    
//    RenderWindow window(VideoMode(WINDOW_LENGTH, WINDOW_WIDTH), "Game");
//    window.setKeyRepeatEnabled(false);
    

    Texture texture;
    if(!texture.loadFromFile("/Users/jakin/Desktop/trump_run.png")){
        cerr << "ERROR!" << endl;
        exit(1);
    }
    
    Texture O;
    if(!O.loadFromFile("/Users/jakin/Desktop/Orc.png")){
        cerr << "ERROR!" << endl;
        exit(1);
    }
    
    
    Sprite Character;
    Character.setTexture(texture);
    Character.setPosition(1000, 1000);
    MCharacter Trump(Character);
    startingScreen.addMainCharacter(&Trump);
    
    Sprite Orc, Orc2;
    Orc.setTexture(O);
    Orc2.setTexture(O);
    Orc.setPosition(0, 200);
    Orc2.setPosition(0, 500);
    Projectile O1(Orc);
    Projectile O2(Orc2);
    startingScreen.add(&O1);
    startingScreen.add(&O2);
    startingScreen.addMap(&map);
    startingScreen.windowWidth = startingScreen.windowHeight = 2000;
    startingScreen.windowTitle = "Game";
//    startingScreen.addMap(&map);
    startingScreen.render();
//
//
//    while(window.isOpen()){
//        Event event;
//        while(window.pollEvent(event)){
//            switch (event.type) {
//                case Event::Closed:
//                    window.close();
//                    break;
//                case Event::KeyPressed:
//                    if(event.key.code == Keyboard::W) {
//                        source.y = Up;
//                        if(Keyboard::isKeyPressed(Keyboard::D))
//                            Character.move(15, -15);
//                        Character.move(0, -20);
//                    } else if(event.key.code == Keyboard::S){
//                        source.y = Down;
//                        Character.move(0, 20);
//                    }else if(event.key.code == Keyboard::A){
//                        source.y = Left;
//                        Character.move(-20, 0);
//                    }else if(event.key.code == Keyboard::D){
//                        source.y = Right;
//                        Character.move(20, 0);
//                    }
//                default:
//                    break;
//            }
//        }
//        Comet.move(0.5, 0);
//        Comet2.move(1, 0);
//        if(source.y >= Y)
//            source.y = 0;
//        if(Comet.getPosition().x < 0)
//            Comet.setPosition(999, Comet.getPosition().y);
//        if(Comet.getPosition().x > WINDOW_WIDTH)
//            Comet.setPosition(1, Comet.getPosition().y);
//        if(Comet.getPosition().y < 0)
//            Comet.setPosition(Comet.getPosition().x, 999);
//        if(Comet.getPosition().y > WINDOW_LENGTH)
//            Comet.setPosition(Comet.getPosition().x, 1);
//
//        if(Comet2.getPosition().x < 0)
//            Comet2.setPosition(999, Comet2.getPosition().y);
//        if(Comet2.getPosition().x > WINDOW_WIDTH)
//            Comet2.setPosition(1, Comet2.getPosition().y);
//        if(Comet2.getPosition().y < 0)
//            Comet2.setPosition(Comet2.getPosition().x, 999);
//        if(Comet2.getPosition().y > WINDOW_LENGTH)
//            Comet2.setPosition(Comet2.getPosition().x, 1);
//
//        if(Character.getPosition().x < 0)
//            Character.setPosition(999, Character.getPosition().y);
//        if(Character.getPosition().x > WINDOW_WIDTH)
//            Character.setPosition(1, Character.getPosition().y);
//        if(Character.getPosition().y < 0)
//            Character.setPosition(Character.getPosition().x, 999);
//        if(Character.getPosition().y > WINDOW_LENGTH)
//            Character.setPosition(Character.getPosition().x, 1);
//
//        if(source.x * 100 >= texture.getSize().x - 100)
//            source.x = 0;
//        time = clock.getElapsedTime();
//        if(time.asMilliseconds() % 100 == 0){
//            source.x++;
//            Score_int++;
//            Score.setString(to_string(Score_int));
//        }
//        Character.setTextureRect(IntRect(source.x * 100, source.y * 100, 100, 100));
//        if(source.x >= X)
//            source.x = 0;
//        Comet.setTextureRect(IntRect(20, 100, 50, 50));
//        Comet2.setTextureRect(IntRect(20, 100, 50, 50));
//        window.clear();
//        window.draw(Character);
//        window.draw(Comet);
//        window.draw(Comet2);
//        window.draw(Score);
//        if(Character.getGlobalBounds().intersects(Comet.getGlobalBounds()))
//            DISPLAY_END(Score_int);
//        window.display();
//    }
//
//    return EXIT_SUCCESS;
//}
//
//void DISPLAY_MENU(){
//    Text t, ins;
//    t.setString("GAME START");
//    ins.setString("Press Any Key to continue!!");
//    Font font;
//    if(!font.loadFromFile("sansation.ttf")){
//        cerr << "Error loading font" << endl;
//        exit(1);
//    }
//    t.setFont(font);
//    ins.setFont(font);
//    t.setFillColor(Color::Cyan);
//    ins.setFillColor(Color::Cyan);
//    t.setCharacterSize(64);
//    ins.setCharacterSize(32);
//    t.setPosition(WINDOW_LENGTH/2 - 7 * 32, WINDOW_LENGTH/2);
//    ins.setPosition(WINDOW_LENGTH/2 - 7 * 32, WINDOW_LENGTH/2 + 200);
//    RenderWindow window(VideoMode(WINDOW_LENGTH, WINDOW_WIDTH), "Game Menu");
//    while(window.isOpen()){
//        Event event;
//        while(window.pollEvent(event)){
//            switch (event.type) {
//                case Event::KeyPressed:
//                    window.close();
//                    return;
//            }
//        }
//        window.clear();
//        window.draw(t);
//        window.draw(ins);
//        window.display();
//    }
//
//}
//
//void DISPLAY_END(int Score_int){
//    Text t, ins, S;
//    t.setString("GAME OVER");
//    ins.setString("Press Any Key to End!!");
//    S.setString("YOUR SCORE: " + to_string(Score_int));
//    Font font;
//    if(!font.loadFromFile("sansation.ttf")){
//        cerr << "Error loading font" << endl;
//        exit(1);
//    }
//    t.setFont(font);
//    ins.setFont(font);
//    S.setFont(font);
//    t.setFillColor(Color::Red);
//    ins.setFillColor(Color::Red);
//    t.setCharacterSize(64);
//    ins.setCharacterSize(32);
//    t.setPosition(WINDOW_LENGTH/2 - 7 * 32, WINDOW_LENGTH/2);
//    ins.setPosition(WINDOW_LENGTH/2 - 7 * 32, WINDOW_LENGTH/2 + 200);
//
//    S.setFillColor(Color::Yellow);
//    S.setPosition(300, 300);
//    RenderWindow window(VideoMode(WINDOW_LENGTH, WINDOW_WIDTH), "Game Menu");
//    while(window.isOpen()){
//        Event event;
//        while(window.pollEvent(event)){
//            switch (event.type) {
//                case Event::KeyPressed:
//                    window.close();
//                    exit(1);
//            }
//        }
//        window.clear();
//        window.draw(t);
//        window.draw(ins);
//        window.draw(S);
//        window.display();
//    }
}
