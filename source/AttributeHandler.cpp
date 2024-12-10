#include "AttributeHandler.h"

#include "LevelUpHandler.h"
#include "Settings.h"
#include "Utility.h"

void AttributeHandler::AttributeHandlerCallback::Run(RE::IMessageBoxCallback::Message a_message)
{
	switch (static_cast<std::int32_t>(a_message)) {
	case 0:
		{
			const auto message_queue = RE::UIMessageQueue::GetSingleton();
			const auto interface_strings = RE::InterfaceStrings::GetSingleton();
			const auto player = RE::PlayerCharacter::GetSingleton();
			const auto av_owner = player ? player->AsActorValueOwner() : nullptr;

			if (!message_queue || !interface_strings || !av_owner) {
				return LevelUpHandler::ConstructMessage(menu, attribute);
			}

			const auto attribute_list = AttributeHandler::GetSingleton()->GetAttributeList(attribute, derived);

			for (const auto& a : attribute_list) {
				av_owner->ModActorValue(a.first, a.second);
			}

			player->GetPlayerRuntimeData().skills->AdvanceLevel(0.f);
			menu->GetRuntimeData().playerLeveled = true;
			message_queue->AddMessage(interface_strings->levelUpMenu, RE::UI_MESSAGE_TYPE::kHide, nullptr);
		}
		break;
	case 1:
		return LevelUpHandler::ConstructMessage(menu, attribute);
	}
}

auto AttributeHandler::GetSingleton() -> AttributeHandler*
{
	static AttributeHandler singleton;

	return std::addressof(singleton);
}

void AttributeHandler::ConstructMessage(RE::LevelUpMenu* a_menu, const RE::ActorValue a_attribute, const RE::ActorValue a_derived)
{
	INFO("AttributeHandler::ConstructMessage ~ Attribute: <{}>, Derived: <{}>", Utility::FormatActorValueName(a_attribute), Utility::FormatActorValueName(a_derived));

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

	const auto sDAF_AttributesBeingIncreased = game_settings->GetSetting("sDAF_AttributesBeingIncreased");
	const auto sDAF_Proceed = game_settings->GetSetting("sDAF_Proceed");

	const auto sYes = game_settings->GetSetting("sYes");
	const auto sNo = game_settings->GetSetting("sNo");

	if (!sDAF_AttributesBeingIncreased || !sDAF_Proceed || !sYes || !sNo) {
		return;
	}

	message->menuDepth = 4;
	message->useHtml = true;

	const auto attribute_list = AttributeHandler::GetSingleton()->GetAttributeStringList(a_attribute, a_derived);

	auto body_text = std::format("<font face='$EverywhereMediumFont'><p>{}</p><br>", sDAF_AttributesBeingIncreased->GetString());

	for (const auto& attribute : attribute_list) {
		body_text += std::format("<p>{}: <font color='#{:X}'>{}</font> >> <font color='#{:X}'>{}</font></p>", std::get<0>(attribute), Settings::CurrentValueColor, std::get<1>(attribute), Settings::UpdatedValueColor, std::get<2>(attribute));
	}

	body_text += std::format("<br><p>{}</p></font>", sDAF_Proceed->GetString());

	message->bodyText = body_text;

	message->buttonText.push_back(sYes->GetString());
	message->buttonText.push_back(sNo->GetString());

	message->isCancellable = true;
	message->cancelOptionIndex = message->buttonText.size() - 1;

	message->callback = RE::BSTSmartPointer<RE::IMessageBoxCallback>{ new AttributeHandlerCallback(a_menu, a_attribute, a_derived) };

	message->QueueMessage();
}

auto AttributeHandler::GetAttributeList(const RE::ActorValue a_attribute, const RE::ActorValue a_derived) const -> std::unordered_map<RE::ActorValue, float>
{
	std::unordered_map<RE::ActorValue, float> result;

	if (attributes.contains(a_attribute)) {
		const auto& a_a = attributes.at(a_attribute);
		const auto a_b = a_a.attribute.global.has_value() && a_a.attribute.global.value() ? a_a.attribute.global.value()->value : a_a.attribute.weight;

		result.emplace(a_attribute, a_b);

		if (a_a.extra.has_value()) {
			const auto& a_c = a_a.extra.value();

			for (const auto& extra : a_c) {
				const auto a_d = extra.second.global.has_value() && extra.second.global.value() ? extra.second.global.value()->value : extra.second.weight;

				result.emplace(extra.first, a_d);
			}
		}

		if (a_a.derived.contains(a_derived)) {
			const auto& b_a = a_a.derived.at(a_derived);
			const auto b_b = b_a.attribute.global.has_value() && b_a.attribute.global.value() ? b_a.attribute.global.value()->value : b_a.attribute.weight;

			result.emplace(a_derived, b_b);

			if (b_a.extra.has_value()) {
				const auto& b_c = b_a.extra.value();

				for (const auto& extra : b_c) {
					const auto b_d = extra.second.global.has_value() && extra.second.global.value() ? extra.second.global.value()->value : extra.second.weight;

					result.emplace(extra.first, b_d);
				}
			}
		}
	}

	return result;
}

auto AttributeHandler::GetAttributeStringList(const RE::ActorValue a_attribute, const RE::ActorValue a_derived) const -> std::vector<std::tuple<std::string, float, float>>
{
	std::vector<std::tuple<std::string, float, float>> result;

	const auto player = RE::PlayerCharacter::GetSingleton();
	const auto av_owner = player ? player->AsActorValueOwner() : nullptr;

	if (!av_owner) {
		return result;
	}

	if (attributes.contains(a_attribute)) {
		const auto& a_a = attributes.at(a_attribute);
		const auto a_b = a_a.attribute.global.has_value() && a_a.attribute.global.value() ? a_a.attribute.global.value()->value : a_a.attribute.weight;
		const auto a_c = av_owner->GetBaseActorValue(a_attribute);

		result.push_back({ Utility::FormatActorValueName(a_attribute), a_c, std::trunc((a_c + a_b) * 10000.f) / 10000.f });

		if (a_a.extra.has_value()) {
			const auto& a_d = a_a.extra.value();

			for (const auto& extra : a_d) {
				const auto b_a = extra.second.global.has_value() && extra.second.global.value() ? extra.second.global.value()->value : extra.second.weight;
				const auto b_b = av_owner->GetBaseActorValue(extra.first);

				result.push_back({ Utility::FormatActorValueName(extra.first), b_b, std::trunc((b_b + b_a) * 10000.f) / 10000.f });
			}
		}

		if (a_a.derived.contains(a_derived)) {
			const auto& c_a = a_a.derived.at(a_derived);
			const auto c_b = c_a.attribute.global.has_value() && c_a.attribute.global.value() ? c_a.attribute.global.value()->value : c_a.attribute.weight;
			const auto c_c = av_owner->GetBaseActorValue(a_derived);

			result.push_back({ Utility::FormatActorValueName(a_derived), c_c, std::trunc((c_c + c_b) * 10000.f) / 10000.f });

			if (c_a.extra.has_value()) {
				const auto& c_d = c_a.extra.value();

				for (const auto& extra : c_d) {
					const auto d_a = extra.second.global.has_value() && extra.second.global.value() ? extra.second.global.value()->value : extra.second.weight;
					const auto d_b = av_owner->GetBaseActorValue(extra.first);

					result.push_back({ Utility::FormatActorValueName(extra.first), d_b, std::trunc((d_b + d_a) * 10000.f) / 10000.f });
				}
			}
		}
	}

	return result;
}

auto AttributeHandler::GetDerivedAttributes(const RE::ActorValue a_attribute) const -> std::vector<RE::ActorValue>
{
	std::vector<RE::ActorValue> temporary;

	if (attributes.contains(a_attribute)) {
		for (const auto& derived : attributes.at(a_attribute).derived) {
			temporary.push_back(derived.first);
		}
	}

	return temporary;
}

void AttributeHandler::Load()
{
	attributes = Settings::Load();
}