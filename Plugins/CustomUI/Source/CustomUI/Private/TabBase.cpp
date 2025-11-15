// Fill out your copyright notice in the Description page of Project Settings.


#include "TabBase.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

void UTabBase::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (IsInvalid(UniformGridPanel))
		return;

	UniformGridPanel->SetSlotPadding(_ButtonPadding);

	if (!_CurrentOrientation.IsSet() || _Orientation != _CurrentOrientation.GetValue())
	{ 
		_CurrentOrientation = _Orientation;
		CreateTabButtons();
	}
}

void UTabBase::SelectTab(FName _tab_id)
{
	for (auto child : UniformGridPanel->GetAllChildren())
	{
		auto tab_btn = Cast<UTabButton>(child);
		if (IsValid(tab_btn))
		{
			if (tab_btn->GetTabID() == _tab_id)
			{
				OnClickTabButton(tab_btn);
				break;
			}
		}
	}
}

void UTabBase::SelectTabIndex(int32 _tab_idx)
{
	auto tab_btn = Cast<UTabButton>(UniformGridPanel->GetChildAt(_tab_idx));
	if (IsValid(tab_btn))
	{
		OnClickTabButton(tab_btn);
	}
	else
	{
		TRACE_WARNING(TEXT("Invalid Tab Index : %d"), _tab_idx);
	}
}

void UTabBase::CreateTabButtons()
{
	if (IsInvalid(_TabButtonClass))
		return;

	UniformGridPanel->ClearChildren();

	int32 row = 0;
	int32 col = 0;
	for (const auto& tab_btn_pair : _TabButtonMap)
	{
		auto tab_btn = CreateWidget<UTabButton>(this, _TabButtonClass);
		if (IsValid(tab_btn))
		{
			tab_btn->_OnButtonClicked.AddDynamic(this, &UTabBase::OnClickTabButton);

			tab_btn->InitWidget(tab_btn_pair.Key, tab_btn_pair.Value);

			auto slot = UniformGridPanel->AddChildToUniformGrid(tab_btn, row, col);
			if (IsValid(slot))
			{
				if (_CurrentOrientation.GetValue() == EOrientation::Orient_Horizontal)
					col++;
				else
					row++;
			}
		}
	}
}

void UTabBase::OnClickTabButton(UButtonBase* _btn)
{
	if (IsInvalid(_btn))
		return;

	for (auto child : UniformGridPanel->GetAllChildren())
	{
		auto tab_btn = Cast<UTabButton>(child);
		if(IsValid(tab_btn))
		{
			if (tab_btn == _btn)
			{
				if (_OnTabSelected.IsBound())
					_OnTabSelected.Broadcast(this, tab_btn->GetTabID());
			}
			else
			{
				tab_btn->ResetButton();
			}
		}
	}
}
