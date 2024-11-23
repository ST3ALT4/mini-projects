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
        cout << "Light is ";
        if (state) {
            cout << "On" << endl;
        } else {
            cout << "Off" << endl;
        }
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
        cout << "Thermostat is ";
        if (state) {
            cout << "On";
        } else {
            cout << "Off";
        }
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
        cout << "Fan is ";
        if (state) {
            cout << "On";
        } else {
            cout << "Off";
        }
        if (state) {
            cout << " and set to speed " << speed << "%.";
        }
        cout << endl;
    }
};

class smartHome {
private:
    int deviceCount;
    smartDevice* devices[10];

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

    void deleteDevice() {
        if (deviceCount > 0) {
            delete devices[--deviceCount];
            cout << "Device removed" << endl;
        } else {
            cout << "No devices to remove" << endl;
        }
    }

    void controlDevice(int index) {
        if (index < 0 || index >= deviceCount) {
            cout << "Invalid device index" << endl;
            return;
        }
        int action;
        do {
            cout << "1.Turn On" << endl
                << "2.Turn Off" << endl
                << "3.Adjust Settings" << endl
                << "4.Check Status" << endl
                << "5.Back to Main Menu" << endl
                << "Enter action for device " << index << ": ";
            cin >> action;

            switch (action) {
                case 1:
                    devices[index]->turnOn();
                    break;
                case 2:
                    devices[index]->turnOff();
                    break;
                case 3:
                    if (dynamic_cast<SmartLight*>(devices[index])) {
                        int brightness;
                        cout << "Enter brightness (0-100): ";
                        cin >> brightness;
                        dynamic_cast<SmartLight*>(devices[index])->adjustSettings(brightness);
                    } else if (dynamic_cast<SmartThermostat*>(devices[index])) {
                        int temp;
                        cout << "Enter temperature: ";
                        cin >> temp;
                        dynamic_cast<SmartThermostat*>(devices[index])->adjustSettings(temp);
                    } else if (dynamic_cast<SmartFan*>(devices[index])) {
                        int speed;
                        cout << "Enter speed (0-100): ";
                        cin >> speed;
                        dynamic_cast<SmartFan*>(devices[index])->adjustSettings(speed);
                    }
                    break;
                case 4:
                    devices[index]->status();
                    break;
                case 5:
                    cout << "Returning to main menu..." << endl;
                    break;
                default:
                    cout << "Invalid action, please try again." << endl;
            }
        } while (action != 5);
    }

    void monitorDevices() {
        for (int i = 0; i < deviceCount; i++) {
            devices[i]->status();
        }
    }

    void loop() {
        int choice;
        do {
            cout << "1. Add Device (Light: 1, Thermostat: 2, Fan: 3)" << endl
                << "2. Delete Last Device" << endl
                << "3. Control Device" << endl
                << "4. Monitor Devices" << endl
                << "5. Exit" << endl
                << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    int deviceType;
                    cout << "Enter device type (1: Light, 2: Thermostat, 3: Fan): ";
                    cin >> deviceType;
                    if (deviceType == 1) {
                        addDevice(new SmartLight());
                    } else if (deviceType == 2) {
                        addDevice(new SmartThermostat());
                    } else if (deviceType == 3) {
                        addDevice(new SmartFan());
                    } else {
                        cout << "Invalid device type" << endl;
                    }
                    break;
                }
                case 2:
                    deleteDevice();
                    break;
                case 3: {
                    int deviceIndex;
                    cout << "Enter device index (0 to " << deviceCount - 1 << "): ";
                    cin >> deviceIndex;
                    controlDevice(deviceIndex);
                    break;
                }
                case 4:
                    monitorDevices();
                    break;
                case 5:
                    cout << "Exiting..." << endl;
                    break;
                default:
                    cout << "Invalid choice, please try again." << endl;
            }
        } while (choice != 5);
    }
};

int main() {
    smartHome home;
    home.loop();
    return 0;
}
