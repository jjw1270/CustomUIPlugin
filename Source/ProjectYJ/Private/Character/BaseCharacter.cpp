// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "YJGameplayTags.h"
#include "Player/BasePlayerState.h"

ABaseCharacter::ABaseCharacter(const FObjectInitializer& _object_initializer)
	:Super(_object_initializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsPlayableChara() == false)
	{
		// AI 캐릭터 ASC 설정
		InitAbilitySystemComponent();
	}
}

void ABaseCharacter::Tick(float _delta)
{
	Super::Tick(_delta);

}

void ABaseCharacter::InitAbilitySystemComponent()
{
	auto tag_manager = UCommonUtils::GetGameInstanceSubsystem<UTagManager>(this);
	if (IsInvalid(tag_manager))
		return;

	auto ps = GetPlayerState<ABasePlayerState>();
	if (IsValid(ps))
	{
		_AbilitySystemComp = ps->GetAbilitySystemComponent();

		if (IsValid(_AbilitySystemComp))
		{
			_AbilitySystemComp->InitAbilityActorInfo(ps, this);

			for (const auto& ability_pair : _DefaultAbilityMap)
			{
				const int32 input_id = tag_manager->FindInputID(ability_pair.Key);

				_AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(ability_pair.Value, 1, input_id));
			}
		}
	}
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return _AbilitySystemComp.Get();
}

bool ABaseCharacter::IsPlayableChara() const
{
	auto controller = GetController();
	if (IsValid(controller))
	{
		return controller->IsA<APlayerController>();
	}

	return false;
}
