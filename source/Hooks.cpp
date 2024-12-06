#include "Hooks.h"

namespace Hooks
{
	struct Run
	{
		using ConfirmLevelUpAttributeCallback = RE::LevelUpMenu::ConfirmLevelUpAttributeCallback;
		using Message = RE::IMessageBoxCallback::Message;

		static void Call(ConfirmLevelUpAttributeCallback* a_callback, Message a_message)
		{
			INFO("Run ~ Call!");

			const auto ui_manager = RE::UI::GetSingleton();
			const auto interface_strings = RE::InterfaceStrings::GetSingleton();

			if (!ui_manager || !interface_strings || !ui_manager->IsMenuOpen(interface_strings->statsMenu)) {
				return;
			}

			switch (a_message) {
			case Message::kUnk0:
				{
					const auto message_queue = RE::UIMessageQueue::GetSingleton();

					if (!message_queue) {
						return;
					}

					const auto player = RE::PlayerCharacter::GetSingleton();
					const auto av_owner = player ? player->AsActorValueOwner() : nullptr;

					if (!av_owner) {
						return;
					}

					const auto value = a_callback->actorValue;

					switch (value) {
					case RE::ActorValue::kHealth:
						{
							av_owner->ModActorValue(value, 11.f);
							player->GetPlayerRuntimeData().skills->AdvanceLevel(0.f);
						}
						break;
					case RE::ActorValue::kMagicka:
						{
							av_owner->ModActorValue(value, 22.f);
							player->GetPlayerRuntimeData().skills->AdvanceLevel(0.f);
						}
						break;
					case RE::ActorValue::kStamina:
						{
							av_owner->ModActorValue(value, 33.f);
							player->GetPlayerRuntimeData().skills->AdvanceLevel(0.f);
						}
						break;
					default:
						break;
					}

					message_queue->AddMessage(interface_strings->levelUpMenu, RE::UI_MESSAGE_TYPE::kHide, nullptr);
				}
				break;
			case Message::kUnk1:
			case Message::kUnk2:
				{
					const auto menu = a_callback->menu;

					if (!menu) {
						return;
					}

					menu->GetRuntimeData().playerLeveled = false;
				}
				break;
			}
		}
		static inline REL::Relocation<decltype(Call)> Callback;
	};

	void Install()
	{
		stl::write_vfunc<RE::LevelUpMenu::ConfirmLevelUpAttributeCallback, 0x1, Run>();

		INFO("Hooks ~ Installed <{}>", typeid(Run).name());
	}
}