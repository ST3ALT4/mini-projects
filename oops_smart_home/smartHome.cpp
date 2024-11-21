#include <iostream>

using namespace std;

class smartDevice {
public:
    bool state;

    smartDevice() {
        state = false;
    }

    virtual void turnOn() {
        state = true;
    }

    virtual void turnOff() {
        state = false;
    }

    virtual void status() {
        cout << "The device is ";
        if (state) {
            cout << "ON" << endl;
        } else {
            cout << "OFF" << endl;
        }
    }

    virtual ~smartDevice() {}
};

class SmartLight : public smartDevice {
public:
    void adjustSettings(int brightness) {
        if (state) {
            cout << "Adjusting light brightness to " << brightness << "%" << endl;
        } else {
            cout << "Light is off, cannot adjust brightness." << endl;
        }
    }

    void status() override {
        cout << "Light is " << (state ? "On" : "Off") << endl;
    }
};

class SmartThermostat : public smartDevice {
private:
    int temperature;
public:
    SmartThermostat() : temperature(20) {}

    void adjustSettings(int temp) {
        if (state) {
            temperature = temp;
            cout << "Setting thermostat temperature to " << temperature << " degrees." << endl;
        } else {
            cout << "Thermostat is off, cannot adjust temperature." << endl;
        }
    }

    void status() override {
        cout << "Thermostat is " << (state ? "On" : "Off");
        if (state) {
            cout << " and set to " << temperature << " degrees.";
        }
        cout << endl;
    }
};

class SmartFan : public smartDevice {
private:
    int speed;
public:
    SmartFan() : speed(0) {}

    void adjustSettings(int newSpeed) {
        if (state) {
            speed = newSpeed;
            cout << "Setting fan speed to " << speed << "%" << endl;
        } else {
            cout << "Fan is off, cannot adjust speed." << endl;
        }
    }

    void status() override {
        cout << "Fan is " << (state ? "On" : "Off");
        if (state) {
            cout << " and set to speed " << speed << "%.";
        }
        cout << endl;
    }
};


class smartHome {
private:
    int deviceCount;
    smartDevice* devices[10];  // Array to store pointers to devices

public:
    smartHome() : deviceCount(0) {}

    ~smartHome() {
        for (int i = 0; i < deviceCount; i++) {
            delete devices[i];
        }
    }

    void addDevice(smartDevice* device) {
        if (deviceCount < 10) {
            devices[deviceCount++] = device;
        } else {
            cout << "Cannot add more devices" << endl;
        }
    }

    void controlDevices() {
        for (int i = 0; i < deviceCount; i++) {
            devices[i]->turnOn();
            devices[i]->status();
            devices[i]->turnOff();
        }
    }

    void monitorDevices() {
        for (int i = 0; i < deviceCount; i++) {
            devices[i]->status();
        }
    }

};

int main() {
    smartHome home;

    home.addDevice(new SmartLight());
    home.addDevice(new SmartThermostat());

    home.controlDevices();

    return 0;
}
