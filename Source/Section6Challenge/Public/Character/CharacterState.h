#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ESC_EquippedOneHanedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon")
};

UENUM()
enum class EActionState : uint8
{
	ECS_Unoccupied,
	ESC_Attack
};