#include "bbsys.h"
#include "std.h"

gxInput *gx_input;
gxDevice *gx_mouse;
gxDevice *gx_keyboard;
std::vector<gxDevice *> gx_joysticks;

static int mouse_x, mouse_y, mouse_z;
static const float JLT = -1.0f / 3.0f;
static const float JHT = 1.0f / 3.0f;

bool input_create() {
    if (gx_input = gx_runtime->openInput(0)) {
        if (gx_keyboard = gx_input->getKeyboard()) {
            if (gx_mouse = gx_input->getMouse()) {
                gx_joysticks.clear();
                for (int k = 0; k < gx_input->numJoysticks(); ++k) {
                    gx_joysticks.push_back(gx_input->getJoystick(k));
                }
                mouse_x = mouse_y = mouse_z = 0;
                return true;
            }
        }
        gx_runtime->closeInput(gx_input);
        gx_input = nullptr;
    }
    return false;
}

bool input_destroy() {
    gx_joysticks.clear();
    gx_runtime->closeInput(gx_input);
    gx_input = nullptr;
    return true;
}

int bbKeyDown(const int n) {
    return gx_keyboard->keyDown(n);
}

int bbKeyHit(const int n) {
    return gx_keyboard->keyHit(n);
}

int bbGetKey() {
    return gx_input->toAscii(gx_keyboard->getKey());
}

int bbWaitKey() {
    for (;;) {
        if (!gx_runtime->idle())
            RTEX(0);
        if (int key = gx_keyboard->getKey()) {
            if (key = gx_input->toAscii(key)) return key;
        }
        gx_runtime->delay(20);
    }
}

void bbFlushKeys() {
    gx_keyboard->flush();
}

int bbMouseDown(const int n) {
    return gx_mouse->keyDown(n);
}

int bbMouseHit(const int n) {
    return gx_mouse->keyHit(n);
}

int bbGetMouse() {
    return gx_mouse->getKey();
}

int bbWaitMouse() {
    for (;;) {
        if (!gx_runtime->idle())
            RTEX(0);
        if (const int key = gx_mouse->getKey()) return key;
        gx_runtime->delay(20);
    }
}

int bbMouseWait() {
    return bbWaitMouse();
}

int bbMouseX() {
    return gx_mouse->getAxisState(0);
}

int bbMouseY() {
    return gx_mouse->getAxisState(1);
}

int bbMouseZ() {
    return gx_mouse->getAxisState(2) / 120;
}

int bbMouseXSpeed() {
    const int dx = bbMouseX() - mouse_x;
    mouse_x += dx;
    return dx;
}

int bbMouseYSpeed() {
    const int dy = bbMouseY() - mouse_y;
    mouse_y += dy;
    return dy;
}

int bbMouseZSpeed() {
    const int dz = bbMouseZ() - mouse_z;
    mouse_z += dz;
    return dz;
}

void bbFlushMouse() {
    gx_mouse->flush();
}

void bbMoveMouse(const int x, const int y) {
    gx_input->moveMouse(mouse_x = x, mouse_y = y);
}

int bbJoyType(const int port) {
    return gx_input->getJoystickType(port);
}

int bbJoyDown(const int n, const int port) {
    if (port < 0 || port >= gx_joysticks.size()) return 0;
    return gx_joysticks[port]->keyDown(n);
}

int bbJoyHit(const int n, const int port) {
    if (port < 0 || port >= gx_joysticks.size()) return 0;
    return gx_joysticks[port]->keyHit(n);
}

int bbGetJoy(const int port) {
    if (port < 0 || port >= gx_joysticks.size()) return 0;
    return gx_joysticks[port]->getKey();
}

int bbWaitJoy(const int port) {
    if (port < 0 || port >= gx_joysticks.size()) return 0;
    for (;;) {
        if (!gx_runtime->idle())
            RTEX(0);
        if (const int key = gx_joysticks[port]->getKey()) return key;
        gx_runtime->delay(20);
    }
}

float bbJoyX(const int port) {
    if (port < 0 || port >= gx_joysticks.size()) return 0;
    return gx_joysticks[port]->getAxisState(0);
}

float bbJoyY(const int port) {
    if (port < 0 || port >= gx_joysticks.size()) return 0;
    return gx_joysticks[port]->getAxisState(1);
}

float bbJoyZ(const int port) {
    if (port < 0 || port >= gx_joysticks.size()) return 0;
    return gx_joysticks[port]->getAxisState(2);
}

float bbJoyU(const int port) {
    if (port < 0 || port >= gx_joysticks.size()) return 0;
    return gx_joysticks[port]->getAxisState(3);
}

float bbJoyV(const int port) {
    if (port < 0 || port >= gx_joysticks.size()) return 0;
    return gx_joysticks[port]->getAxisState(4);
}

float bbJoyPitch(const int port) {
    if (port < 0 || port >= gx_joysticks.size()) return 0;
    return gx_joysticks[port]->getAxisState(5) * 180;
}

float bbJoyYaw(const int port) {
    if (port < 0 || port >= gx_joysticks.size()) return 0;
    return gx_joysticks[port]->getAxisState(6) * 180;
}

float bbJoyRoll(const int port) {
    if (port < 0 || port >= gx_joysticks.size()) return 0;
    return gx_joysticks[port]->getAxisState(7) * 180;
}

int bbJoyHat(const int port) {
    if (port < 0 || port >= gx_joysticks.size()) return 0;
    return gx_joysticks[port]->getAxisState(8);
}

int bbJoyXDir(const int port) {
    if (port < 0 || port >= gx_joysticks.size()) return 0;
    const float t = gx_joysticks[port]->getAxisState(0);
    return t < JLT ? -1 : (t > JHT ? 1 : 0);
}

int bbJoyYDir(const int port) {
    if (port < 0 || port >= gx_joysticks.size()) return 0;
    const float t = gx_joysticks[port]->getAxisState(1);
    return t < JLT ? -1 : (t > JHT ? 1 : 0);
}

int bbJoyZDir(const int port) {
    if (port < 0 || port >= gx_joysticks.size()) return 0;
    const float t = gx_joysticks[port]->getAxisState(2);
    return t < JLT ? -1 : (t > JHT ? 1 : 0);
}

int bbJoyUDir(const int port) {
    if (port < 0 || port >= gx_joysticks.size()) return 0;
    const float t = gx_joysticks[port]->getAxisState(3);
    return t < JLT ? -1 : (t > JHT ? 1 : 0);
}

int bbJoyVDir(const int port) {
    if (port < 0 || port >= gx_joysticks.size()) return 0;
    const float t = gx_joysticks[port]->getAxisState(4);
    return t < JLT ? -1 : (t > JHT ? 1 : 0);
}

void bbFlushJoy() {
    for (int k = 0; k < gx_joysticks.size(); ++k) gx_joysticks[k]->flush();
}

void bbEnableDirectInput(const int enable) {
    gx_runtime->enableDirectInput(!!enable);
}

int bbDirectInputEnabled() {
    return gx_runtime->directInputEnabled();
}

void input_link(void (*rtSym)(const char *sym, void *pc)) {
    rtSym("%KeyDown%key", bbKeyDown);
    rtSym("%KeyHit%key", bbKeyHit);
    rtSym("%GetKey", bbGetKey);
    rtSym("%WaitKey", bbWaitKey);
    rtSym("FlushKeys", bbFlushKeys);

    rtSym("%MouseDown%button", bbMouseDown);
    rtSym("%MouseHit%button", bbMouseHit);
    rtSym("%GetMouse", bbGetMouse);
    rtSym("%WaitMouse", bbWaitMouse);
    rtSym("%MouseWait", bbWaitMouse);
    rtSym("%MouseX", bbMouseX);
    rtSym("%MouseY", bbMouseY);
    rtSym("%MouseZ", bbMouseZ);
    rtSym("%MouseXSpeed", bbMouseXSpeed);
    rtSym("%MouseYSpeed", bbMouseYSpeed);
    rtSym("%MouseZSpeed", bbMouseZSpeed);
    rtSym("FlushMouse", bbFlushMouse);
    rtSym("MoveMouse%x%y", bbMoveMouse);

    rtSym("%JoyType%port=0", bbJoyType);
    rtSym("%JoyDown%button%port=0", bbJoyDown);
    rtSym("%JoyHit%button%port=0", bbJoyHit);
    rtSym("%GetJoy%port=0", bbGetJoy);
    rtSym("%WaitJoy%port=0", bbWaitJoy);
    rtSym("%JoyWait%port=0", bbWaitJoy);
    rtSym("#JoyX%port=0", bbJoyX);
    rtSym("#JoyY%port=0", bbJoyY);
    rtSym("#JoyZ%port=0", bbJoyZ);
    rtSym("#JoyU%port=0", bbJoyU);
    rtSym("#JoyV%port=0", bbJoyV);
    rtSym("#JoyPitch%port=0", bbJoyPitch);
    rtSym("#JoyYaw%port=0", bbJoyYaw);
    rtSym("#JoyRoll%port=0", bbJoyRoll);
    rtSym("%JoyHat%port=0", bbJoyHat);
    rtSym("%JoyXDir%port=0", bbJoyXDir);
    rtSym("%JoyYDir%port=0", bbJoyYDir);
    rtSym("%JoyZDir%port=0", bbJoyZDir);
    rtSym("%JoyUDir%port=0", bbJoyUDir);
    rtSym("%JoyVDir%port=0", bbJoyVDir);
    rtSym("FlushJoy", bbFlushJoy);

    rtSym("EnableDirectInput%enable", bbEnableDirectInput);
    rtSym("%DirectInputEnabled", bbDirectInputEnabled);
}
