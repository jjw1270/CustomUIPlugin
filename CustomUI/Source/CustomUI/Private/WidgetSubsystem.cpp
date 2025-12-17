// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetSubsystem.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UWidgetSubsystem::Initialize(FSubsystemCollectionBase& _collection)
{

}

void UWidgetSubsystem::Deinitialize()
{
	ClearAllWidgets(true);
}

void UWidgetSubsystem::PlayerControllerChanged(APlayerController* _new_pc)
{
	ClearAllWidgets(true);
	RebuildWidgets(Cast<AWidgetPlayerController>(_new_pc));
}

void UWidgetSubsystem::ClearAllWidgets(bool _clear_close_event)
{
	if (IsValid(_CurrentPage))
	{
		if (_clear_close_event)
			_CurrentPage->_OnCloseEvent.Clear();

		_CurrentPage->Close(true);
		_CurrentPage = nullptr;
	}

	for (UPopupBase* popup : _CurrentPopups)
	{
		if (IsValid(popup))
		{
			if (_clear_close_event)
				popup->_OnCloseEvent.Clear();

			popup->Close(true);
		}
	}
	_CurrentPopups.Empty();
}

void UWidgetSubsystem::RebuildWidgets(AWidgetPlayerController* _pc)
{
	if (IsInvalid(_pc))
		return;

	// page
	if (IsValid(_RemainingPageClass))
	{
		CreatePage(_RemainingPageClass);
	}
	else
	{
		CreatePage(_pc->GetInitialPageClass());
	}

	// popups
	for (const auto& remaining_popup_class : _RemainingPopupClasses)
	{
		CreatePopup(remaining_popup_class);
	}
}

UPageBase* UWidgetSubsystem::CreatePage(TSubclassOf<UPageBase> _page_class)
{
	auto local_player = GetLocalPlayer();
	if (IsInvalid(local_player))
		return nullptr;

	auto pc = Cast<AWidgetPlayerController>(local_player->GetPlayerController(GetWorld()));
	if (IsInvalid(pc))
		return nullptr;

	if (IsValid(_CurrentPage) && _CurrentPage->IsInViewport())
	{
		if (_CurrentPage->GetClass() == _page_class.Get())
		{
			// 이미 열려있는 경우
			return _CurrentPage;
		}
		else
		{
			// 이전 page 닫아줌
			_CurrentPage->Close(true);
			_CurrentPage = nullptr;
		}
	}

	if (IsInvalid(_page_class))
	{
		TRACE_ERROR(TEXT("_page_class Invalid!!"));
		return nullptr;
	}

	_CurrentPage = CreateWidget<UPageBase>(pc, _page_class);
	if (IsValid(_CurrentPage))
	{
		_CurrentPage->AddToPlayerScreen((int32)EWidgetZOrder::Page);

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
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(pc,
				_CurrentPage->GetConfig().SetFocus ? _CurrentPage : nullptr,
				_CurrentPage->GetConfig().InMouseLockMode,
				_CurrentPage->GetConfig().HideCursorDuringCapture,
				_CurrentPage->GetConfig().FlushInput);
			break;
		case EInputMode::GameOnly:
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(pc,
				_CurrentPage->GetConfig().FlushInput);
			break;
		case EInputMode::UIOnly:
			UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(pc,
				_CurrentPage->GetConfig().SetFocus ? _CurrentPage : nullptr,
				_CurrentPage->GetConfig().InMouseLockMode,
				_CurrentPage->GetConfig().FlushInput);
			break;
		}

		pc->SetShowMouseCursor(_CurrentPage->GetConfig().ShowMouseCursor);
	}

	return _CurrentPage;
}

UPopupBase* UWidgetSubsystem::CreatePopup(TSubclassOf<UPopupBase> _popup_class)
{
	auto local_player = GetLocalPlayer();
	if (IsInvalid(local_player))
		return nullptr;

	auto pc = Cast<AWidgetPlayerController>(local_player->GetPlayerController(GetWorld()));
	if (IsInvalid(pc))
		return nullptr;


	if (CanOpenPopup(_popup_class) == false)
		return nullptr;

	auto popup = CreateWidget<UPopupBase>(pc, _popup_class);
	if (IsValid(popup))
	{
		popup->_OnCloseEvent.AddDynamic(this, &UWidgetSubsystem::OnPopupClosed);

		popup->AddToPlayerScreen((int32)EWidgetZOrder::Popup);

		// set is remain on level changed
		if (popup->GetConfig().RemainOnLevelChanged)
		{
			_RemainingPopupClasses.AddUnique(_popup_class);
		}

		pc->SetShowMouseCursor(popup->GetConfig().ShowMouseCursor);

		_CurrentPopups.Add(popup);
	}

	return popup;
}

void UWidgetSubsystem::OnPopupClosed(UWidgetBase* _widget, bool _is_removed)
{
	auto popup = Cast<UPopupBase>(_widget);
	if (IsInvalid(popup))
		return;

	auto local_player = GetLocalPlayer();
	if (IsInvalid(local_player))
		return;

	auto pc = Cast<AWidgetPlayerController>(local_player->GetPlayerController(GetWorld()));
	if (IsInvalid(pc))
		return;

	if (popup->GetConfig().RemainOnLevelChanged)
	{
		_RemainingPopupClasses.Remove(popup->GetClass());
	}

	pc->SetShowMouseCursor(!popup->GetConfig().ShowMouseCursor);

	if(_CurrentPopups.Contains(popup))
	{
		_CurrentPopups.Remove(popup);
	}

	// update mouse cursor
	auto top_popup = GetTopPopup();
	if (IsValid(top_popup))
	{
		pc->SetShowMouseCursor(top_popup->GetConfig().ShowMouseCursor);
	}
	else if(IsValid(_CurrentPage))
	{
		pc->SetShowMouseCursor(_CurrentPage->GetConfig().ShowMouseCursor);
	}
}

UPopupBase* UWidgetSubsystem::GetTopPopup() const
{
	if (_CurrentPopups.IsEmpty() == false)
	{
		return _CurrentPopups.Last();
	}

	return nullptr;
}

bool UWidgetSubsystem::CanOpenPopup(TSubclassOf<UPopupBase> _popup_class) const
{
	if (IsInvalid(_popup_class))
	{
		TRACE_ERROR(TEXT("_popup_class Invalid!!"));
		return false;
	}

	auto default_class = _popup_class.GetDefaultObject();
	if (IsInvalid(default_class))
		return false;

	if (default_class->GetConfig().AllowDuplicate == false)
	{
		if (IsPopupOpened(_popup_class))
		{
			TRACE_WARNING(TEXT("이미 열려있습니다 : %s"), *_popup_class->GetName());
			return false;
		}
	}

	return true;
}

bool UWidgetSubsystem::IsPopupOpened(TSubclassOf<UPopupBase> _popup_class) const
{
	for (const auto popup : _CurrentPopups)
	{
		if (IsValid(popup))
		{
			if (popup->GetClass() == _popup_class.Get())
				return true;
		}
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

UWidgetSubsystem* UWidgetHelpers::GetWidgetSubsystem(const UObject* _world_ctx)
{
	return UCommonUtils::GetLocalPlayerSubsystem<UWidgetSubsystem>(_world_ctx);
}

AWidgetPlayerController* UWidgetHelpers::GetPlayerController(const UObject* _world_ctx)
{
	auto widget_subsys = GetWidgetSubsystem(_world_ctx);
	if (IsValid(widget_subsys))
	{
		auto local_player = widget_subsys->GetLocalPlayer();
		if (IsValid(local_player))
		{
			return Cast<AWidgetPlayerController>(local_player->GetPlayerController(widget_subsys->GetWorld()));
		}
	}

	return nullptr;
}

UPageBase* UWidgetHelpers::OpenPage(const UObject* _world_ctx, TSubclassOf<UPageBase> _page_class)
{
	auto widget_subsys = GetWidgetSubsystem(_world_ctx);
	if (IsValid(widget_subsys))
	{
		return widget_subsys->CreatePage(_page_class);
	}

	return nullptr;
}

UPopupBase* UWidgetHelpers::OpenPopup(const UObject* _world_ctx, TSubclassOf<UPopupBase> _popup_class)
{
	auto widget_subsys = GetWidgetSubsystem(_world_ctx);
	if (IsValid(widget_subsys))
	{
		return widget_subsys->CreatePopup(_popup_class);
	}

	return nullptr;
}

void UWidgetHelpers::ClosePopup(UPopupBase* _popup, bool _force_immediately)
{
	if (IsValid(_popup))
	{
		_popup->Close(_force_immediately);
	}
}

UPopupBase* UWidgetHelpers::GetTopPopup(const UObject* _world_ctx)
{
	auto widget_subsys = GetWidgetSubsystem(_world_ctx);
	if (IsValid(widget_subsys))
	{
		return widget_subsys->GetTopPopup();
	}

	return nullptr;
}
