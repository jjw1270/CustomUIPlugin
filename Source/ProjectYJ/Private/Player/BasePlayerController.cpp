// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BasePlayerController.h"
#include "Player/BasePlayerState.h"
#include "AbilitySystemComponent.h"

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();
	// ConsoleCommand(TEXT("showdebug abilitysystem"));
}

UAbilitySystemComponent* ABasePlayerController::GetAbilitySystemComponent() const
{
	auto ps = GetPlayerState<ABasePlayerState>();
	if (IsValid(ps))
	{
		return ps->GetAbilitySystemComponent();
	}

	return nullptr;
}
