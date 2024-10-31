// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOverlay.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class SECTION6CHALLENGE_API UPlayerOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetHealthBar(float Percent);

	void SetStaminaBar(float Percent);

	void SetKillCount(int Count);

private:

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* KillCount;
};
