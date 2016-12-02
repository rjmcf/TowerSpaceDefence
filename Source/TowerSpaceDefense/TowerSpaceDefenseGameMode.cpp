// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "TowerSpaceDefense.h"
#include "TowerSpaceDefenseGameMode.h"
#include "TowerSpaceDefensePawn.h"

ATowerSpaceDefenseGameMode::ATowerSpaceDefenseGameMode()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = ATowerSpaceDefensePawn::StaticClass();
}
