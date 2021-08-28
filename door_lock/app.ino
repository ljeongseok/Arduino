#include "Doorlock.h"
#include <Button.h>
#include <Keypad.h>
#include "keymap.h"

Keypad keypad(makeKeymap(keys),rowPins, colPins, ROWS,COLS);
Button btn(2);
Doorlock door;

void setpassword(){
    door.setPassword();
}


void setup(){
    door.init(13,3);
    door.readpassword();
    btn.setCallback(setpassword);
}

void loop(){
    btn.check();
    door.run();

    char key = keypad.getKey();
    if (key){
        door.process(key);
    }
}
