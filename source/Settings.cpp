#include "Settings.h"

#include "Utility.h"

auto Settings::Load() -> std::unordered_map<RE::ActorValue, AttributeHandler::AttributeList>
{
	INFO("Settings::Load ~ Trying to load settings...");

	std::unordered_map<RE::ActorValue, AttributeHandler::AttributeList> result;

	try {
		const auto settings = toml::parse(TOML_PATH);

		REQ_CoreScripts_AttributeSystem = Utility::GetFormID(toml::find<std::string_view>(settings, "REQ_CoreScripts_AttributeSystem"));

		LineBreak = toml::find<bool>(settings, "LineBreak");

		CurrentValueColor = toml::find<std::uint32_t>(settings, "CurrentValueColor");
		UpdatedValueColor = toml::find<std::uint32_t>(settings, "UpdatedValueColor");
		ButtonColor = toml::find<std::uint32_t>(settings, "ButtonColor");

		const auto& attributes = toml::find<toml::array>(settings, "Attributes");

		for (const auto& attribute : attributes) {
			const auto a_a = Utility::GetActorValue(toml::find<std::string_view>(attribute, "Attribute"));
			const auto a_b = toml::find<float>(attribute, "Weight");
			const auto a_c = attribute.contains("Global") ? Utility::GetForm<RE::TESGlobal>(toml::find<std::string_view>(attribute, "Global")) : std::nullopt;

			std::unordered_map<RE::ActorValue, AttributeHandler::DerivedAttribute> temporary_derived;

			const auto& deriveds = toml::find<toml::array>(attribute, "Derived");

			for (const auto& derived : deriveds) {
				const auto b_a = Utility::GetActorValue(toml::find<std::string_view>(derived, "Attribute"));
				const auto b_b = toml::find<float>(derived, "Weight");
				const auto b_c = derived.contains("Global") ? Utility::GetForm<RE::TESGlobal>(toml::find<std::string_view>(derived, "Global")) : std::nullopt;

				std::optional<std::unordered_map<RE::ActorValue, AttributeHandler::Attribute>> temporary_extra;

				if (derived.contains("Extra")) {
					const auto& extras = toml::find<toml::array>(derived, "Extra");

					for (const auto& extra : extras) {
						const auto c_a = Utility::GetActorValue(toml::find<std::string_view>(extra, "Attribute"));
						const auto c_b = toml::find<float>(extra, "Weight");
						const auto c_c = extra.contains("Global") ? Utility::GetForm<RE::TESGlobal>(toml::find<std::string_view>(extra, "Global")) : std::nullopt;

						const auto c_d = AttributeHandler::Attribute{ c_b, c_c };

						if (!temporary_extra.has_value()) {
							temporary_extra.emplace();
						}

						temporary_extra.value().insert_or_assign(c_a, c_d);
					}
				}

				const auto data_derived = AttributeHandler::DerivedAttribute{ AttributeHandler::Attribute{ b_b, b_c }, temporary_extra };

				temporary_derived.insert_or_assign(b_a, data_derived);
			}

			std::optional<std::unordered_map<RE::ActorValue, AttributeHandler::Attribute>> temporary_extra;

			if (attribute.contains("Extra")) {
				const auto& extras = toml::find<toml::array>(attribute, "Extra");

				for (const auto& extra : extras) {
					const auto d_a = Utility::GetActorValue(toml::find<std::string_view>(extra, "Attribute"));
					const auto d_b = toml::find<float>(extra, "Weight");
					const auto d_c = extra.contains("Global") ? Utility::GetForm<RE::TESGlobal>(toml::find<std::string_view>(extra, "Global")) : std::nullopt;

					const auto d_d = AttributeHandler::Attribute{ d_b, d_c };

					if (!temporary_extra.has_value()) {
						temporary_extra.emplace();
					}

					temporary_extra.value().insert_or_assign(d_a, d_d);
				}
			}

			const auto temporary_attribute = AttributeHandler::AttributeList{ AttributeHandler::Attribute{ a_b, a_c }, temporary_derived, temporary_extra };

			result.insert_or_assign(a_a, temporary_attribute);
		}
	} catch (const toml::syntax_error& error) {
		stl::report_and_error(std::format("Settings::Load ~ Failed to parse TOML! <{}>", error.what()));
	}

	INFO("Settings::Load ~ Settings loaded!");

	return result;
}

void Settings::CreateGameSettings()
{
	const auto game_settings = RE::GameSettingCollection::GetSingleton();

	if (!game_settings) {
		return;
	}

	auto sDAF_CurrentlyIncreasing = RE::malloc<RE::Setting>();

	sDAF_CurrentlyIncreasing->name = const_cast<char*>("sDAF_CurrentlyIncreasing");
	sDAF_CurrentlyIncreasing->data.s = const_cast<char*>("Currently increasing: %s");

	stl::emplace_vtable<RE::Setting>(sDAF_CurrentlyIncreasing);

	auto sDAF_Proceed = RE::malloc<RE::Setting>();

	sDAF_Proceed->name = const_cast<char*>("sDAF_Proceed");
	sDAF_Proceed->data.s = const_cast<char*>("Proceed?");

	stl::emplace_vtable<RE::Setting>(sDAF_Proceed);

	auto sDAF_AttributesBeingIncreased = RE::malloc<RE::Setting>();

	sDAF_AttributesBeingIncreased->name = const_cast<char*>("sDAF_AttributesBeingIncreased");
	sDAF_AttributesBeingIncreased->data.s = const_cast<char*>("Attributes being increased:");

	stl::emplace_vtable<RE::Setting>(sDAF_AttributesBeingIncreased);

	auto sDAF_Description = RE::malloc<RE::Setting>();

	sDAF_Description->name = const_cast<char*>("sDAF_Description");
	sDAF_Description->data.s = const_cast<char*>("...");

	stl::emplace_vtable<RE::Setting>(sDAF_Description);

	game_settings->InsertSetting(sDAF_CurrentlyIncreasing);
	game_settings->InsertSetting(sDAF_Proceed);
	game_settings->InsertSetting(sDAF_AttributesBeingIncreased);
	game_settings->InsertSetting(sDAF_Description);
}