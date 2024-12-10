#pragma once

class AttributeHandler
{
public:
	struct Attribute
	{
		float weight;
		std::optional<RE::TESGlobal*> global;
	};

	struct DerivedAttribute
	{
		Attribute attribute;
		std::optional<std::unordered_map<RE::ActorValue, Attribute>> extra;
	};

	struct AttributeList
	{
		Attribute attribute;
		std::unordered_map<RE::ActorValue, DerivedAttribute> derived;
		std::optional<std::unordered_map<RE::ActorValue, Attribute>> extra;
	};

	class AttributeHandlerCallback : public RE::IMessageBoxCallback
	{
	public:
		AttributeHandlerCallback(RE::LevelUpMenu* a_menu, const RE::ActorValue a_attribute, const RE::ActorValue a_derived)
		{
			menu = a_menu;
			attribute = a_attribute;
			derived = a_derived;
		}

		~AttributeHandlerCallback() override = default;

		void Run(RE::IMessageBoxCallback::Message a_message) override;
	private:
		RE::LevelUpMenu* menu;
		RE::ActorValue attribute;
		RE::ActorValue derived;
	};

	static auto GetSingleton() -> AttributeHandler*;

	static void ConstructMessage(RE::LevelUpMenu* a_menu, const RE::ActorValue a_attribute, const RE::ActorValue a_derived);

	auto GetAttributeList(const RE::ActorValue a_attribute, const RE::ActorValue a_derived) const->std::unordered_map<RE::ActorValue, float>;
	auto GetAttributeStringList(const RE::ActorValue a_attribute, const RE::ActorValue a_derived) const->std::vector<std::tuple<std::string, float, float>>;
	auto GetDerivedAttributes(const RE::ActorValue a_attribute) const -> std::vector<RE::ActorValue>;
	void Load();
private:
	AttributeHandler() = default;
	AttributeHandler(const AttributeHandler&) = delete;
	AttributeHandler(AttributeHandler&&) = delete;

	~AttributeHandler() = default;

	AttributeHandler& operator=(const AttributeHandler&) = delete;
	AttributeHandler& operator=(AttributeHandler&&) = delete;

	std::unordered_map<RE::ActorValue, AttributeList> attributes;
};