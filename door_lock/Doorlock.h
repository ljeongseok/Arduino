#pragma once

#include <MiniCom.h>
#include <storage.h>
#include <Servo.h>
#include <Button.h>

class Button;

class Doorlock: public MiniCom{
protected:
    String input ="";       // 입력값 저장
    String inputStar  = ""; // 입력할 때 출력할 * 문자열
    String password = "";   // 도어락 비밀번호
    
    const int BASE_ADDRESS = 100; // eeprom 비밀번호 주소
    
    bool bInput = false;    // 키 입력 상태 변수, * 누르면 true / 입력시작, # 누르면 false 입력중지
    int timerId = -1;       // 타임아웃용 타이머 id

    bool bSetPassword = false;  // 비밀번호 설정 모드 여부
    int check_password = 3;     // 비밀번호 틀린 회수 저장, 기회 3번
    int led_pin;    // led 핀번호
    int servo_pin;  // servo 핀번호
    Servo lock;
    int button_pin;
    

public:
    Doorlock(int serial_bps=115200, int lcd_addr=0x27);
    // SimpleTimer& getTimer(){return timer;}
    void init(int led_pin, int servo_pin);
    void readpassword();     // 비밀번호 가져오기     
    void setPassword();      // 새 비밀번호 만들기
    void process(char key);  // 비밀번호 입력
    void check();            // 비밀번호 체크
    void tick();             // 비프음
    void reset();            // 시간초과 리셋
    
    int setTimeout(unsigned long time, timer_callback f);
    

};