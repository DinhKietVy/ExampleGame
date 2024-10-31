// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UPlayerOverlay;

UCLASS()
class SECTION6CHALLENGE_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerOverlay> PlayerOverlayClass;

	UPROPERTY()
	UPlayerOverlay* PlayerOverlay;

public:
	FORCEINLINE UPlayerOverlay* GetPlayerOverlay() { return PlayerOverlay; };
};
