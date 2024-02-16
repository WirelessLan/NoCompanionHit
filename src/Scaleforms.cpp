#include "Scaleforms.h"

#include "Configs.h"

namespace Scaleforms {
	class SetOptionHandler : public RE::Scaleform::GFx::FunctionHandler {
	public:
		virtual void Call(const Params& a_params) override {
			if (a_params.argCount != 2 || a_params.args[0].GetType() != RE::Scaleform::GFx::Value::ValueType::kString)
				return;

			if (strcmp(a_params.args[0].GetString(), "bEnabled") == 0)
				Configs::g_enabled = a_params.args[1].GetBoolean();
			else if (strcmp(a_params.args[0].GetString(), "bEnableIgnorePlayerProjectileWhenInCombat") == 0)
				Configs::g_enableIgnorePlayerProjectileWhenInCombat = a_params.args[1].GetBoolean();
			else if (strcmp(a_params.args[0].GetString(), "bEnableIgnorePlayerToAllyProjectileHit") == 0)
				Configs::g_enableIgnorePlayerToAllyProjectileHit = a_params.args[1].GetBoolean();
			else if (strcmp(a_params.args[0].GetString(), "bEnableIgnoreAllyToPlayerProjectileHit") == 0)
				Configs::g_enableIgnoreAllyToPlayerProjectileHit = a_params.args[1].GetBoolean();
			else if (strcmp(a_params.args[0].GetString(), "bEnableIgnoreAllyToAllyProjectileHit") == 0)
				Configs::g_enableIgnoreAllyToAllyProjectileHit = a_params.args[1].GetBoolean();
			else if (strcmp(a_params.args[0].GetString(), "bEnableIgnoreBeamProjectile") == 0)
				Configs::g_enableIgnoreBeamProjectile = a_params.args[1].GetBoolean();
			else if (strcmp(a_params.args[0].GetString(), "bEnableIgnoreFlameProjectile") == 0)
				Configs::g_enableIgnoreFlameProjectile = a_params.args[1].GetBoolean();
			else if (strcmp(a_params.args[0].GetString(), "bEnableIgnoreMissileProjectile") == 0)
				Configs::g_enableIgnoreMissileProjectile = a_params.args[1].GetBoolean();
		}
	};

	void RegisterFunction(RE::Scaleform::GFx::Movie* a_view, RE::Scaleform::GFx::Value* a_f4se_root, RE::Scaleform::GFx::FunctionHandler* a_handler, F4SE::stl::zstring a_name) {
		RE::Scaleform::GFx::Value fn;
		a_view->CreateFunction(&fn, a_handler);
		a_f4se_root->SetMember(a_name, fn);
	}

	void Register(RE::Scaleform::GFx::Movie* a_view, RE::Scaleform::GFx::Value* a_f4se_root) {
		RegisterFunction(a_view, a_f4se_root, new SetOptionHandler(), "SetOption"sv);
	}
}
