// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ButtonBase.h"
#include "Components/SizeBox.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"


TSet<FKey> UButtonBase::ClickKeyList =
{
	EKeys::LeftMouseButton
};

void UButtonBase::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(true);

	if (GetIsEnabled() == false)
	{
		TRACE_WARNING(TEXT("EButtonState::Disabled 를 사용해야 합니다!"));
	}
}

void UButtonBase::SetIsEnabled(bool _is_enabled)
{
	Super::SetIsEnabled(_is_enabled);

	if(_is_enabled)
	{
		TRACE_WARNING(TEXT("EButtonState::Disabled 를 사용해야 합니다!"));
	}
}

void UButtonBase::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (IsValid(SizeBox))
	{
		SizeBox->SetVisibility(ESlateVisibility::Visible);

		auto sizebox_slot = Cast<UCanvasPanelSlot>(SizeBox->Slot);
		if (IsValid(sizebox_slot))
		{
			sizebox_slot->SetAnchors(_UseFixedSize ? FAnchors(0.5f) : FAnchors(0.0f, 0.0f, 1.0f, 1.0f));

			sizebox_slot->SetPosition(FVector2D(0.0f));
			sizebox_slot->SetAlignment(FVector2D(0.5f));
			sizebox_slot->SetSize(FVector2D(0.0f));

			sizebox_slot->SetAutoSize(true);
		}

		if (_UseFixedSize)
		{
			SizeBox->SetWidthOverride(_FixedSize.X);
			SizeBox->SetHeightOverride(_FixedSize.Y);
		}
		else
		{
			SizeBox->ClearWidthOverride();
			SizeBox->ClearHeightOverride();
		}
	}

	UpdateButtonStyle();
}

void UButtonBase::SetButtonDisabled(bool _is_disabled)
{
	if (_is_disabled)
	{
		SetButtonState(EButtonState::Disabled);
	}
	else
	{
		if (IsHovered())
			SetButtonState(EButtonState::Hovered);
		else
			SetButtonState(EButtonState::Normal);
	}
}

void UButtonBase::ResetButtonState()
{
	if (_ButtonState == EButtonState::Disabled)
	{
		SetButtonState(EButtonState::Disabled);
	}
	else
	{
		if (IsHovered())
			SetButtonState(EButtonState::Hovered);
		else
			SetButtonState(EButtonState::Normal);
	}
}

void UButtonBase::SetButtonState(EButtonState _state)
{
	if(_ButtonState == _state)
		return;
	_ButtonState = _state;

	UpdateButtonStyle();
	OnButtonStateChanged();
}

void UButtonBase::UpdateButtonStyle()
{
	auto style_ptr = _StateStyles.Find(_ButtonState);
	if (IsValid(style_ptr))
	{
		if (IsValid(Border))
		{
			Border->SetBrush(style_ptr->Brush);
			Border->SetContentColorAndOpacity(style_ptr->ContentColor);
		}
	}
}

void UButtonBase::SetClickKeyList(const TSet<FKey>& _key_list)
{
	ClickKeyList = _key_list;
}
