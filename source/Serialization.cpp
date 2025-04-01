#include "Serialization.h"

auto Serialization::GetSingleton() -> Serialization*
{
	static Serialization singleton;

	return std::addressof(singleton);
}

void Serialization::OnGameLoaded(SKSE::SerializationInterface* a_interface)
{
	const auto player = RE::PlayerCharacter::GetSingleton();
	const auto av_storage = player ? player->AsActorValueOwner() : nullptr;

	if (!av_storage) {
		ERROR("Serialization::OnGameLoaded ~ Failed to obtain the player's actor value storage! Aborting deserialization....");

		return;
	}

	std::uint32_t type, version, length;

	while (a_interface->GetNextRecordInfo(type, version, length)) {
		if (version != kVersion) {
			ERROR("Serialization::OnGameLoaded ~ Version mismatch! Current version: <{}> Expected version: <{}>! Aborting deserialization...", version, std::to_underlying(kVersion));

			continue;
		}

		if (type != kQueue) {
			continue;
		}

		auto& queue = GetSingleton()->queue;

		std::size_t queue_size;
		a_interface->ReadRecordData(&queue_size, sizeof(queue_size));

		for (std::size_t index = 0; index < queue_size; index++) {
			RE::ActorValue av;
			float value;

			a_interface->ReadRecordData(&av, sizeof(av));
			a_interface->ReadRecordData(&value, sizeof(value));

			av_storage->SetBaseActorValue(av, value);

			queue.insert(av);

			INFO("Serialization::OnGameLoaded ~ Successfully deserialized actor value: <{}> with a value of: <{}>", av, value);
		}

		break;
	}
}

void Serialization::OnGameSaved(SKSE::SerializationInterface* a_interface)
{
	if (!a_interface->OpenRecord(kQueue, kVersion)) {
		ERROR("Serialization::OnGameSaved ~ Failed to open the queue record! Aborting serialization...");

		return;
	}

	const auto& queue = GetSingleton()->queue;
	const auto queue_size = queue.size();

	if (!a_interface->WriteRecordData(&queue_size, sizeof(queue_size))) {
		ERROR("Serialization::OnGameSaved ~ Failed to serialize the number of elements inside the queue!");

		return;
	}

	if (queue.empty()) {
		ERROR("Serialization::OnGameSaved ~ The queue is currently empty! Aborting serialization...");

		return;
	}

	const auto player = RE::PlayerCharacter::GetSingleton();
	const auto av_storage = player ? player->AsActorValueOwner() : nullptr;

	if (!av_storage) {
		ERROR("Serialization::OnGameSaved ~ Failed to obtain the player's actor value storage! Aborting serialization...");

		return;
	}

	for (const auto& av : queue) {
		const auto value = av_storage->GetBaseActorValue(av);

		if (!a_interface->WriteRecordData(&av, sizeof(av))) {
			ERROR("Serialization::OnGameSaved ~ Failed to serialize actor value: <{}> with a value of: <{}>!", av, value);

			return;
		}

		if (!a_interface->WriteRecordData(&value, sizeof(value))) {
			ERROR("Serialization::OnGameSaved ~ Failed to serialize actor value: <{}> with a value of: <{}>!", av, value);

			return;
		}

		INFO("Serialization::OnGameSaved ~ Successfully serialized actor value: <{}> with a value of: <{}>", av, value);
	}
}

void Serialization::Queue(RE::ActorValue a_value)
{
	if (a_value == RE::ActorValue::kNone) {
		return;
	}

	GetSingleton()->queue.insert(a_value);

	INFO("Serialization::Queue ~ Pushed actor value: <{}> to the actor value queue.", a_value);
}