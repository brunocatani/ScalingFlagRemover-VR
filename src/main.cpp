#include "LogPath.h"

#include <iostream>
#include <sstream>
#include <string>

namespace
{
	void RemoveScalingFlags(std::string_view a_when)
	{
		auto dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) {
			logger::warn("Unable to access DataHandler! Nothing will be changed...");
			return;
		}
		std::uint32_t changedNPCs{ 0 };
		for (auto currentNPC : dataHandler->GetFormArray<RE::TESNPC>()) {
			if (currentNPC && currentNPC->HasPCLevelMult()) {
				currentNPC->actorData.actorBaseFlags.reset(RE::ACTOR_BASE_DATA::Flag::kPCLevelMult);
				if (currentNPC->actorData.calcLevelMin < currentNPC->actorData.calcLevelMax) {
					currentNPC->actorData.level = currentNPC->actorData.calcLevelMax;
				} else {
					currentNPC->actorData.level = currentNPC->actorData.calcLevelMin;
				}
				++changedNPCs;
			}
		}
		logger::info("Removed PC Level Mult from {} NPC records ({})", changedNPCs, a_when);
	}

	// Diagnostic for the reload revert: which records carry the flag again once the save is read.
	void CountScalingFlags()
	{
		auto dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) {
			return;
		}
		std::uint32_t flagged{ 0 };
		std::string sample;
		for (auto currentNPC : dataHandler->GetFormArray<RE::TESNPC>()) {
			if (currentNPC && currentNPC->HasPCLevelMult()) {
				if (++flagged <= 12) {
					sample += fmt::format(" {:08X}", currentNPC->formID);
				}
			}
		}
		logger::info("After the save load {} NPC records have PC Level Mult:{}", flagged, sample);
	}
}

void ListenerThing(F4SE::MessagingInterface::Message* a_thing)
{
	switch (a_thing->type) {
	case F4SE::MessagingInterface::kGameDataReady:
		if (a_thing->data) {
			RemoveScalingFlags("game data ready");
		}
		break;
	case F4SE::MessagingInterface::kPreLoadGame:
		// A save loaded after the game returned to the main menu (a death reload, or quitting to the
		// menu) finds NPC records reverted to their plugin data, measured on the bench: Piper scaled
		// with the player again. Game data is ready only once per process, so this runs before each load.
		RemoveScalingFlags("before save load");
		break;
	case F4SE::MessagingInterface::kPostLoadGame:
		CountScalingFlags();
		break;
	default:
		break;
	}
}

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Query(const F4SE::QueryInterface* a_f4se, F4SE::PluginInfo* a_info)
{
#ifndef NDEBUG
	auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
	auto path = LogPath::GetFallout4VRLogDirectory();
	if (!path) {
		return false;
	}

	*path /= fmt::format(FMT_STRING("{}.log"), Version::PROJECT);
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

#ifndef NDEBUG
	log->set_level(spdlog::level::trace);
#else
	log->set_level(spdlog::level::info);
	log->flush_on(spdlog::level::info);
#endif

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("%g(%#): [%^%l%$] %v"s);

	logger::info(FMT_STRING("{} v{}"), Version::PROJECT, Version::NAME);

	a_info->infoVersion = F4SE::PluginInfo::kVersion;
	a_info->name = "GLXRM_ScalingFlagRemover";
	a_info->version = 1;

	if (a_f4se->IsEditor()) {
		logger::critical("loaded in editor");
		return false;
	}

	// RuntimeVersion() is F4SEVR's compatibility value, not the executable's version.
	if (!REL::Module::IsVR() || REL::Module::get().version() != F4SE::RUNTIME_VR_1_2_72) {
		logger::critical("Fallout4VR.exe 1.2.72.0 is required");
		return false;
	}
	logger::info("Query passed: executable {}; F4SE compatibility runtime {}",
		REL::Module::get().version().string(), a_f4se->RuntimeVersion().string());

	return true;
}

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Load(const F4SE::LoadInterface* a_f4se)
{
	F4SE::Init(a_f4se, false);  // false = don't reinitialize logger, keep the one from Query

	F4SE::GetMessagingInterface()->RegisterListener(ListenerThing);

	return true;
}
