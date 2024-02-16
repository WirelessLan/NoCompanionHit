#include "Configs.h"

#include <Windows.h>

namespace Configs {
	bool g_enabled = true;
	bool g_enableIgnorePlayerProjectileWhenInCombat = true;
	bool g_enableIgnorePlayerToAllyProjectileHit = true;
	bool g_enableIgnoreAllyToPlayerProjectileHit = true;
	bool g_enableIgnoreAllyToAllyProjectileHit = true;

	bool g_enableIgnoreBeamProjectile = true;
	bool g_enableIgnoreFlameProjectile = true;
	bool g_enableIgnoreMissileProjectile = true;

	std::string GetINIOption(const char* section, const char* key) {
		std::string	result;
		char resultBuf[256] = { 0 };

		static const std::string& configPath = "Data\\MCM\\Settings\\" + std::string(Version::PROJECT) + ".ini";
		GetPrivateProfileStringA(section, key, NULL, resultBuf, sizeof(resultBuf), configPath.c_str());
		return resultBuf;
	}

	void ReadINI() {
		std::string value;

		value = GetINIOption("Settings", "bEnabled");
		if (!value.empty()) {
			try {
				g_enabled = std::stoul(value);
			}
			catch (...) {}
		}
		logger::info(FMT_STRING("bEnabled: {}"), g_enabled);

		value = GetINIOption("Settings", "bEnableIgnorePlayerProjectileWhenInCombat");
		if (!value.empty()) {
			try {
				g_enableIgnorePlayerProjectileWhenInCombat = std::stoul(value);
			}
			catch (...) {}
		}
		logger::info(FMT_STRING("bEnableIgnorePlayerProjectileWhenInCombat: {}"), g_enableIgnorePlayerProjectileWhenInCombat);

		value = GetINIOption("Settings", "bEnableIgnorePlayerToAllyProjectileHit");
		if (!value.empty()) {
			try {
				g_enableIgnorePlayerToAllyProjectileHit = std::stoul(value);
			}
			catch (...) {}
		}
		logger::info(FMT_STRING("bEnableIgnorePlayerToAllyProjectileHit: {}"), g_enableIgnorePlayerToAllyProjectileHit);

		value = GetINIOption("Settings", "bEnableIgnoreAllyToPlayerProjectileHit");
		if (!value.empty()) {
			try {
				g_enableIgnoreAllyToPlayerProjectileHit = std::stoul(value);
			}
			catch (...) {}
		}
		logger::info(FMT_STRING("bEnableIgnoreAllyToPlayerProjectileHit: {}"), g_enableIgnoreAllyToPlayerProjectileHit);

		value = GetINIOption("Settings", "bEnableIgnoreAllyToAllyProjectileHit");
		if (!value.empty()) {
			try {
				g_enableIgnoreAllyToAllyProjectileHit = std::stoul(value);
			}
			catch (...) {}
		}
		logger::info(FMT_STRING("bEnableIgnoreAllyToAllyProjectileHit: {}"), g_enableIgnoreAllyToAllyProjectileHit);

		value = GetINIOption("Settings", "bEnableIgnoreBeamProjectile");
		if (!value.empty()) {
			try {
				g_enableIgnoreBeamProjectile = std::stoul(value);
			}
			catch (...) {}
		}
		logger::info(FMT_STRING("bEnableIgnoreBeamProjectile: {}"), g_enableIgnoreBeamProjectile);

		value = GetINIOption("Settings", "bEnableIgnoreFlameProjectile");
		if (!value.empty()) {
			try {
				g_enableIgnoreFlameProjectile = std::stoul(value);
			}
			catch (...) {}
		}
		logger::info(FMT_STRING("bEnableIgnoreFlameProjectile: {}"), g_enableIgnoreFlameProjectile);

		value = GetINIOption("Settings", "bEnableIgnoreMissileProjectile");
		if (!value.empty()) {
			try {
				g_enableIgnoreMissileProjectile = std::stoul(value);
			}
			catch (...) {}
		}
		logger::info(FMT_STRING("bEnableIgnoreMissileProjectile: {}"), g_enableIgnoreMissileProjectile);
	}
}
