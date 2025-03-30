#include "AttributeHandler.h"
#include "Hooks.h"
#include "Scripts.h"
#include "Serialization.h"
#include "Settings.h"

void InitializeLogger()
{
    auto path = SKSE::log::log_directory();
   
    if (!path) { return; }

    const auto plugin = SKSE::PluginDeclaration::GetSingleton();
    *path /= std::format("{}.log", plugin->GetName());

    std::vector<spdlog::sink_ptr> sinks{
        std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true),
        std::make_shared<spdlog::sinks::msvc_sink_mt>()
    };

    auto logger = std::make_shared<spdlog::logger>("global", sinks.begin(), sinks.end());
   
    logger->set_level(spdlog::level::info);
    logger->flush_on(spdlog::level::info);

    spdlog::set_default_logger(std::move(logger));
    spdlog::set_pattern("[%^%L%$] %v");
}

void HandleMessage(SKSE::MessagingInterface::Message* a_message)
{
    switch (a_message->type) {
    case SKSE::MessagingInterface::kInputLoaded:
        {
            Settings::CreateGameSettings();
        }
        break;
    case SKSE::MessagingInterface::kDataLoaded:
        {
            Hooks::Install();
            AttributeHandler::GetSingleton()->Load();
        }
        break;
    case SKSE::MessagingInterface::kNewGame:
    case SKSE::MessagingInterface::kPostLoadGame:
        {
            Scripts::Disable();
        }
        break;
    }
}

void InitializeSerialization()
{
    const auto serialization_interface = SKSE::GetSerializationInterface();

    serialization_interface->SetUniqueID(Serialization::kModID);
    serialization_interface->SetSaveCallback(Serialization::OnGameSaved);
    serialization_interface->SetLoadCallback(Serialization::OnGameLoaded);
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
    InitializeLogger();

    SKSE::Init(a_skse);

    SKSE::AllocTrampoline(14 * 3);

    const auto messaging_interface = SKSE::GetMessagingInterface();

    if (!messaging_interface) { stl::report_and_fail("Failed to communicate with the messaging interface!"); }

    messaging_interface->RegisterListener(HandleMessage);

    InitializeSerialization();

    return true;
}
