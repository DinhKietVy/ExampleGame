// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerOverlay::SetHealthBar(float Percent)
{
	if (HealthBar == nullptr) return;

	HealthBar->SetPercent(Percent);
}

void UPlayerOverlay::SetStaminaBar(float Percent)
{
	if (StaminaBar == nullptr) return;

	StaminaBar->SetPercent(Percent);
}

void UPlayerOverlay::SetKillCount(int32 Count)
{
	if (KillCount == nullptr) return;

	KillCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), Count)));
}
