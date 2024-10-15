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
	EAS_Unoccupied,
	EAC_Attack
};

UENUM()
enum class EEnemyState : uint8
{
	EES_Patrol UMETA(DisplayName = "Patrol"),
	EES_Waiting UMETA(DisplayName = "Waiting"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EEC_Attacking UMETA(DisplayName = "Attacking"),
	EEC_Beaten,
};