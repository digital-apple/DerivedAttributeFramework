#pragma once

class LevelUpHandler
{
public:
	class ConfirmLevelUpAttributeCallbackEx : public RE::IMessageBoxCallback
	{
	public:
		ConfirmLevelUpAttributeCallbackEx(RE::LevelUpMenu* a_menu, RE::ActorValue a_attribute, std::unordered_map<std::size_t, RE::ActorValue>& a_map, std::size_t a_cancel)
		{
			menu = a_menu;
			attribute = a_attribute;
			derived_map = a_map;
			cancel_index = a_cancel;
		}

		~ConfirmLevelUpAttributeCallbackEx() override = default;

		void Run(RE::IMessageBoxCallback::Message a_message) override;
	private:
		RE::LevelUpMenu* menu;
		RE::ActorValue attribute;
		std::unordered_map<std::size_t, RE::ActorValue> derived_map;
		std::size_t cancel_index;
	};

	static auto GetSingleton() -> LevelUpHandler*;

	static void ConstructMessage(RE::LevelUpMenu* a_menu, const RE::ActorValue a_attribute);

	static bool Locked();
	static void Lock(bool a_lock);
private:
	LevelUpHandler() = default;
	LevelUpHandler(const LevelUpHandler&) = delete;
	LevelUpHandler(LevelUpHandler&&) = delete;

	~LevelUpHandler() = default;

	LevelUpHandler& operator=(const LevelUpHandler&) = delete;
	LevelUpHandler& operator=(LevelUpHandler&&) = delete;

	std::atomic<bool> locked;
};