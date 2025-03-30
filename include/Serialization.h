#pragma once

class Serialization
{
public:
	enum : std::uint32_t
	{
		kModID = 'DAFI',
		kQueue = 'DAFQ',
		kVersion = 101
	};

	static auto GetSingleton() -> Serialization*;

	static void OnGameLoaded(SKSE::SerializationInterface* a_interface);
	static void OnGameSaved(SKSE::SerializationInterface* a_interface);

	static void Queue(RE::ActorValue a_value);
private:
	Serialization() = default;
	Serialization(const Serialization&) = delete;
	Serialization(Serialization&&) = delete;

	~Serialization() = default;

	Serialization& operator=(const Serialization&) = delete;
	Serialization& operator=(Serialization&&) = delete;

	std::set<RE::ActorValue> queue;
};