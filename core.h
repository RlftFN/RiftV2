#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>
#include "offsets.h"
#include "ue4.h"
#include "ue4_enums.h"
#include "ue4_functions.h"
#include "util.h"

namespace core
{
	GObjects* GlobalObjects;
	static void** GWorld;

	static void LoadAthena()
	{
		void* controller = UE4::GetFirstPlayerController(*GWorld);

		// -------------
		// Cheat Manager
		void* cheatManager = UE4::StaticConstructObject_Internal
		(
			GlobalObjects->FindObjectByFullName("Class /Script/Engine.CheatManager")
			, controller
			, nullptr
			, 0
			, 0
			, nullptr
			, false
			, nullptr
			, false
		);
		// -------------
		// Cheat Manager

		/*
		auto GameState = ReadPointer(*GWorld, 0x120);
		if (GameState)
		{
			std::cout << "game state exists" << std::endl;

			auto playlist = GlobalObjects->FindObjectByFullName("FortPlaylistAthena /Game/Athena/Playlists/Respawn/Playlist_Respawn_20_Lava.Playlist_Respawn_20_Lava");

			AFortGameStateAthena* MyGameState = reinterpret_cast<AFortGameStateAthena*>(GameState);
			MyGameState->CurrentPlaylistInfo.BasePlaylist = playlist;

			std::cout << "set playlist" << std::endl;
		}
		*/

		// -------------
		// Summon
		auto Summon_offset = GlobalObjects->FindObjectByFullName("Function /Script/Engine.CheatManager.Summon");

		if (Summon_offset)
		{
			struct
			{
				FString ClassName;
			} SummonParams;

			SummonParams.ClassName = L"PlayerPawn_Athena_C";

			UE4::ProcessEvent(cheatManager, Summon_offset, &SummonParams, 0);

			std::cout << "Summoned!" << std::endl;
		}
		// -------------
		// Summon


		auto pawn = GlobalObjects->FindObject("PlayerPawn_Athena_C_");



		// Possess
		// -------------
		auto Possess_offset = GlobalObjects->FindObjectByFullName("Function /Script/Engine.Controller.Possess");

		if (Possess_offset)
		{
			if (pawn)
			{
				struct
				{
					void* InPawn;
				} PossessParams;

				PossessParams.InPawn = pawn;

				UE4::ProcessEvent(controller, Possess_offset, &PossessParams, 0);

				std::cout << "Possessed!" << std::endl;
			}
		}
		// -------------
		// Possess


		// ServerReadyToStartMatch
		// -------------
		auto ServerReadyToStartMatch_offset = GlobalObjects->FindObjectByFullName("Function /Script/FortniteGame.FortPlayerController.ServerReadyToStartMatch");

		if (ServerReadyToStartMatch_offset)
		{
			UE4::ProcessEvent(controller, ServerReadyToStartMatch_offset, nullptr, 0);

			std::cout << "Controller is ready to start match!" << std::endl;
		}
		// -------------
		// ServerReadyToStartMatch


		// -------------
		// StartMatch
		auto StartMatch_offset = GlobalObjects->FindObjectByFullName("Function /Script/Engine.GameMode.StartMatch");

		if (StartMatch_offset)
		{
			auto mode = GlobalObjects->FindObject("Athena_GameMode_C_");

			if (mode)
			{
				UE4::ProcessEvent(mode, StartMatch_offset, nullptr, 0);

				std::cout << "Started match!" << std::endl;
			}
		}
		// -------------
		// StartMatch

		std::cout << "Done!" << std::endl;

		// -------------
		// Extras
		auto K2_TeleportTo_offset = GlobalObjects->FindObjectByFullName("Function /Script/Engine.Actor.K2_TeleportTo");

		if (K2_TeleportTo_offset)
		{
			struct
			{
				FVector DestLocation;
				FRotator DestRotation;
			} TeleportParams;

			TeleportParams.DestLocation = FVector{ 4024.503662, -631.651001, 3533.149902 }; // Salty Springs
			TeleportParams.DestRotation = FRotator{ 1,1,1 };

			UE4::ProcessEvent(pawn, K2_TeleportTo_offset, &TeleportParams, 0);

			std::cout << "Teleported to Salty Springs!" << std::endl;
		}


		auto DestroyAll_offset = GlobalObjects->FindObjectByFullName("Function /Script/Engine.CheatManager.DestroyAll");

		if (DestroyAll_offset)
		{
			struct
			{
				void* aClass;
			} DestroyParams;

			DestroyParams.aClass = GlobalObjects->FindObject("FortHLODSMActor");

			UE4::ProcessEvent(cheatManager, DestroyAll_offset, &DestroyParams, 0);

			std::cout << "Destroyed all instances of FortHLODSMActor!" << std::endl;
		}
		// -------------
		// Extras

		// Character
		auto PlayerState = ReadPointer(controller, 0x220);
		if (PlayerState)
		{
			auto head = GlobalObjects->FindObjectByFullName("CustomCharacterPart /Game/Characters/CharacterParts/Male/Medium/Heads/CP_Athena_Head_M_AshtonMilo.CP_Athena_Head_M_AshtonMilo");
			auto body = GlobalObjects->FindObjectByFullName("CustomCharacterPart /Game/Athena/Heroes/Meshes/Bodies/CP_Athena_Body_M_AshtonMilo.CP_Athena_Body_M_AshtonMilo");

			AFortPlayerState* MyFortPlayerState = reinterpret_cast<AFortPlayerState*>(PlayerState);

			MyFortPlayerState->CharacterData.Parts[0] = head;
			MyFortPlayerState->CharacterData.Parts[1] = body;

			auto CharacterData_offset = GlobalObjects->FindObjectByFullName("Function /Script/FortniteGame.FortPlayerState.OnRep_CharacterData");

			if (CharacterData_offset)
			{
				UE4::ProcessEvent(PlayerState, CharacterData_offset, nullptr, 0);
			}
		}

		auto obj = GlobalObjects->FindObjectByFullName("BP_NightNight_Scripting_C /Game/Athena/Maps/Athena_POI_Foundations.Athena_POI_Foundations.PersistentLevel.BP_NightNight_Scripting_2");
		auto func = GlobalObjects->FindObjectByFullName("Function /Game/Athena/Prototype/Blueprints/NightNight/BP_NightNight_Scripting.BP_NightNight_Scripting_C.LoadNightNightLevel");

		if (obj && func)
		{
			// Start NightNight
			struct
			{
				bool Condition;
			} params;
			params.Condition = true;
			UE4::ProcessEvent(obj, func, &params, 0);

			// Play Level Sequence
			UE4::ProcessEvent(GlobalObjects->FindObjectByFullName("LevelSequencePlayer /Game/Athena/Maps/Test/S10/NightNightSequenceMap.NightNightSequenceMap.PersistentLevel.NightNight.AnimationPlayer"), GlobalObjects->FindObjectByFullName("Function /Script/MovieScene.MovieSceneSequencePlayer.Play"), nullptr, 0);
		}
	};

	static void Setup()
	{
		GlobalObjects = reinterpret_cast<GObjects*>(util::GET_BASE_ADDRESS() + GOBJECTS_OFFSET);
		GWorld = reinterpret_cast<void**>(util::GET_BASE_ADDRESS() + GWORLD_OFFSET);

		LoadAthena();
	};
}