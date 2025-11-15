// Fill out your copyright notice in the Description page of Project Settings.


#include "TabButton.h"

void UTabButton::SynchronizeProperties()
{
	_ButtonType = EButtonType::Toggle;
	_UseSubClick = false;
	_SubClickSound = nullptr;
	
	Super::SynchronizeProperties();
}

void UTabButton::InitWidget(FName _tab_id, const FTabButtonConfig& _config)
{
	_TabID = _tab_id;

	_ButtonStyle = _config.ButtonStyle;
	_HoverSound = _config.HoverSound;
	_ClickSound = _config.ClickSound;

	UpdateButtonStyle();
}
