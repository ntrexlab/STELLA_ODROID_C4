#define wheel_to_wheel_d            0.13       // 바퀴와 바퀴 간 거리  [m]
#define distance_per_rev            0.2041        // 한바퀴 회전시 이동 거리 [m / rev]
#define pulse_per_rev               60000        // 한바퀴 회전시 엔코더 펄스 카운트(이 값은 부착된 엔코더와 감속기를 고려해 정해진다.) [pulse / rev]
#define pulse_per_distance          293973.542381    // 1m 이동시 엔코더 펄스 카운트 [pulse / m]
#define gear_ratio                  30           // 감속비
#define motor_scale_const           293.8245103  // m/s to rpm



void calculate_wheel_vel(float linear, float angular, float *left_rpm,  float *right_rpm);
