/* *****************************************************************
PID Controller for Pitch Angle
***************************************************************** */
#include <stdint.h>

typedef struct {
        float setpoint;
        float prev_feedback;
        float integral;
        float kp, ki, kd;
        float output_min;
        float output_max;
        uint32_t prev_time_ms;
        uint8_t integral_clamped;
} PID_Controller;

void pid_init(PID_Controller* pid,
              float kp, float ki, float kd,
              float out_min, float out_max) {
        pid->setpoint = 0;
        pid->prev_feedback = 0;
        pid->integral = 0;
        pid->kp = kp;
        pid->ki = ki;
        pid->kd = kd;
        pid->output_min = out_min;
        pid->output_max = out_max;
        pid->integral_clamped = 0;
}

int main() {

        return 0;
}

