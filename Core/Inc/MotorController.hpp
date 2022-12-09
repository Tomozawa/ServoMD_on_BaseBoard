#pragma once

#include<main.h>
#include<Parameters.hpp>
#include<list>

namespace servo_md{
	//MotorControllerに最低限必要な実装
	//基本的にはそのままにしておくこと
	class MotorController_Base{
		private:
			static std::list<MotorController_Base> instances;
		protected:
			explicit MotorController_Base(){instances.push_back(*this);}
		public:
			inline static void trigger_emergency_callback(void){
				for(MotorController_Base controller : instances){
					controller.emergency_callback();
				}
			}

			inline static void trigger_update(void){
				for(MotorController_Base controller : instances){
					controller.update();
				}
			}

			virtual void emergency_callback(void){throw std::logic_error("emergency_callback is not implemented");}
			virtual void update(void){}
	};

	//以下のクラスが要実装
	class MotorController : public MotorController_Base{
		public:
			//コンストラクタ(引数やオーバーロードは自由に決めてよい)
			explicit MotorController(TIM_HandleTypeDef* pwm_tim){}

			//パラメーターを保持する保管庫的なクラスを登録する関数
			void set_register(Parameters& params);

			//パラメーターの値を読み込み、それに従ってモーターに出力する関数
			//定期的に呼ばれる
			void update() override;

			//Emergencyスイッチが扱われたときに呼ばれるコールバック関数
			void emergency_callback(void) override;
	};
}
