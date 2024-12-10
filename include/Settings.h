#pragma once

#include "AttributeHandler.h"

class Settings
{
public:
	static auto Load() -> std::unordered_map<RE::ActorValue, AttributeHandler::AttributeList>;

	static void CreateGameSettings();

	inline static RE::FormID REQ_CoreScripts_AttributeSystem = { 0x0 };

	inline static bool LineBreak = { false };

	inline static std::uint32_t CurrentValueColor = { 0xFF0000 };
	inline static std::uint32_t UpdatedValueColor = { 0x00FF00 };
	inline static std::uint32_t ButtonColor = { 0xFFFFAA };

private:
	inline static std::filesystem::path TOML_PATH = { L"Data\\SKSE\\Plugins\\DerivedAttributeFramework.toml" };
};