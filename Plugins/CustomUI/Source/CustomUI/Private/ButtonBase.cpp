// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonBase.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/CanvasPanelSlot.h"

TSet<FKey> UButtonBase::ClickKeyList =
{
	EKeys::LeftMouseButton
};

TSet<FKey> UButtonBase::SubClickKeyList =
{
	EKeys::RightMouseButton
};

void UButtonBase::NativeConstruct()
{
	Super::NativeConstruct();

	SetButtonState(EButtonState::Normal);
}

void UButtonBase::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (_ButtonType == EButtonType::Toggle)
	{
		// 토글형 버튼은 SubClick 개념이 없음.
		_UseSubClick = false;
		_SubClickSound = nullptr;

		if (_OnButtonDoubleClicked.IsBound())
		{
			TRACE_ERROR(TEXT("토글형 버튼은 DoubleClick 개념이 없습니다."));
			_OnButtonDoubleClicked.Clear();
		}
	}

	if (IsValid(SizeBox))
	{
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

void UButtonBase::NativeOnMouseEnter(const FGeometry& _geo, const FPointerEvent& _mouse_event)
{
	Super::NativeOnMouseEnter(_geo, _mouse_event);

	if (_ButtonType == EButtonType::Toggle && _ButtonState == EButtonState::Pressed)
		return;

	if(_ButtonState != EButtonState::Hovered)
		PlaySound(_HoverSound);

	SetButtonState(EButtonState::Hovered);
}

void UButtonBase::NativeOnMouseLeave(const FPointerEvent& _mouse_event)
{
	Super::NativeOnMouseLeave(_mouse_event);

	if (_ButtonType == EButtonType::Toggle && _ButtonState == EButtonState::Pressed)
		return;

	SetButtonState(EButtonState::Normal);
}

FReply UButtonBase::NativeOnMouseButtonDown(const FGeometry& _geo, const FPointerEvent& _mouse_event)
{
	FReply reply = Super::NativeOnMouseButtonDown(_geo, _mouse_event);

	if (_ButtonType == EButtonType::Toggle)
	{
		if (ClickKeyList.Contains(_mouse_event.GetEffectingButton()))
		{
			if (_ButtonState != EButtonState::Pressed)
				PlaySound(_ClickSound);

			SetButtonState(EButtonState::Pressed);

			if (_OnButtonClicked.IsBound())
				_OnButtonClicked.Broadcast(this);
		}
	}
	else
	{
		if (ClickKeyList.Contains(_mouse_event.GetEffectingButton()))
		{
			if (_ButtonState != EButtonState::Pressed)
				PlaySound(_ClickSound);

			SetButtonState(EButtonState::Pressed);
		}
		
		if (_UseSubClick)
		{
			if (SubClickKeyList.Contains(_mouse_event.GetEffectingButton()))
			{
				if (_ButtonState != EButtonState::Pressed)
					PlaySound(_SubClickSound);

				SetButtonState(EButtonState::Pressed);
			}
		}
	}

	return MoveTemp(reply);
}

FReply UButtonBase::NativeOnMouseButtonUp(const FGeometry& _geo, const FPointerEvent& _mouse_event)
{
	FReply reply = Super::NativeOnMouseButtonUp(_geo, _mouse_event);

	if(_ButtonType == EButtonType::Click)
	{
		if (ClickKeyList.Contains(_mouse_event.GetEffectingButton()))
		{
			if (_OnButtonClicked.IsBound())
				_OnButtonClicked.Broadcast(this);

			SetButtonState(EButtonState::Hovered);
		}

		if (_UseSubClick)
		{
			if (SubClickKeyList.Contains(_mouse_event.GetEffectingButton()))
			{
				if (_OnButtonSubClicked.IsBound())
					_OnButtonSubClicked.Broadcast(this);

				SetButtonState(EButtonState::Hovered);
			}
		}
	}

	return MoveTemp(reply);
}

FReply UButtonBase::NativeOnMouseButtonDoubleClick(const FGeometry& _geo, const FPointerEvent& _mouse_event)
{
	FReply reply = Super::NativeOnMouseButtonDoubleClick(_geo, _mouse_event);
	
	if (ClickKeyList.Contains(_mouse_event.GetEffectingButton()))
	{
		if (_ButtonType == EButtonType::Click)
		{
			// double click은 button state에서 예외
			PlaySound(_ClickSound);

			if (_OnButtonDoubleClicked.IsBound())
				_OnButtonDoubleClicked.Broadcast(this);
		}
	}

	return MoveTemp(reply);
}

void UButtonBase::ResetButton()
{
	SetButtonState(EButtonState::Normal);
}

void UButtonBase::SetButtonState(EButtonState _state)
{
	if(_ButtonState == _state)
		return;
	_ButtonState = _state;

	UpdateButtonStyle();
}

void UButtonBase::UpdateButtonStyle()
{
	if (IsValid(Img_ButtonStyle))
	{
		auto style_ptr = _ButtonStyle.Find(_ButtonState);
		if (IsValid(style_ptr))
		{
			Img_ButtonStyle->SetBrush(*style_ptr);
		}
	}
}

void UButtonBase::SetClickKeyList(const TSet<FKey>& _key_list)
{
	ClickKeyList = _key_list;
}

void UButtonBase::SetSubClickKeyList(const TSet<FKey>& _key_list)
{
	SubClickKeyList = _key_list;
}
