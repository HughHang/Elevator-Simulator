#include "Button.h"

Button::Button(bool p, int n){
    pressed = p;
    id = n;
}

void Button::illuminate(){
    if(pressed == false){
        pressed = true;
        qInfo("\nButton illuminated\n");
    }
}

void Button::dimOut(){
    if(pressed == true){
        pressed = false;
        qInfo("\nButton dimmed out\n");
    }
}

bool Button::getPressed(){
   return pressed;
}
