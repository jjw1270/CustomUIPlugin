// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetSubsystem.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UWidgetSubsystem::Initialize(FSubsystemCollectionBase& _collection)
{

}

void UWidgetSubsystem::Deinitialize()
{
	ClearAllWidgets();
}

void UWidgetSubsystem::PlayerControllerChanged(APlayerController* _new_pc)
{
	ClearAllWidgets();
	RebuildWidgets(Cast<AWidgetPlayerController>(_new_pc));
}

void UWidgetSubsystem::ClearAllWidgets()
{
	if (IsValid(_CurrentPage))
	{
		_CurrentPage->RemoveFromParent();
		_CurrentPage = nullptr;
	}

	for (UPopupBase* popup : _CurrentPopups)
	{
		if (IsValid(popup))
		{
			popup->RemoveFromParent();
		}
	}
	_CurrentPopups.Empty();
}

void UWidgetSubsystem::RebuildWidgets(AWidgetPlayerController* _pc)
{
	if (IsInvalid(_pc))
	{
		TRACE_ERROR(TEXT("Player Controller는 반드시 WidgetPlayerController를 상속받아야 합니다!"));
		return;
	}

	// page
	if (IsValid(_RemainingPageClass))
	{
		CreatePage(_pc, _RemainingPageClass);
	}
	else
	{
		CreatePage(_pc, _pc->GetInitialPageClass());
	}

	// popups
	for (const auto& remaining_popup_class : _RemainingPopupClasses)
	{
		CreatePopup(_pc, remaining_popup_class);
	}
}

void UWidgetSubsystem::CreatePage(AWidgetPlayerController* _pc, TSubclassOf<UPageBase> _page_class)
{
	if (IsInvalid(_pc))
		return;

	if (IsInvalid(_page_class))
	{
		TRACE_ERROR(TEXT("_page_class Invalid!!"));
		return;
	}

	// 현재 page가 존재함.
	if (IsValid(_CurrentPage) && _CurrentPage->IsInViewport())
	{
		if (_CurrentPage->StaticClass() == _page_class)
		{
			// 같으면 무시
			return;
		}
		else
		{
			// 이전 Page 닫아야 함
			_CurrentPage->RemoveFromParent();
			_CurrentPage = nullptr;
		}
	}

	_CurrentPage = CreateWidget<UPageBase>(_pc, _page_class);
	if (IsValid(_CurrentPage))
	{
		_CurrentPage->AddToPlayerScreen((int32)EPopupStackPriority::Page);

		// set is remain on level changed
		if (_CurrentPage->GetConfig().RemainOnLevelChanged)
		{
			_RemainingPageClass = _page_class;
		}
		else
		{
			_RemainingPageClass = nullptr;
		}

		// set input mode
		switch (_CurrentPage->GetConfig().InputMode)
		{
		case EInputMode::GameAndUI:
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(_pc,
				_CurrentPage->GetConfig().SetFocus ? _CurrentPage : nullptr,
				_CurrentPage->GetConfig().InMouseLockMode,
				_CurrentPage->GetConfig().HideCursorDuringCapture,
				_CurrentPage->GetConfig().FlushInput);
			break;
		case EInputMode::GameOnly:
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(_pc,
				_CurrentPage->GetConfig().FlushInput);
			break;
		case EInputMode::UIOnly:
			UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(_pc,
				_CurrentPage->GetConfig().SetFocus ? _CurrentPage : nullptr,
				_CurrentPage->GetConfig().InMouseLockMode,
				_CurrentPage->GetConfig().FlushInput);
			break;
		}

		_pc->SetShowMouseCursor(_CurrentPage->GetConfig().ShowMouseCursor);
	}
}

void UWidgetSubsystem::CreatePopup(AWidgetPlayerController* _pc, TSubclassOf<UPopupBase> _popup_class)
{
	if (IsInvalid(_pc))
		return;

	if (IsInvalid(_popup_class))
	{
		TRACE_ERROR(TEXT("_popup_class Invalid!!"));
		return;
	}

	auto popup = CreateWidget<UPopupBase>(_pc, _popup_class);
	if (IsValid(popup))
	{
		popup->AddToPlayerScreen((int32)popup->GetConfig().StackPriority);

		// set is remain on level changed
		if (popup->GetConfig().RemainOnLevelChanged)
		{
			_RemainingPopupClasses.Add(_popup_class);
		}

		_pc->SetShowMouseCursor(popup->GetConfig().ShowMouseCursor);

		_CurrentPopups.Add(popup);
	}
}

void UWidgetHelpers::OpenPage(const UObject* _world_ctx, TSubclassOf<UPageBase> _page_class)
{
	auto widget_subsys = UCommonUtils::GetLocalPlayerSubsystem<UWidgetSubsystem>(_world_ctx);
	if (IsInvalid(widget_subsys))
		return;

	auto local_player = widget_subsys->GetLocalPlayer();
	if (IsInvalid(local_player))
		return;

	auto pc = Cast<AWidgetPlayerController>(local_player->GetPlayerController(nullptr));
	if (IsInvalid(pc))
		return;

	widget_subsys->CreatePage(pc, MoveTemp(_page_class));
}

void UWidgetHelpers::OpenPopup(const UObject* _world_ctx, TSubclassOf<UPopupBase> _popup_class)
{
	auto widget_subsys = UCommonUtils::GetLocalPlayerSubsystem<UWidgetSubsystem>(_world_ctx);
	if (IsInvalid(widget_subsys))
		return;

	auto local_player = widget_subsys->GetLocalPlayer();
	if (IsInvalid(local_player))
		return;

	auto pc = Cast<AWidgetPlayerController>(local_player->GetPlayerController(nullptr));
	if (IsInvalid(pc))
		return;

	widget_subsys->CreatePopup(pc, MoveTemp(_popup_class));
}
