// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define DRAW_LINE(StartLocation, EndLocation,Color) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation,Color, true, -1.f, 0, 1.f);