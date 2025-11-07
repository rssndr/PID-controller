#include <stdint.h>

#define EXPECTED_DT_MS  5.0f
#define DT_MIN_MS       (EXPECTED_DT_MS * 0.8f)
#define DT_MAX_MS       (EXPECTED_DT_MS * 1.2f)

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
        pid->prev_time_ms = 0;
}

float pid_update(PID_Controller* pid, float feedback, uint32_t now_ms) {
        if (pid->prev_time_ms == 0) {
                pid->prev_time_ms = now_ms;
                pid->prev_feedback = feedback;
                return 0.0f;
        }

        float dt = (float)(now_ms - pid->prev_time_ms);
        if (dt < DT_MIN_MS) dt = DT_MIN_MS;
        if (dt > DT_MAX_MS) dt = DT_MAX_MS;
        dt /= 1000.0f;

        float error = pid->setpoint - feedback;

        float integral_new = pid->integral + error * dt;

        float derivative = (feedback - pid->prev_feedback) / dt;

        float output = pid->kp * error
                     + pid->ki * integral_new
                     - pid->kd * derivative;

        pid->integral_clamped = 0;
        if (output > pid->output_max) {
                output = pid->output_max;
                pid->integral_clamped = 1;
        } else if (output < pid->output_min) {
                output = pid->output_min;
                pid->integral_clamped = 1;
        }

        if (pid->integral_clamped && (error * (integral_new - pid->integral) > 0)) {
                integral_new = pid->integral;
        }

        pid->integral = integral_new;
        pid->prev_feedback = feedback;
        pid->prev_time_ms = now_ms;

        return output;
}

/* ******** SIMULATION ******** */
int main() {

        return 0;
}

