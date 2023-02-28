// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadlyInstance.h"
#include "Containers/Array.h"
#include "MenuManager.h"
#include "PlayerManager.h"
#include "Kismet/Gameplaystatics.h"


void UDeadlyInstance::MenuSetup()
{   
    SaveGameSlotName = "DeadlySlot";
    LoadGame();    
    menuActor = UGameplayStatics::GetActorOfClass(GetWorld(), AMenuManager::StaticClass());
    
    if(menuActor)
    { 
        menuManager = Cast<AMenuManager>(menuActor);
        menuManager->CharacterCreated.AddDynamic(this, &UDeadlyInstance::SaveCharacter);
        menuManager->getHex();

        if (SaveGameObject->alivePlayers.Num() > 0)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("m_maxHealth: %f"), 
            SaveGameObject->alivePlayers[0].maxHealth));

            menuManager->spawnAlivePlayers(SaveGameObject->alivePlayers);
        }
    }

    //playerManager = UGameplayStatics::GetActorOfClass(GetWorld(), AMenuManager::StaticClass());
    
}

void UDeadlyInstance::DungeonSetup()
{
    SaveGameSlotName = "DeadlySlot";
    LoadGame();
    playerActor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerManager::StaticClass());

    if (playerActor)
    {
        playerManager = Cast<APlayerManager>(playerActor);
        if (SaveGameObject->alivePlayers.Num() > 0)
        {
            playerManager->handlePlayersToSpawn(SaveGameObject->alivePlayers);
            playerManager->spawnEnemies(1,4);
        }
    }
}

void UDeadlyInstance::LoadGame()
{
    // Try to load a saved game file (with name: <SaveGameSlotName>.sav) if exists
    USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot(SaveGameSlotName, 0);
    SaveGameObject = Cast<UDeadlySave>(LoadedGame);
        
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Trying to load a saved game."));

    // If file does not exist try create a new one
    if (!SaveGameObject)
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No saved games found. Trying to save a new one."));

        // Instantiate a new SaveGame object
        SaveGameObject = Cast<UDeadlySave>(UGameplayStatics::CreateSaveGameObject(UDeadlySave::StaticClass()));

        // Call SaveGameToSlot to serialize and save our SaveGameObject with name: <SaveGameSlotName>.sav
        bool IsSaved = UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveGameSlotName, 0);

        LogIfGameWasSavedOrNot(IsSaved);
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Saved game found. Loaded."));
    }
}

void UDeadlyInstance::SaveGame()
{
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Saving game..."));

    // Call SaveGameToSlot to serialize and save our SaveGameObject with name: <SaveGameSlotName>.sav
    bool IsSaved = UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveGameSlotName, 0);

    LogIfGameWasSavedOrNot(IsSaved);
}

void UDeadlyInstance::SaveCharacter(APlayerCharacter* character)
{ 
    SaveGameObject->alivePlayers.Emplace(character->getStats());

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("m_maxHealth: %f"),
        SaveGameObject->alivePlayers[0].maxHealth));

    bool IsSaved = UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveGameSlotName, 0);

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("m_maxHealth: %f"),
        SaveGameObject->alivePlayers[0].maxHealth));

    LogIfGameWasSavedOrNot(IsSaved);
}

void UDeadlyInstance::LogIfGameWasSavedOrNot(const bool IsSaved)
{
    if (IsSaved)
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Game saved."));
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Cannot save the game."));
    }
}