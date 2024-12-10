#pragma once

#include "Settings.h"

namespace Scripts
{
	inline static void Disable()
	{
		const auto VM = RE::BSScript::Internal::VirtualMachine::GetSingleton();

		if (!VM) {
			ERROR("Scripts::Disable ~ Failed to access the Virtual Machine!");

			return;
		}

		const auto REQ_CoreScripts_AttributeSystem = RE::TESForm::LookupByID<RE::TESQuest>(Settings::REQ_CoreScripts_AttributeSystem);

		if (!REQ_CoreScripts_AttributeSystem) {
			ERROR("Scripts::Disable ~ Failed to find Quest <REQ_CoreScripts_AttributeSystem>!");

			return;
		}

		REQ_CoreScripts_AttributeSystem->Stop();

		RE::BGSBaseAlias* AttributeSystem = nullptr;

		for (std::uint32_t i = 0; i < REQ_CoreScripts_AttributeSystem->aliases.size(); i++) {
			const auto& alias = REQ_CoreScripts_AttributeSystem->aliases[i];

			if (alias && alias->aliasName == "AttributeSystem") {
				AttributeSystem = alias;
			}
		}

		if (!AttributeSystem) {
			ERROR("Scripts::Disable ~ Failed to find Reference Alias: <AttributeSystem>!");

			return;
		}

		const auto handle_policy = VM->GetObjectHandlePolicy();
		auto handle = handle_policy ? handle_policy->GetHandleForObject(AttributeSystem->GetVMTypeID(), AttributeSystem) : RE::VMHandle{};

		if (!handle) {
			return;
		}

		RE::BSTSmartPointer<RE::BSScript::Object> REQ_AttributeSystem;

		if (VM->FindBoundObject(handle, "REQ_AttributeSystem", REQ_AttributeSystem)) {
			VM->UnbindObject(REQ_AttributeSystem);

			INFO("Scripts::Disable ~ Unbound REQ_AttributeSystem!");
		}
	}
}