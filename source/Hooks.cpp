#include "Hooks.h"

#include "LevelUpHandler.h"

namespace Hooks
{
	template <RE::ActorValue T>
	struct InterceptMessage
	{
		static void Call(RE::LevelUpMenu* a_menu, RE::ActorValue a_attribute = T)
		{
			return LevelUpHandler::ConstructMessage(a_menu, a_attribute);
		}
	};

	void Install()
	{
		REL::Relocation H{ RELOCATION_ID(51033, 51910), 0xE };
		REL::Relocation M{ RELOCATION_ID(51034, 51912), 0xE };
		REL::Relocation S{ RELOCATION_ID(51035, 51914), 0xE };

		H.write_branch<5>(InterceptMessage<RE::ActorValue::kHealth>::Call);
		INFO("Hooks::Install ~ Hooked <{}>", typeid(InterceptMessage<RE::ActorValue::kHealth>).name());

		M.write_branch<5>(InterceptMessage<RE::ActorValue::kMagicka>::Call);
		INFO("Hooks::Install ~ Hooked <{}>", typeid(InterceptMessage<RE::ActorValue::kMagicka>).name());

		S.write_branch<5>(InterceptMessage<RE::ActorValue::kStamina>::Call);
		INFO("Hooks::Install ~ Hooked <{}>", typeid(InterceptMessage<RE::ActorValue::kStamina>).name());
	}
}