// Copyright (c) 2026 Shaurya Goyal. All Rights Reserved.


#include "MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "ShooterPlayerController.h"
#include "GameFramework/PlayerController.h"

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (StartGameButton)
    {
        StartGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartGameClicked);
    }

    if (ResumeButton)
    {
        ResumeButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnResumeClicked);
    }

    if (QuitButton)
    {
        QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClicked);
    }
}

void UMainMenuWidget::OnStartGameClicked()
{
    if (bRestartLevel)
    {
        UGameplayStatics::OpenLevel(GetWorld(), FName(*UGameplayStatics::GetCurrentLevelName(GetWorld())));
        return;
    }

    APlayerController* PC = GetOwningPlayer();

    if (PC)
    {
        PC->SetShowMouseCursor(false);
        PC->SetInputMode(FInputModeGameOnly());
    }

    RemoveFromParent();
}

void UMainMenuWidget::OnResumeClicked()
{
    AShooterPlayerController* PC = Cast<AShooterPlayerController>(GetOwningPlayer());

    if (PC)
    {
        UGameplayStatics::SetGamePaused(GetWorld(), false);
        PC->HidePauseMenu();
    }
}

void UMainMenuWidget::OnQuitClicked()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UMainMenuWidget::SetResumeVisibility(bool bVisible)
{
    if (ResumeButton)
    {
        ResumeButton->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    }
}

void UMainMenuWidget::SetStartGameVisibility(bool bVisible)
{
    if (StartGameButton)
    {
        StartGameButton->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    }
}