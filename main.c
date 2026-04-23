#include <stdio.h>
#include <unistd.h>
#include "pid.h"

typedef struct {
        float x, y, z;
        float vx, vy, vz;
        float roll, pitch, yaw;
        float p, q, r;
} QuadState;

#define GRAVITY         9.81f          // m/s²
#define MASS            0.25f           // kg
#define ARM_LENGTH      0.2f           // meters (center to motor)
#define THRUST_FACTOR   0.005f        // motor_cmd → Newtons
#define TORQUE_FACTOR   0.000001f      // differential → torque
#define Ixx             0.01f          // kg·m²
#define Iyy             0.01f
#define Izz             0.02f

#define DEG_TO_RAD(deg) ((deg) * 0.01745329252f)
#define RAD_TO_DEG(rad) ((rad) * 57.2957795131f)

int main() {
        QuadState quad = {0};
        quad.z  = 0.0f;
        quad.vz = 0.0f;
        quad.roll  = DEG_TO_RAD(0.0f);
        quad.pitch = DEG_TO_RAD(0.0f);
        quad.yaw   = DEG_TO_RAD(0.0f);
        quad.p = 0.0f;
        quad.q = 0.0f;
        quad.r = 0.0f;

        PID_Controller altitude_pid;
        pid_init(&altitude_pid, 1300.0f, 1000.0f, 200.0f, 100.0f, 2000.0f);
        altitude_pid.setpoint = 1.0f;

        float dt = 0.005f;
        uint32_t now_ms = 0;
        uint32_t next_print_ms = 0;

        for (int step = 0; step < 4000; step++) {
                now_ms += (uint32_t)(dt * 1000.0f);

                float thrust_cmd = pid_update(&altitude_pid, quad.z, now_ms);
                float thrust = thrust_cmd * THRUST_FACTOR;

                float az = (thrust / MASS) - GRAVITY;
                quad.vz += az * dt;
                quad.z += quad.vz * dt;

                if (quad.z < 0.0f) {
                        quad.z = 0;
                        quad.vz = 0;
                }

                if (now_ms >= 8000 && now_ms < 8100) {
                        // 20 N gust for 100 ms
                        quad.vz += 20.0f * dt;
                }

                if (now_ms >= next_print_ms) {
                        printf("t=%.3f | z=%.4f m | vz=%.4f m/s\n",
                               now_ms / 1000.0f, quad.z, quad.vz);
                        next_print_ms += 100;
                }

                usleep(5000);
        }

        return 0;
}

