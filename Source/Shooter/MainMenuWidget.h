// Copyright (c) 2026 Shaurya Goyal. All Rights Reserved.

#pragma once


#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"

#include "MainMenuWidget.generated.h"

/**
 * 
 */

class UButton;

UCLASS()
class SHOOTER_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* StartGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ResumeButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	UFUNCTION()
	void OnStartGameClicked();

	UFUNCTION()
	void OnResumeClicked();

	UFUNCTION()
	void OnQuitClicked();

public:

	void SetResumeVisibility(bool bVisible);
	void SetStartGameVisibility(bool bVisible);
	
	bool bRestartLevel = false;
};
