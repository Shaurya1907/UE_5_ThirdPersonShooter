// Copyright (c) 2026 Shaurya Goyal. All Rights Reserved.


#include "HUDWidget.h"

void UHUDWidget::SetHealthBarPercent(float NewPercent)
{
	if(NewPercent >=0.0f && NewPercent <= 1.0f)
	{
		if(HealthBar)
		{
			HealthBar->SetPercent(NewPercent);
		}
	}
}
