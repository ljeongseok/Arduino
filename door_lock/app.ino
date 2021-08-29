#include "Doorlock.h"
#include <Button.h>
#include <Keypad.h>
#include "keymap.h"

Keypad keypad(makeKeymap(keys),rowPins, colPins, ROWS,COLS);
Button btn(2);
Doorlock door;

// 초기 비밀번호 : A B C D
void setPassword(){
    door.setPassword();
}

void setup(){
    door.init(13,3);    // 부저 핀 번호, 모터 핀 번호
    door.readpassword();
    btn.setCallback(setPassword);
}

void loop(){
    btn.check();
    door.run();

    char key = keypad.getKey();
    if (key){
        door.process(key);
    }
}
