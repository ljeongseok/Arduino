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
    offLcd();
}

void Doorlock::readpassword()
{
    password = readRom(BASE_ADDRESS); 
}

void Doorlock::setPassword()
{
    tick();
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
        offLcd();
        
    }
}

// int Doorlock::setTimeout(unsigned long time, timer_callback f)
// {
//     return timer.setTimeout(time,f);
// }

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
        t2 = millis();
        ptime = true;
        print(1,"chance :",check_password);

    } else if(key == '#' && bInput == true){      // 키 입력 완료
        // timer.deleteTimer(timerId);
        bInput = false;
        ptime = false;
        check();                // 마지막 처리
        offLcd();        
        
    } else if(bInput){
        input += key;
        inputStar +="*";
        print(1,inputStar.c_str());
        // timer.restartTimer(timerId);
        t2 = millis();
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
            offLcd();
            print(1,"fail");
            delay(600000);
            print(1,"");
            check_password=3;
            tick();
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
    ptime = false;
    if (check_password<3){
        print(1,"chance :",check_password);
    } else print(1,"");
    
    offLcd();
}

void Doorlock::run()
{
    MiniCom::run();
    if(ptime){
        t1 = millis();
        if(t1 - t2 > 5000){
            reset();
        }
    }
}

