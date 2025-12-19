// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/Buttons/ToggleButton.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"


void UToggleButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	OnSelectChanged();
}

void UToggleButton::NativeOnMouseEnter(const FGeometry& _geo, const FPointerEvent& _mouse_event)
{
	Super::NativeOnMouseEnter(_geo, _mouse_event);

	if (_ButtonState == EButtonState::Disabled)
	{
		SetButtonState(EButtonState::Disabled);
	}
	else if (_ButtonState != EButtonState::Hovered)
	{
		SetButtonState(EButtonState::Hovered);
		PlaySound(_HoverSound);
	}
}

void UToggleButton::NativeOnMouseLeave(const FPointerEvent& _mouse_event)
{
	Super::NativeOnMouseLeave(_mouse_event);

	if (_ButtonState == EButtonState::Disabled)
	{
		SetButtonState(EButtonState::Disabled);
	}
	else
	{
		ResetButtonState();
	}
}

FReply UToggleButton::NativeOnMouseButtonDown(const FGeometry& _geo, const FPointerEvent& _mouse_event)
{
	Super::NativeOnMouseButtonDown(_geo, _mouse_event);

	if (_ButtonState == EButtonState::Disabled)
	{
		SetButtonState(EButtonState::Disabled);
	}
	else if (_ButtonState == EButtonState::Hovered)
	{
		if (ClickKeyList.Contains(_mouse_event.GetEffectingButton()))
		{
			if (_ButtonState != EButtonState::Pressed)
			{
				SetButtonState(EButtonState::Pressed);
				PlaySound(_ClickSound);
			}
		}
	}

	return GetReply();
}

FReply UToggleButton::NativeOnMouseButtonUp(const FGeometry& _geo, const FPointerEvent& _mouse_event)
{
	Super::NativeOnMouseButtonUp(_geo, _mouse_event);

	if (_ButtonState == EButtonState::Disabled)
	{
		SetButtonState(EButtonState::Disabled);
	}
	else if (_ButtonState == EButtonState::Pressed)
	{
		if (ClickKeyList.Contains(_mouse_event.GetEffectingButton()))
		{
			ToggleSelected();

			if (_OnButtonClicked.IsBound())
				_OnButtonClicked.Broadcast(this, _IsSelected);

			ResetButtonState();
		}
	}

	return GetReply();
}

void UToggleButton::SetIsSelected(bool _is_selected)
{
	if (_IsSelected == _is_selected)
		return;
	_IsSelected = _is_selected;

	OnSelectChanged();
	UpdateButtonStyle();
}

bool UToggleButton::ToggleSelected()
{
	SetIsSelected(!_IsSelected);
	return _IsSelected;
}

void UToggleButton::OnSelectChanged_Implementation()
{
	if (_IsSelected)
	{
		SetText(_SelectedText);
	}
	else
	{
		SetText(_UnselectedText);
	}
}

void UToggleButton::UpdateButtonStyle()
{
	if (_IsSelected)
	{
		auto style_ptr = _SelectedStateStyles.Find(_ButtonState);
		if (IsValid(style_ptr))
		{
			if (IsValid(Border))
			{
				Border->SetBrush(style_ptr->Brush);
				Border->SetContentColorAndOpacity(style_ptr->ContentColor);
			}

			if (IsValid(TextBlock))
			{
				TextBlock->SetFont(style_ptr->Font);
			}
		}
	}
	else
	{
		Super::UpdateButtonStyle();
	}
}
