#include <stdio.h>
#include "pid.h"

typedef struct {
        float x, y, z;
        float vx, vy, vz;
        float roll, pitch, yaw;
        float p, q, r;
} QuadState;

#define GRAVITY         9.81f          // m/s²
#define MASS            1.0f           // kg
#define ARM_LENGTH      0.2f           // meters (center to motor)
#define THRUST_FACTOR   0.00005f       // motor_cmd → Newtons
#define TORQUE_FACTOR   0.000001f      // differential → torque
#define Ixx             0.01f          // kg·m²
#define Iyy             0.01f
#define Izz             0.02f

#define DEG_TO_RAD(deg) ((deg) * 0.01745329252f)
#define RAD_TO_DEG(rad) ((rad) * 57.2957795131f)

int main() {
        QuadState quad = {0};
        quad.z  = 1.0f;
        quad.vz = 0.0f;
        quad.roll  = DEG_TO_RAD(0.0f);
        quad.pitch = DEG_TO_RAD(0.0f);
        quad.yaw   = DEG_TO_RAD(0.0f);
        quad.p = 0.0f;
        quad.q = 0.0f;
        quad.r = 0.0f;

        float dt = 0.005f;
        uint32_t now_ms = 0;
        uint32_t next_print_ms = 0;

        for (int step = 0; step < 400; step++) {
                now_ms += (uint32_t)(dt * 1000.0f);

                float az = -GRAVITY;
                quad.vz += az * dt;
                quad.z += quad.vz * dt;

                if (now_ms >= next_print_ms) {
                        printf("t=%.3f | z=%.4f m | vz=%.4f m/s\n",
                               now_ms / 1000.0f, quad.z, quad.vz);
                        next_print_ms += 100;
                }
        }

        return 0;
}

