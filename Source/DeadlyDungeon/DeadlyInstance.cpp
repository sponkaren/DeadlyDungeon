// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadlyInstance.h"
#include "Containers/Array.h"
#include "MenuManager.h"
#include "PlayerManager.h"
#include "Kismet/Gameplaystatics.h"


void UDeadlyInstance::Setup()
{   
    SaveGameSlotName = "DeadlySlot";
    LoadGame();    
    menuActor = UGameplayStatics::GetActorOfClass(GetWorld(), AMenuManager::StaticClass());
    
    if(menuActor)
    { 
        menuManager = Cast<AMenuManager>(menuActor);
        menuManager->CharacterCreated.AddDynamic(this, &UDeadlyInstance::SaveCharacter);
        menuManager->getHex();

        if (SaveGameObject->One == true)
        {
            menuManager->spawnAlivePlayer(SaveGameObject->characterOne, 0);
        }
        if (SaveGameObject->Two == true)
        {
            menuManager->spawnAlivePlayer(SaveGameObject->characterTwo, 1);
        }
        if (SaveGameObject->Three == true)
        {
            menuManager->spawnAlivePlayer(SaveGameObject->characterThree, 2);
        }
        if (SaveGameObject->Four == true)
        {
            menuManager->spawnAlivePlayer(SaveGameObject->characterFour, 3);
        }
    }

    //playerManager = UGameplayStatics::GetActorOfClass(GetWorld(), AMenuManager::StaticClass());
    
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

    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Save that shit!"));
 
    //When arrays don't cooperate..
    if (SaveGameObject->One == false)
    {
        SaveGameObject->characterOne = character->getStats();
        SaveGameObject->One = true;
    }
    else if (SaveGameObject->Two == false)
    {
        SaveGameObject->characterTwo = character->getStats();
        SaveGameObject->Two = true;
    }
    else if (SaveGameObject->Three == false)
    {
        SaveGameObject->characterThree = character->getStats();
        SaveGameObject->Three = true;
    }
    else if (SaveGameObject->Four == false)
    {
        SaveGameObject->characterFour = character->getStats();
        SaveGameObject->Four = true;
    }

    bool IsSaved = UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveGameSlotName, 0);

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