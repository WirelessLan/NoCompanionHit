#include "Hooks.h"

#include "Configs.h"

namespace Hooks {
	bool IsPlayerTeammate(RE::Actor* a_actor) {
		if (!a_actor)
			return false;
		return (a_actor->niFlags.flags >> 26) & 1;
	}

	bool IgnoreProjectile(RE::Projectile& a_projectile, RE::TESObjectREFR* a_ref) {
		if (!a_ref)
			return false;

		auto shooterPtr = a_projectile.shooter.get();
		if (!shooterPtr.get())
			return false;

		RE::Actor* shooterActorRef = shooterPtr->As<RE::Actor>();
		if (!shooterActorRef)
			return false;

		RE::Actor* targetActorRef = a_ref->As<RE::Actor>();
		if (!targetActorRef)
			return false;

		if (shooterActorRef->IsPlayerRef()) {
			// 플레이어->동료
			if (Configs::g_enableIgnorePlayerToAllyProjectileHit && IsPlayerTeammate(targetActorRef)) {
				if (Configs::g_enableIgnorePlayerProjectileWhenInCombat && !shooterActorRef->IsInCombat())
					return false;
				return true;
			}
		}
		else if (IsPlayerTeammate(shooterActorRef)) {
			// 동료->플레이어
			if (Configs::g_enableIgnoreAllyToPlayerProjectileHit && targetActorRef->IsPlayerRef())
				return true;
			else if (Configs::g_enableIgnoreAllyToAllyProjectileHit && IsPlayerTeammate(targetActorRef))
				return true;
		}

		return false;
	}

	struct ImpactCreation {
		std::uint64_t		unk00[0x20 >> 3];	// 00
		RE::TESObjectREFR* impactTarget;		// 20
	};

	template<std::uint64_t id, std::ptrdiff_t diff>
	class BeamProjectile_AddImpactHook {
	public:
		static void Install() {
			REL::Relocation<std::uintptr_t> target(REL::ID(id), diff);
			func = *(func_t*)(target.get());
			REL::safe_write(target.get(), (std::uintptr_t)ProcessHook);
		}
	private:
		using func_t = std::uint32_t(*)(RE::Projectile&, const ImpactCreation&);

		static std::uint32_t ProcessHook(RE::Projectile& a_projectile, const ImpactCreation& a_impactCreation) {
			if (Configs::g_enabled && Configs::g_enableIgnoreBeamProjectile) {
				if (IgnoreProjectile(a_projectile, a_impactCreation.impactTarget))
					return static_cast<std::uint32_t>(-1);
			}
			return func(a_projectile, a_impactCreation);
		}

		inline static func_t func;
	};

	template<std::uint64_t id, std::ptrdiff_t diff>
	class FlameProjectile_AddImpactHook {
	public:
		static void Install() {
			REL::Relocation<std::uintptr_t> target(REL::ID(id), diff);
			func = *(func_t*)(target.get());
			REL::safe_write(target.get(), (std::uintptr_t)ProcessHook);
		}
	private:
		using func_t = std::uint32_t(*)(RE::Projectile&, const ImpactCreation&);

		static std::uint32_t ProcessHook(RE::Projectile& a_projectile, const ImpactCreation& a_impactCreation) {
			if (Configs::g_enabled && Configs::g_enableIgnoreFlameProjectile) {
				if (IgnoreProjectile(a_projectile, a_impactCreation.impactTarget))
					return static_cast<std::uint32_t>(-1);
			}
			return func(a_projectile, a_impactCreation);
		}

		inline static func_t func;
	};

	template<std::uint64_t id, std::ptrdiff_t diff>
	class MissileProjectile_HandleHitsHook {
	public:
		static void Install() {
			REL::Relocation<std::uintptr_t> target(REL::ID(id), diff);
			func = *(func_t*)(target.get());
			REL::safe_write(target.get(), (std::uintptr_t)ProcessHook);
		}
	private:
		using func_t = bool(*)(RE::Projectile&, const RE::hknpAllHitsCollector&, const RE::NiPoint3&, const RE::NiPoint3&);

		static bool ProcessHook(RE::Projectile& a_projectile, const RE::hknpAllHitsCollector& a_allHitsCollector, const RE::NiPoint3& a_arg3, const RE::NiPoint3& a_arg4) {
			if (Configs::g_enabled && Configs::g_enableIgnoreMissileProjectile) {
				const auto hitsArr = a_allHitsCollector.GetHits();
				if (!hitsArr)
					return func(a_projectile, a_allHitsCollector, a_arg3, a_arg4);

				RE::hknpAllHitsCollector& modifiedAllHitsCollector = const_cast<RE::hknpAllHitsCollector&>(a_allHitsCollector);
				std::vector<RE::hknpCollisionResult> modifiedCollectionResultVec;

				for (std::int32_t ii = 0; ii < a_allHitsCollector.GetNumHits(); ii++) {
					const auto& collisionResult = hitsArr[ii];
					if (collisionResult.hitBodyInfo.m_shapeKey.storage != 0xFFFFFFFF) {
						modifiedCollectionResultVec.push_back(collisionResult);
						continue;
					}

					RE::NiCollisionObject* collisionObj = reinterpret_cast<RE::NiCollisionObject*>(collisionResult.hitBodyInfo.m_shapeUserData.storage);
					if (!collisionObj || !collisionObj->sceneObject) {
						modifiedCollectionResultVec.push_back(collisionResult);
						continue;
					}

					RE::TESObjectREFR* ref = reinterpret_cast<RE::TESObjectREFR*>(collisionObj->sceneObject->userData);

					if (IgnoreProjectile(a_projectile, ref))
						continue;

					modifiedCollectionResultVec.push_back(collisionResult);
				}

				modifiedAllHitsCollector.hits._size = static_cast<std::int32_t>(modifiedCollectionResultVec.size());

				for (std::uint32_t ii = 0; ii < modifiedCollectionResultVec.size(); ii++)
					modifiedAllHitsCollector.hits._data[ii] = modifiedCollectionResultVec[ii];

				return func(a_projectile, modifiedAllHitsCollector, a_arg3, a_arg4);
			}
			return func(a_projectile, a_allHitsCollector, a_arg3, a_arg4);
		}

		inline static func_t func;
	};

	void Install() {
		BeamProjectile_AddImpactHook<700255, 0x720>::Install();
		FlameProjectile_AddImpactHook<407784, 0x720>::Install();
		MissileProjectile_HandleHitsHook<1432029, 0x728>::Install();
	}
}
