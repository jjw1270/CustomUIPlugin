// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetHelper.h"
#include "WidgetSubsystem.h"
#include "Widgets/PageBase.h"
#include "Widgets/PopupBase.h"


UPageBase* UWidgetHelper::OpenPage(const UObject* _world_ctx, TSubclassOf<UPageBase> _page_class)
{
	auto widget_subsys = UCommonUtils::GetLocalPlayerSubsystem<UWidgetSubsystem>(_world_ctx);
	if (IsValid(widget_subsys))
	{
		return widget_subsys->CreatePage(_page_class);
	}

	return nullptr;
}

UPopupBase* UWidgetHelper::OpenPopup(const UObject* _world_ctx, TSubclassOf<UPopupBase> _popup_class)
{
	auto widget_subsys = UCommonUtils::GetLocalPlayerSubsystem<UWidgetSubsystem>(_world_ctx);
	if (IsValid(widget_subsys))
	{
		return widget_subsys->CreatePopup(_popup_class);
	}

	return nullptr;
}

void UWidgetHelper::ClosePopup(UPopupBase* _popup, bool _force_immediately)
{
	if (IsValid(_popup))
	{
		_popup->Close(_force_immediately);
	}
}

UPopupBase* UWidgetHelper::GetTopPopup(const UObject* _world_ctx)
{
	auto widget_subsys = UCommonUtils::GetLocalPlayerSubsystem<UWidgetSubsystem>(_world_ctx);
	if (IsValid(widget_subsys))
	{
		return widget_subsys->GetTopPopup();
	}

	return nullptr;
}

bool UWidgetHelper::IsDesignTime(const UUserWidget* _widget)
{
	if (IsValid(_widget))
	{
		return _widget->IsDesignTime();
	}

	return false;
}
