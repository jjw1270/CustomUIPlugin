// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetBase.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/WidgetTree.h"
#include "MovieScene.h"

void UWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	OnNativeVisibilityChanged.Clear();
	OnNativeVisibilityChanged.AddUObject(this, &UWidgetBase::OnVisibilityChanged);
	
	_OnWidgetStateChanged.Clear();
	_OnWidgetStateChanged.AddUObject(this, &UWidgetBase::OnWidgetStateChanged);
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
		SetWidgetState(EWidgetState::OnShow);
	}
}

void UWidgetBase::OnAnimationFinished_Implementation(const UWidgetAnimation* _anim)
{
	Super::OnAnimationFinished_Implementation(_anim);

	if (IsValid(_anim) && _CurrentAnim == _anim)
	{
		switch (_WidgetState)
		{
		case EWidgetState::OnShow:
			if (_CurrentAnim == ShowAnim)
			{
				SetWidgetState(EWidgetState::Idle);
			}
			break;
		case EWidgetState::OnHide:
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

	_OnWidgetStateChanged.Broadcast(_old_state);
}

void UWidgetBase::OnWidgetStateChanged_Implementation(EWidgetState _old_state)
{
	if (_WidgetState == EWidgetState::Hide)
	{
		HideWidget();
		return;
	}

	UWidgetAnimation* anim_to_play = nullptr;
	bool is_idle = false;

	switch (_WidgetState)
	{
	case EWidgetState::OnShow:
		SetRenderOpacity(1.0f);
		anim_to_play = ShowAnim;
		PlaySound(_ShowSound);
		break;
	case EWidgetState::Idle:
		anim_to_play = IdleAnim;
		is_idle = true;
		break;
	case EWidgetState::OnHide:
		anim_to_play = HideAnim;
		PlaySound(_HideSound);
		break;
	default:
		break;
	}

	StopAnimation(_CurrentAnim); // Event는 호출되지 않는다.

	if (IsValid(anim_to_play))
	{
		_CurrentAnim = MoveTemp(anim_to_play);
		PlayAnimation(_CurrentAnim, 0.0f, is_idle ? 0 : 1);
	}
	else
	{
		if (_WidgetState == EWidgetState::OnShow)
		{
			SetWidgetState(EWidgetState::Idle);
		}
		else if (_WidgetState == EWidgetState::OnHide)
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
		SetWidgetState(EWidgetState::OnHide);
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

void UWidgetBase::BindOnWidgetStateChanged(FD_OnWidgetStateChanged _proc)
{
	_OnWidgetStateChanged.AddWeakLambda(this,
		[Proc(MoveTemp(_proc))](EWidgetState _old_state)
		{
			Proc.ExecuteIfBound(_old_state);
		}
	);
}

void UWidgetBase::BindOnWidgetStateChanged(F_OnWidgetStateChanged& _proc)
{
	_OnWidgetStateChanged.Add(MoveTemp(_proc));
}

void UWidgetBase::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	OnSynchronizeProperties();
}
