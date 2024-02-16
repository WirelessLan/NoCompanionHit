#pragma once

namespace Configs {
	extern bool g_enabled;
	extern bool g_enableIgnorePlayerProjectileWhenInCombat;
	extern bool g_enableIgnorePlayerToAllyProjectileHit;
	extern bool g_enableIgnoreAllyToPlayerProjectileHit;
	extern bool g_enableIgnoreAllyToAllyProjectileHit;
	extern bool g_enableIgnoreBeamProjectile;
	extern bool g_enableIgnoreFlameProjectile;
	extern bool g_enableIgnoreMissileProjectile;

	void ReadINI();
}
