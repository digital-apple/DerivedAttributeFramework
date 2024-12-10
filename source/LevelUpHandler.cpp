#include "LevelUpHandler.h"

#include "AttributeHandler.h"
#include "Settings.h"
#include "Utility.h"

void LevelUpHandler::ConfirmLevelUpAttributeCallbackEx::Run(RE::IMessageBoxCallback::Message a_message)
{
	const auto message = static_cast<std::int32_t>(a_message);

	if (message == cancel_index) {
		return;
	}

	AttributeHandler::ConstructMessage(menu, attribute, derived_map[message]);
}

auto LevelUpHandler::GetSingleton() -> LevelUpHandler*
{
	static LevelUpHandler singleton;

	return std::addressof(singleton);
}

void LevelUpHandler::ConstructMessage(RE::LevelUpMenu* a_menu, const RE::ActorValue a_attribute)
{
	if (!a_menu) {
		return;
	}

	const auto factory_manager = RE::MessageDataFactoryManager::GetSingleton();
	const auto game_settings = RE::GameSettingCollection::GetSingleton();
	const auto interface_strings = RE::InterfaceStrings::GetSingleton();

	if (!factory_manager || !game_settings || !interface_strings) {
		return;
	}

	const auto message_factory = factory_manager->GetCreator<RE::MessageBoxData>(interface_strings->messageBoxData);
	const auto message = message_factory ? message_factory->Create() : nullptr;

	if (!message) {
		return;
	}

	const auto sDAF_CurrentlyIncreasing = game_settings->GetSetting("sDAF_CurrentlyIncreasing");
	const auto sDAF_Description = game_settings->GetSetting("sDAF_Description");

	const auto sCancel = game_settings->GetSetting("sCancel");

	if (!sDAF_CurrentlyIncreasing || !sDAF_Description || !sCancel) {
		return;
	}

	char buffer[0x104];
	std::snprintf(buffer, sizeof(buffer), sDAF_CurrentlyIncreasing->GetString(), Utility::FormatActorValueName(a_attribute).data());

	std::string body_text = std::string(buffer) + "<br><br>" + sDAF_Description->GetString();

	message->menuDepth = 4;
	message->useHtml = true;
	message->bodyText = body_text;

	std::unordered_map<std::size_t, RE::ActorValue> derived_map;

	const auto derived_attributes = AttributeHandler::GetSingleton()->GetDerivedAttributes(a_attribute);

	for (std::size_t i = 0; i < derived_attributes.size(); i++) {
		auto text = Utility::FormatActorValueName(derived_attributes[i]);

		if (Settings::LineBreak) {
			std::size_t whitespace = text.find(' ');

			if (whitespace != std::string::npos) {
				text.replace(whitespace, 1, "\n");
			}
		}

		message->buttonText.push_back(std::format("<font color='#{:X}'>{}</font>", Settings::ButtonColor, text.data()).data());
		derived_map.emplace(i, derived_attributes[i]);
	}

	message->buttonText.push_back(sCancel->GetString());

	message->isCancellable = true;
	message->cancelOptionIndex = message->buttonText.size() - 1;

	message->callback = RE::BSTSmartPointer<RE::IMessageBoxCallback>{ new ConfirmLevelUpAttributeCallbackEx(a_menu, a_attribute, derived_map, message->buttonText.size() - 1) };

	message->QueueMessage();
}