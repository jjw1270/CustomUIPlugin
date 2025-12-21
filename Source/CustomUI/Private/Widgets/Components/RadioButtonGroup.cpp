// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/RadioButtonGroup.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"


void URadioButtonGroup::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	UpdateRadioButtons();
}

void URadioButtonGroup::UpdateRadioButtons()
{
	if (IsInvalid(GP_ButtonGroup))
		return;

	auto all_child = GP_ButtonGroup->GetAllChildren();

	GP_ButtonGroup->ClearChildren();

	int32 row = 0;
	int32 col = 0;

	for (auto child : all_child)
	{
		auto radio_button = Cast<URadioButton>(child);
		if (IsValid(radio_button))
		{
			if (radio_button->GetWidgetID().IsNone())
			{
				TRACE_WARNING(TEXT("Radio Button(%s)의 WidgetID가 설정되지 않았습니다!!"), *radio_button->GetName());
			}

			radio_button->_OnClicked.RemoveAll(this);
			radio_button->_OnClicked.AddDynamic(this, &URadioButtonGroup::OnClickRadioButton);

			auto slot = GP_ButtonGroup->AddChildToGrid(radio_button, row, col);
			if (IsValid(slot))
			{
				slot->SetPadding(_ButtonPadding);

				slot->SetHorizontalAlignment(_HorizontalAlignment);
				slot->SetVerticalAlignment(_VerticalAlignment);

				if (_Orientation == EOrientation::Orient_Horizontal)
					col++;
				else
					row++;
			}
		}
		else
		{
			TRACE_ERROR(TEXT("GP_ButtonGroup에는 Radio Button 만 가능합니다!!"));
			GP_ButtonGroup->ClearChildren();
			return;
		}
	}
}

void URadioButtonGroup::OnClickRadioButton(UButtonBase* _btn)
{
	if (IsInvalid(_btn))
		return;

	SelectRadioButtonByWidgetID(_btn->GetWidgetID());
}

void URadioButtonGroup::SelectRadioButtonByWidgetID(FName _widget_id)
{
	for (auto child : GP_ButtonGroup->GetAllChildren())
	{
		auto radio_btn = Cast<URadioButton>(child);
		if (IsValid(radio_btn))
		{
			if (radio_btn->GetWidgetID() == _widget_id)
			{
				radio_btn->SetIsSelected(true);

				if (_OnRadioButtonSelected.IsBound())
					_OnRadioButtonSelected.Broadcast(radio_btn);
			}
			else
			{
				radio_btn->SetIsSelected(false);
			}
		}
	}
}

void URadioButtonGroup::SelectRadioButtonByIndex(int32 _index)
{
	if (_index < 0 || _index >= GP_ButtonGroup->GetChildrenCount())
	{
		TRACE_WARNING(TEXT("out of index : %d"), _index);
		_index = FMath::Clamp(_index, 0, GP_ButtonGroup->GetChildrenCount() - 1);
	}

	int32 idx = 0;
	for (auto child : GP_ButtonGroup->GetAllChildren())
	{
		auto radio_btn = Cast<URadioButton>(child);
		if (IsValid(radio_btn))
		{
			if (idx == _index)
			{
				radio_btn->SetIsSelected(true);
				
				if (_OnRadioButtonSelected.IsBound())
					_OnRadioButtonSelected.Broadcast(radio_btn);
			}
			else
			{
				radio_btn->SetIsSelected(false);
			}

			idx++;
		}
	}
}
