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
        menuManager->m_ID = SaveGameObject->characterID;
        menuManager->CharacterCreated.AddDynamic(this, &UDeadlyInstance::SaveCharacter);
        menuManager->getHex();

        if (SaveGameObject->alivePlayers.Num() > 0)
        {
            menuManager->spawnAlivePlayers(SaveGameObject->alivePlayers);
        }

        unitInfoWidget = nullptr;
        
        if (unitInfoWidgetClass)
        {
            if (!unitInfoWidget)
            {
                unitInfoWidget = CreateWidget<UUnitInfoWidget>(this, unitInfoWidgetClass);
                unitInfoWidget->AddToViewport();
                menuManager->unitInfoWidget = unitInfoWidget;
            }
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
        playerManager->PlayerDeath.AddDynamic(this, &UDeadlyInstance::DeleteCharacter);
        playerManager->GameOver.AddDynamic(this, &UDeadlyInstance::setGameOver);
        
        if (SaveGameObject->alivePlayers.Num() > 0)
        {
            spawnWidgets();
            playerManager->turnActionWidgetSetup(turnActionWidget);
            playerManager->turnOrderWidgetSetup(turnOrderWidget);
            playerManager->unitInfoWidgetSetup(unitInfoWidget);
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
    int tempID = SaveGameObject->characterID;
    menuManager->m_ID = ++tempID;
    SaveGameObject->characterID = tempID;

    SaveGameObject->alivePlayers.Emplace(character->getStats());

    bool IsSaved = UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveGameSlotName, 0);

    LogIfGameWasSavedOrNot(IsSaved);
}

void UDeadlyInstance::DeleteCharacter(int ID)
{
    for(int i{0};i<SaveGameObject->alivePlayers.Num();++i)
    {
        if (SaveGameObject->alivePlayers[i].ID == ID)
        {
            SaveGameObject->alivePlayers.RemoveAt(i);
        }
    }

    SaveGame();
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

void UDeadlyInstance::spawnWidgets()
{
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Spawning that shit"));

    turnActionWidget = nullptr;
    turnOrderWidget = nullptr;
    endScreenWidget = nullptr;
    unitInfoWidget = nullptr;


    if (turnActionWidgetClass)
    {
        if (!turnActionWidget)
        {
            turnActionWidget = CreateWidget<UTurnActionWidget>(this, turnActionWidgetClass);
            turnActionWidget->AddToViewport();
        }
    }

    if (turnOrderWidgetClass)
    {
        if (!turnOrderWidget)
        {
            turnOrderWidget = CreateWidget<UTurnOrderWidget>(this, turnOrderWidgetClass);
            turnOrderWidget->AddToViewport();
        }
    }

    if (unitInfoWidgetClass)
    {
        if (!unitInfoWidget)
        {
            unitInfoWidget = CreateWidget<UUnitInfoWidget>(this, unitInfoWidgetClass);
            unitInfoWidget->AddToViewport();
        }
    }

}

bool UDeadlyInstance::characterExists()
{
    return menuManager->characterExists(); 
}

void UDeadlyInstance::setGameOver(bool win)
{    
    if (endScreenWidgetClass)
    {
        if (!endScreenWidget)
        {
            endScreenWidget = CreateWidget<UEndScreenWidget>(this, endScreenWidgetClass);
            endScreenWidget->setText(win);
            endScreenWidget->AddToViewport();
            endScreenWidget->ToMenu.AddDynamic(this, &UDeadlyInstance::goToMenu);
        }
    }
}

void UDeadlyInstance::goToMenu()
{
    UGameplayStatics::OpenLevel(this, LevelToLoad);
}
