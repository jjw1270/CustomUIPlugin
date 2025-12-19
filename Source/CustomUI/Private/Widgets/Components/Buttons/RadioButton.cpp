// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/Buttons/RadioButton.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"


void URadioButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	OnSelectChanged();
}

void URadioButton::NativeOnMouseEnter(const FGeometry& _geo, const FPointerEvent& _mouse_event)
{
	Super::NativeOnMouseEnter(_geo, _mouse_event);

	if (_ButtonState == EButtonState::Disabled)
	{
		SetButtonState(EButtonState::Disabled);
	}
	else if (_IsSelected == false && _ButtonState != EButtonState::Hovered)
	{
		SetButtonState(EButtonState::Hovered);
		PlaySound(_HoverSound);
	}
}

void URadioButton::NativeOnMouseLeave(const FPointerEvent& _mouse_event)
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

FReply URadioButton::NativeOnMouseButtonDown(const FGeometry& _geo, const FPointerEvent& _mouse_event)
{
	Super::NativeOnMouseButtonDown(_geo, _mouse_event);

	if (_ButtonState == EButtonState::Disabled)
	{
		SetButtonState(EButtonState::Disabled);
	}
	else if (_IsSelected == false && _ButtonState == EButtonState::Hovered)
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

FReply URadioButton::NativeOnMouseButtonUp(const FGeometry& _geo, const FPointerEvent& _mouse_event)
{
	Super::NativeOnMouseButtonUp(_geo, _mouse_event);

	if (_ButtonState == EButtonState::Disabled)
	{
		SetButtonState(EButtonState::Disabled);
	}
	else if (_IsSelected == false && _ButtonState == EButtonState::Pressed)
	{
		if (ClickKeyList.Contains(_mouse_event.GetEffectingButton()))
		{
			SetIsSelected(true);

			if (_OnButtonClicked.IsBound())
				_OnButtonClicked.Broadcast(this);

			SetButtonState(EButtonState::Normal);
		}
	}

	return GetReply();
}

void URadioButton::SetIsSelected(bool _is_selected)
{
	if (_IsSelected == _is_selected)
		return;
	_IsSelected = _is_selected;

	OnSelectChanged();
	UpdateButtonStyle();
}

void URadioButton::OnSelectChanged_Implementation()
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

void URadioButton::UpdateButtonStyle()
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
