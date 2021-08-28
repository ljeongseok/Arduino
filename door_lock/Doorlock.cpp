#include "Doorlock.h"


Doorlock::Doorlock(int serial_bps, int lcd_addr)
        : MiniCom(serial_bps,lcd_addr){
}

void Doorlock::init(int led_pin, int servo_pin)
{
    MiniCom::init();
    this->led_pin = led_pin;
    pinMode(led_pin,OUTPUT);
    lock.attach(servo_pin);
    print(0,"[[Door Lock]]");
    lock.write(0);
    
}

void Doorlock::readpassword()
{
    password = readRom(BASE_ADDRESS); 
}

void Doorlock::setPassword()
{
    tick();
    print(1,"change password");
    bSetPassword = !bSetPassword;
    if(bSetPassword){   // 새 비밀번호 입력 시작
        onLcd();
        input = "";
        inputStar="";
        bInput=true;
        print(1,inputStar.c_str());

    } else{     // 새 비밀번호 입력 완료
        // ROM에 비밀번호 저장
        password = input;   // 비밀번호 갱신
        writeRom(BASE_ADDRESS,input.c_str());
        bSetPassword = !bSetPassword;
        bInput=false;
        print(1,"");
        
    }
}

int Doorlock::setTimeout(unsigned long time, timer_callback f)
{
    return timer.setTimeout(time,f);
}

void Doorlock::process(char key)
{
    tick();
    // timer = getTimer();
    if (key == '*' && bInput == false){      // 키 입력 시작
        input = "";
        inputStar ="";
        bInput = true;
        // timerId = timer.setTimeout(5000,reset);
        onLcd();
        
    } else if(key == '#'){      // 키 입력 완료
        // deleteTimer(timerId);
        bInput = false;
        check();                // 마지막 처리
        
    } else if(bInput){
        input += key;
        inputStar +="*";
        print(1,inputStar.c_str());
        // restartTimer(timerId);
    }
}

void Doorlock::check()
{
    if (input==password){   // 비밀번호 일치
        lock.write(90);
        print(1,"open");
        delay(5000);
        lock.write(0);
        print(1,"");
        
        check_password = 3;

    } else {        // 비밀번호 불일치
        for(int i=0;i<3;i++){
            delay(100);
            tick();
        }
        check_password--;
        print(1,"chance :",check_password);
        if(check_password == 0){
            print(1,"fail");
            delay(5000);
            print(1,"");
            check_password=3;
        }
    }
}

void Doorlock::tick()
{
    digitalWrite(led_pin,HIGH);
    delay(100);
    digitalWrite(led_pin,LOW);
    
}

void Doorlock::reset()
{
    input = "";
    inputStar="";
    bInput = false;
    offLcd();
    print(1,"");
}