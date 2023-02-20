#pragma once

#include<main.h>
#include<Parameters.hpp>
#include<list>

namespace servo_md{
	//以下のクラスが要実装
	class MotorController{
		private:


			//PWMを出力するためのタイマー
			TIM_HandleTypeDef* pwm_tim;
			uint8_t pwm_channel;

			//パラメーターを保持する保管庫的なクラス
			Parameters& params;

			//角度計算用の変数
			float angle = 0;//rad
			uint16_t pwm_count_min = 0;//回
			uint16_t pwm_count_max = 0;//回

			//緊急停止用のフラグ
			//trueのときはモーターの角度を変えない
			bool is_emergency = false;

			static std::list<MotorController*> pInstances;

			//角度はラジアンで指定する
			void set_angle(float angle);

			//Emergencyスイッチが扱われたときに呼ばれるコールバック関数
			void emergency_callback(void){
				is_emergency = true;

				//PWMを止めると角度が0になるため、どちらが良いか？
				//HAL_TIM_PWM_Stop(pwm_tim, pwm_channel);
			}

		public:
			//コンストラクタ(default引数はTIM_CHANNEL_1を利用したSG90用)
			explicit MotorController(
				TIM_HandleTypeDef* pwm_tim,
				Parameters& params,
				unsigned long source_clock,
				uint8_t pwm_channel=TIM_CHANNEL_1,
				uint16_t pwm_cycle=20000,//microsec
				uint16_t pwm_pulse_min=500,//microsec
				uint16_t pwm_pulse_max=2400//microsec
			): pwm_tim(pwm_tim), pwm_channel(pwm_channel), params(params){
/// @attention プリスケール後の周波数は1MHz以下のすること
				//カウンタの増加周期　　microsec/回
				const uint16_t count_up_period = 1000000/(source_clock/(pwm_tim->Instance->PSC + 1));

				pwm_count_max = pwm_pulse_max/count_up_period;
				pwm_count_min = pwm_pulse_min/count_up_period;
				pwm_tim->Instance->ARR = pwm_cycle/count_up_period;

				HAL_TIM_PWM_Start(pwm_tim, pwm_channel);

				pInstances.push_back(this);
			}

			//パラメーターを保持する保管庫的なクラスを登録する関数
			void set_register(Parameters& params);

			//パラメーターの値を読み込み、それに従ってモーターに出力する関数
			//定期的に呼ばれる
			void update(){
				if(!is_emergency){
					//パラメーターの値を読み込む
					MotorParam motorparam = params.get_motor_params();
					angle = motorparam.target;
					//読み込んだ値をモーターに出力する
					set_angle(angle);
				}
			}

			static void trigger_emergency_callback(void);

			static void trigger_update(void);
	};
}
