#define SDL_MAIN_HANDLED// This was very annoying.
#include <iostream>
#include <SDL.h>
#include <SDL_gamecontroller.h>
#include <Windows.h>
#include <WinUser.h>

struct Vector3 {
    float x, y, z;
    Vector3() {
        x = y = z = 0;
    }
    Vector3(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {

    }

    const float Magnitude() const {
        return sqrt(x * x + y * y + z * z);
    }
    const Vector3 Normalized() const {
        float m = this->Magnitude();
        return Vector3(x / m, y / m, z / m);
    }
    const void operator+=(const Vector3& v) {
        x += v.x;
        y += v.y;
        z += v.z;
    }
    const Vector3 operator/(const float& v) const {
        return Vector3(x / v, y / v, z / v);
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector3& v);
};
std::ostream& operator<<(std::ostream& os, const Vector3& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}
float Dot(const Vector3& v1, const Vector3& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
template <typename T>
int Sign(const T& t) {
    if (t >= 0) return 1;
    return -1;
}

// Hardcoded configuration, not really based on anything but tests.
int delay = 10;
float deadzone = 0.5;
float multiplier = 1;
float power = 1.8;

void ProcessAccelerometer_MouseStick(const Vector3 accel) {
    // std::cout << accel << std::endl;
    float x = (abs(accel.z));
    float y = (abs(accel.x));
    if (abs(x) < deadzone)x = 0;
    if (abs(y) < deadzone)y = 0;
    x = pow(x, power);
    y = pow(y, power);
    x *= Sign(accel.z) * -1*multiplier;
    y *= Sign(accel.x)*multiplier;

    LPPOINT current = (LPPOINT)malloc(sizeof(LPPOINT));// new doesn't work here? lppoint is typedef?
    if (GetCursorPos(current)) {
        int dx = round(x);
        int dy = round(y);
        SetCursorPos(current->x + dx, current->y + dy);
    }
    else {

    }

    // std::cout << x << " || " << y << std::endl;
    free(current);// Ugly but it works.
}
void ProcessAccelerometer_WASD(const Vector3 accel) {
    std::cout << accel << std::endl;
    float x = (abs(accel.z));
    float y = (abs(accel.x));
    if (abs(x) < deadzone)x = 0;
    if (abs(y) < deadzone)y = 0;
    x = pow(x, power);
    y = pow(y, power);
    x *= Sign(accel.z) * -1 * multiplier;
    y *= Sign(accel.x) * multiplier;

    // it would be better to use a state machine here
    float deadzone = 1;

    if (abs(y) > deadzone) {
        // y-axis simulation
        INPUT inputs[2] = {};
        ZeroMemory(inputs, sizeof(inputs));

        inputs[0].type = INPUT_KEYBOARD;
        inputs[0].ki.wVk = 0x57;// w
        if (y < 0)
            inputs[0].ki.dwFlags = 0;
        else
            inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;

        inputs[1].type = INPUT_KEYBOARD;
        inputs[1].ki.wVk = 0x53;// s
        if (y > 0)
            inputs[1].ki.dwFlags = 0;
        else
            inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

        UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    }
    else {
        // zero
        INPUT inputs[2] = {};
        ZeroMemory(inputs, sizeof(inputs));

        inputs[0].type = INPUT_KEYBOARD;
        inputs[0].ki.wVk = 0x57;// w
            inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;

        inputs[1].type = INPUT_KEYBOARD;
        inputs[1].ki.wVk = 0x53;// s
            inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

        UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    }

    if (abs(x) > deadzone) {
        // y-axis simulation
        INPUT inputs[2] = {};
        ZeroMemory(inputs, sizeof(inputs));

        inputs[0].type = INPUT_KEYBOARD;
        inputs[0].ki.wVk = 0x41;// d
        if (x < 0)
            inputs[0].ki.dwFlags = 0;
        else
            inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;

        inputs[1].type = INPUT_KEYBOARD;
        inputs[1].ki.wVk = 0x44;// a
        if (x > 0)
            inputs[1].ki.dwFlags = 0;
        else
            inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

        UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    }
    else {
        // zero
        INPUT inputs[2] = {};
        ZeroMemory(inputs, sizeof(inputs));

        inputs[0].type = INPUT_KEYBOARD;
        inputs[0].ki.wVk = 0x44;// d
        inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;

        inputs[1].type = INPUT_KEYBOARD;
        inputs[1].ki.wVk = 0x41;// a
        inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

        UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    }

}

int main(int argc, char* argv[])
{
    // A lot boilerplate code SDL example.
    SetCursorPos(0, 0);

    SDL_SetHint(SDL_HINT_ACCELEROMETER_AS_JOYSTICK, "0");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_JOY_CONS, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS4_RUMBLE, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS5_RUMBLE, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
    SDL_SetHint(SDL_HINT_LINUX_JOYSTICK_DEADZONES, "1");

    if (SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Fetch the first occurrence of a game controller.
    SDL_GameController* controller = nullptr;
    const char* name = nullptr;
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        if (SDL_IsGameController(i)) {
            name = SDL_GameControllerNameForIndex(i);
            SDL_Log("Considering Controller: %s", name);

            SDL_JoystickID controller_id = SDL_JoystickGetDeviceInstanceID(i);
            controller = SDL_GameControllerOpen(i);

            if (!SDL_GameControllerHasSensor(controller, SDL_SENSOR_ACCEL) || !SDL_GameControllerHasSensor(controller, SDL_SENSOR_GYRO)) {
                SDL_Log("Skipping %s because it doesn't have both accelerometer and gyrometer.\n", name);
                continue;
            }

            if (SDL_GameControllerHasSensor(controller, SDL_SENSOR_ACCEL)) {
                SDL_Log("Enabling accelerometer at %.2f Hz\n", SDL_GameControllerGetSensorDataRate(controller, SDL_SENSOR_ACCEL));
                SDL_GameControllerSetSensorEnabled(controller, SDL_SENSOR_ACCEL, SDL_TRUE);
            }
            if (SDL_GameControllerHasSensor(controller, SDL_SENSOR_GYRO)) {
                SDL_Log("Enabling gyro at %.2f Hz\n", SDL_GameControllerGetSensorDataRate(controller, SDL_SENSOR_GYRO));
                SDL_GameControllerSetSensorEnabled(controller, SDL_SENSOR_GYRO, SDL_TRUE);
            }
        }
    }

    if (controller == nullptr) {
        SDL_Log("Failed to find a adequate controller.\n");
        return 1;
    }

    SDL_Log("\nUsing %s.\n", name);

    while (true) {
        SDL_Event e;

        SDL_PumpEvents();

        Vector3 acceleration;
        int samples = 0;
        while (SDL_PeepEvents(&e, 1, SDL_GETEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT) == 1) {
            switch (e.type) {
            case SDL_CONTROLLERSENSORUPDATE:
                // Only fetch the accelerometer for now.
                if (e.csensor.sensor != SDL_SENSOR_ACCEL) break;
                /*SDL_Log("Controller %d sensor %s: %.2f, %.2f, %.2f\n",
                    e.csensor.which,
                    e.csensor.sensor == SDL_SENSOR_ACCEL ? "accelerometer" :
                    e.csensor.sensor == SDL_SENSOR_GYRO ? "gyro" : "unknown",
                    e.csensor.data[0],
                    e.csensor.data[1],
                    e.csensor.data[2]);*/

                // Take average for stability
                acceleration += Vector3(e.csensor.data[0], e.csensor.data[1], e.csensor.data[2]);
                ++samples;
            }
        }
    
        if (samples>0) {
            
            // ProcessAccelerometer_MouseStick(acceleration/(float)samples);
            ProcessAccelerometer_WASD(acceleration / (float)samples);
        }
        SDL_Delay(delay);
    }

    return 0;
}

// Used resources:
// sdl examples https://github.com/libsdl-org/SDL/tree/419ae29d90d3934e1a382cd693ac60a1f492e43a/test
// Intro to sdl https://lazyfoo.net/tutorials/SDL/19_gamepads_and_joysticks/index.php