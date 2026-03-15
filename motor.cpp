#include <iostream>
#include <cstdint>

class StepControl {
public:
    virtual ~StepControl() {}
    virtual void setStep(uint8_t step) = 0;
};

class ServoControl {
public:
    virtual ~ServoControl() {}
    virtual void servoSpecific() = 0;
};

class motor {
public:
    virtual ~motor() {}

    virtual void init() {
        std::cout << "Base motor init\n";
    }

    virtual void run() {
        std::cout << "Base motor run\n";
    }

    virtual void stop() {
        std::cout << "Base motor stop\n";
    }
};

class motorStep : public motor, public StepControl {
public:
    void init() override {
        std::cout << "motorStep init\n";
    }

    void run() override {
        std::cout << "motorStep run\n";
    }

    void stop() override {
        std::cout << "motorStep stop\n";
    }

    void setStep(uint8_t step) override {
        std::cout << "motorStep set step = "
                  << static_cast<int>(step) << "\n";
    }
};

class motorServo : public motor, public ServoControl {
public:
    void init() override {
        std::cout << "motorServo init\n";
    }

    void run() override {
        std::cout << "motorServo run\n";
    }

    void stop() override {
        std::cout << "motorServo stop\n";
    }

    void servoSpecific() override {
        std::cout << "motorServo specific action\n";
    }
};

enum class EDevice : uint8_t {
    MOTORDC = 0,
    MOTORSTEP_1,
    MOTORSERVO_1,
    COUNT
};

struct DeviceMotor {
    motor* p_motor = nullptr;
};

class motorFactory {
public:
    motorFactory() {
        init();
    }

    motor* getMotor(EDevice dev) {
        uint8_t idx = static_cast<uint8_t>(dev);
        if (idx < static_cast<uint8_t>(EDevice::COUNT)) {
            return device[idx].p_motor;
        }
        return nullptr;
    }

    StepControl* getStepControl(EDevice dev) {
        switch (dev) {
            case EDevice::MOTORSTEP_1:
                return &motorStepObj;
            default:
                return nullptr;
        }
    }

    ServoControl* getServoControl(EDevice dev) {
        switch (dev) {
            case EDevice::MOTORSERVO_1:
                return &motorServoObj;
            default:
                return nullptr;
        }
    }

private:
    void init() {
        device[static_cast<uint8_t>(EDevice::MOTORDC)]      .p_motor = &motorDC;
        device[static_cast<uint8_t>(EDevice::MOTORSTEP_1)]  .p_motor = &motorStepObj;
        device[static_cast<uint8_t>(EDevice::MOTORSERVO_1)] .p_motor = &motorServoObj;

        for (uint8_t i = 0; i < static_cast<uint8_t>(EDevice::COUNT); ++i) {
            if (device[i].p_motor) {
                device[i].p_motor->init();
            }
        }
    }

private:
    static motor       motorDC;
    static motorStep   motorStepObj;
    static motorServo  motorServoObj;

    static DeviceMotor device[static_cast<uint8_t>(EDevice::COUNT)];
};

motor       motorFactory::motorDC;
motorStep   motorFactory::motorStepObj;
motorServo  motorFactory::motorServoObj;
DeviceMotor motorFactory::device[static_cast<uint8_t>(EDevice::COUNT)];


int main() {
    motorFactory factory;

    std::cout << "\n--- Polymorphism test ---\n";
    motor* m = factory.getMotor(EDevice::MOTORSTEP_1);
    if (m) {
        m->run();
        m->stop();
    }

    std::cout << "\n--- Step control ---\n";
    if (auto step = factory.getStepControl(EDevice::MOTORSTEP_1)) {
        step->setStep(100);
    }

    std::cout << "\n--- Servo control ---\n";
    if (auto servo = factory.getServoControl(EDevice::MOTORSERVO_1)) {
        servo->servoSpecific();
    }

    return 0;
}


/**
 * - Không make sure: Nguyên tắc thứ ba, "Liskov Substitution Principle", nói rằng nếu một chương trình được thiết kế để sử dụng 
 * một đối tượng của lớp cơ sở, thì nó phải có thể sử dụng đối tượng của lớp dẫn xuất mà không cần biết sự khác 
 * biệt giữa chúng. Mà trong factory class thì trả về đang là base và đối tượng muốn lấy là loại motor riêng biệt
 * dấn đến việc giá trị trong table là DRIVDeviceDriveUnit nhưng giá trị nhận được là DRIVDeviceFactory (DRIVDeviceSteppingMotor) 
 * và nó không đảm bảo là việc trả về là base DRIVDeviceDriveUnit mặc dù vẫn có thể sử dụng
 * - Make sure violate Nguyên tắc "Dependency Inversion Principle" khuyến nghị chúng ta nên phụ thuộc vào các abstraction, không phụ 
 * thuộc vào concretions. Điều này giúp giảm sự phụ thuộc trực tiếp giữa các module, giúp code linh hoạt hơn. Và
 * ở đây nó đang bị phụ thuộc vào dẫn xuất vì phải downcasting sang DRIVDeviceSteppingMotor để sử dụng các method riêng biệt của 
 * step motor
 * - đùng nhiều interface này có ảnh hưởng chút vì phải gọi qua hàm qua cơ chế virtual nhưng chắc ko sao
 */