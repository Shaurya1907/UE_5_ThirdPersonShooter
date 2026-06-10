// Copyright (c) 2026 Shaurya Goyal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/ProgressBar.h"

#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UProgressBar* HealthBar;
	
	void SetHealthBarPercent(float Percent);
};
