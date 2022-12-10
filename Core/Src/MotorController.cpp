//ステッピングモーターコントロール用クラス
#include<MotorController.hpp>
#include <numbers>

namespace servo_md
{
	//角度からPWMのパルス幅を計算する関数
    void MotorController::set_angle(float angle)
    {
        //角度からパルス幅を計算する angle [0,pi] -> count [pwm_count_min,pwm_count_max]
        uint16_t count = (uint16_t)(pwm_count_min + (pwm_count_max - pwm_count_min) * angle / std::numbers::pi);
        //パルス幅を設定する
        __HAL_TIM_SET_COMPARE(pwm_tim, pwm_channel, count);
    }

    void MotorController::set_register(Parameters &parameters)
    {
        this->params = parameters;
    }





} // namespace servo_md
