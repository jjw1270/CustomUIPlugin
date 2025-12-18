// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WidgetBase.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/WidgetTree.h"
#include "MovieScene.h"

void UWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	OnNativeVisibilityChanged.Clear();
	OnNativeVisibilityChanged.AddUObject(this, &UWidgetBase::OnVisibilityChanged);
}

void UWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	SetRenderOpacity(0.0f);
}

void UWidgetBase::NativeTick(const FGeometry& _geo, float _delta)
{
	Super::NativeTick(_geo, _delta);

	if (_WidgetState == EWidgetState::Hide)
	{
		SetWidgetState(EWidgetState::Showing);
	}
}

void UWidgetBase::OnAnimationFinished_Implementation(const UWidgetAnimation* _anim)
{
	Super::OnAnimationFinished_Implementation(_anim);

	if (IsValid(_anim) && _CurrentAnim == _anim)
	{
		switch (_WidgetState)
		{
		case EWidgetState::Showing:
			if (_CurrentAnim == ShowAnim)
			{
				SetWidgetState(EWidgetState::Idle);
			}
			break;
		case EWidgetState::Hiding:
			if (_CurrentAnim == HideAnim)
			{
				SetWidgetState(EWidgetState::Hide);
			}
			break;
		default:
			break;
		}
	}
}

void UWidgetBase::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	OnSynchronizeProperties();
}

void UWidgetBase::OnVisibilityChanged(ESlateVisibility _visibility)
{
	if (_visibility == ESlateVisibility::Collapsed || _visibility == ESlateVisibility::Hidden)
	{
		SetRenderOpacity(0.0f);
		_CurrentAnim = nullptr;
		_WidgetState = EWidgetState::Hide;
	}
}

void UWidgetBase::SetWidgetState(EWidgetState _new_state)
{
	if (_WidgetState == _new_state)
		return;

	EWidgetState _old_state = _WidgetState;
	_WidgetState = _new_state;

	OnWidgetStateChanged(_old_state);
}

void UWidgetBase::OnWidgetStateChanged_Implementation(EWidgetState _old_state)
{
	// broadcast events
	switch (_WidgetState)
	{
	case EWidgetState::Hide:
		if (_OnCloseEvent.IsBound())
			_OnCloseEvent.Broadcast(this, (_WidgetHideType == EWidgetHideType::RemoveFromParent));
		break;
	case EWidgetState::Showing:
		if (_OnShowEvent.IsBound())
			_OnShowEvent.Broadcast(this);
		break;
	case EWidgetState::Idle:
		if (_OnIdleEvent.IsBound())
			_OnIdleEvent.Broadcast(this);
		break;
	default:
		break;
	}

	if (_WidgetState == EWidgetState::Hide)
	{
		HideWidget();
		return;
	}

	UWidgetAnimation* anim_to_play = nullptr;
	bool is_idle = false;

	switch (_WidgetState)
	{
	case EWidgetState::Showing:
		SetRenderOpacity(1.0f);
		anim_to_play = ShowAnim;
		PlaySound(_ShowSound);
		break;
	case EWidgetState::Idle:
		anim_to_play = IdleAnim;
		is_idle = true;
		break;
	case EWidgetState::Hiding:
		anim_to_play = HideAnim;
		PlaySound(_HideSound);
		break;
	default:
		break;
	}

	StopAnimation(_CurrentAnim); // Event는 호출되지 않는다.

	if (IsValid(anim_to_play))
	{
		_CurrentAnim = anim_to_play;
		PlayAnimation(_CurrentAnim, 0.0f, is_idle ? 0 : 1);
	}
	else
	{
		if (_WidgetState == EWidgetState::Showing)
		{
			SetWidgetState(EWidgetState::Idle);
		}
		else if (_WidgetState == EWidgetState::Hiding)
		{
			SetWidgetState(EWidgetState::Hide);
		}
	}
}

void UWidgetBase::Hide(EWidgetHideType _type, bool _force_immediately)
{
	if (_type == EWidgetHideType::NA)
	{
		TRACE_ERROR(TEXT("_type이 NA 일 수 없습니다."));
		return;
	}

	if (_WidgetHideType == _type)
		return;
	_WidgetHideType = _type;

	if (_force_immediately)
	{
		HideWidget();
	}
	else
	{
		SetWidgetState(EWidgetState::Hiding);
	}
}

void UWidgetBase::Close(bool _force_immediately)
{
	Hide(EWidgetHideType::RemoveFromParent, _force_immediately);
}

void UWidgetBase::HideWidget()
{
	switch (_WidgetHideType)
	{
	case EWidgetHideType::NA:
		TRACE_ERROR(TEXT("_HideType이 NA 일 수 없습니다."));
		break;
	case EWidgetHideType::RemoveFromParent:
		RemoveFromParent();
		break;
	case EWidgetHideType::Collapsed:
		Super::SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EWidgetHideType::Hidden:
		Super::SetVisibility(ESlateVisibility::Hidden);
		break;
	}

	_WidgetHideType = EWidgetHideType::NA;
}
