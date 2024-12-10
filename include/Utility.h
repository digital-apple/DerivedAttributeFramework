#pragma once

namespace Utility
{
	inline static auto GetActorValue(const std::string_view& a_raw) -> RE::ActorValue
	{
		RE::ActorValue result = RE::ActorValue::kNone;

		const auto value_list = RE::ActorValueList::GetSingleton();

		if (value_list) {
			result = value_list->LookupActorValueByName(a_raw);
		}

		if (result == RE::ActorValue::kNone) {
			stl::report_and_error(std::format("Utility::GetActorValue ~ Invalid Actor Value: <{}>", a_raw.data()));
		}

		return result;
	}

	template <typename T>
	inline static auto GetForm(const std::string_view& a_raw) -> std::optional<T*>
	{
		if (a_raw.empty()) {
			return std::nullopt;
		}

		std::regex pattern(R"(^0x[0-9A-Fa-f]{1,6}~[a-zA-Z0-9_.-]+\.(?:esm|esp|esl)$)");

		if (!a_raw.empty() && !std::regex_match(a_raw.data(), pattern)) {
			stl::report_and_error(std::format("Utility::GetForm ~ Invalid TOML Entry: <{}>", a_raw.data()));
		}

		std::istringstream stream(a_raw.data());

		RE::FormID FormID;
		std::string ModName;

		stream >> std::hex >> FormID;
		stream.ignore(1);
		std::getline(stream, ModName);

		const auto data_manager = RE::TESDataHandler::GetSingleton();

		if (data_manager && data_manager->LookupLoadedModByName(ModName)) {
			return data_manager->LookupForm<T>(FormID, ModName);
		}

		WARN("Utility::GetForm ~ Failed to locate Form: <{:X}> in plugin: <{}>", FormID, ModName.c_str());

		return std::nullopt;
	}

	inline static auto GetFormID(const std::string_view& a_raw) -> RE::FormID
	{
		if (a_raw.empty()) {
			return RE::FormID{};
		}

		std::regex pattern(R"(^0x[0-9A-Fa-f]{1,6}~[a-zA-Z0-9_.-]+\.(?:esm|esp|esl)$)");

		if (!a_raw.empty() && !std::regex_match(a_raw.data(), pattern)) {
			stl::report_and_error(std::format("Utility::GetFormID ~ Invalid TOML Entry: <{}>", a_raw.data()));
		}

		std::istringstream stream(a_raw.data());

		RE::FormID FormID;
		std::string ModName;

		stream >> std::hex >> FormID;
		stream.ignore(1);
		std::getline(stream, ModName);

		const auto data_manager = RE::TESDataHandler::GetSingleton();

		if (data_manager && data_manager->LookupLoadedModByName(ModName)) {
			return data_manager->LookupFormID(FormID, ModName);
		}

		WARN("Utility::GetFormID ~ Failed to locate Form ID: <{:X}> in plugin: <{}>", FormID, ModName.c_str());

		return RE::FormID{};
	}

	inline static auto FormatActorValueName(RE::ActorValue a_value) -> std::string
	{
		std::string result;

		const auto value_list = RE::ActorValueList::GetSingleton();

		if (value_list) {
			const auto info = value_list->GetActorValue(a_value);

			result = info->GetName();
		}

		if (result.empty()) {
			const auto raw = std::to_string(a_value);

			std::ranges::for_each(raw, [&result](char i) {
				if (std::isupper(i) && !result.empty()) {
					result += ' ';
				}
				result += i;
			});
		}

		return result;
	}
}